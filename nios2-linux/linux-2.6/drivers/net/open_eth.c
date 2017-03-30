/*--------------------------------------------------------------------
 * open_eth.c
 *
 * Ethernet driver for Open Ethernet Controller (www.opencores.org).
 *
 * Based on:
 *
 * Ethernet driver for Motorola MPC8xx.
 *      Copyright (c) 1997 Dan Malek (dmalek@jlc.net)
 *
 * mcen302.c: A Linux network driver for Mototrola 68EN302 MCU
 *
 *      Copyright (C) 1999 Aplio S.A. Written by Vadim Lebedev
 *
 * Copyright (c) 2002 Simon Srot (simons@opencores.org)
 * Copyright (C) 2004 Microtronix Datacom Ltd
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 *
 * History:
 *    Jun/20/2004   DGT Microtronix Datacom NiosII
 *
 ---------------------------------------------------------------------*/

#include <linux/etherdevice.h>
#include <linux/crc32.h>
#include <linux/io.h>
#include <linux/mii.h>
#include <linux/phy.h>
#include <linux/platform_device.h>
#include <net/ethoc.h>

#define OETH_REVISION_DATECODE 20050321

#include "open_eth.h"
#include "net/open_eth.h"

#define OETH_MII_TIMEOUT	(1 + (HZ / 5))

#define OETH_DEBUG 0

#if OETH_DEBUG > 1
    #define PRINTK2(args...) printk(args)
#else
    #define PRINTK2(args...)
#endif  // OETH_DEBUG > 1

#ifdef OETH_DEBUG
    #define PRINTK(args...) printk(args)
#else
    #define PRINTK(args...)
#endif  // OETH_DEBUG

/* The transmitter timeout */
#define TX_TIMEOUT  (2*HZ)

/* Buffer number (must be 2^n)
 */
//;dgt;;;#define OETH_RXBD_NUM      8
#define OETH_RXBD_NUM       16
//;dgt;;;#define OETH_TXBD_NUM      8
#define OETH_TXBD_NUM       16

#define OETH_RXBD_NUM_MASK  (OETH_RXBD_NUM-1)
#define OETH_TXBD_NUM_MASK  (OETH_TXBD_NUM-1)

/* Buffer size
 */
#define OETH_RX_BUFF_SIZE   2048
#define OETH_TX_BUFF_SIZE   2048

static int buffer_size = OETH_RXBD_NUM * OETH_RX_BUFF_SIZE +
	OETH_TXBD_NUM * OETH_TX_BUFF_SIZE;

module_param(buffer_size, int, 0);
MODULE_PARM_DESC(buffer_size, "DMA buffer allocation size");

/* The buffer descriptors track the ring buffers.
 */
struct oeth_private {
	void __iomem*			iobase;
	void __iomem*			membase;
	int						dma_alloc;
	s8						phy_id;

	struct sk_buff*			rx_skbuff[OETH_RXBD_NUM];
    struct sk_buff*			tx_skbuff[OETH_TXBD_NUM];

    ushort					tx_next;   /* Next buffer to be sent */
    ushort					tx_last;   /* Next buffer to be checked if packet sent */
    ushort					tx_full;   /* Buffer ring fuul indicator */
    ushort					rx_cur;    /* Next buffer to be checked if packet received */

    oeth_regs*				regs;      /* Address of controller registers. */
    oeth_bd*				rx_bd_base;/* Address of Rx BDs. */
    oeth_bd*				tx_bd_base;/* Address of Tx BDs. */

	struct net_device*		netdev;
    struct net_device_stats	stats;
    struct tasklet_struct	oeth_rx_tasklet;
    struct tasklet_struct	oeth_tx_tasklet;
	struct mii_bus*			mdio;
	struct phy_device*		phy;
	spinlock_t      		lock;
};

#if OETH_DEBUG
static void
oeth_print_packet(unsigned long add, int len)
{
    int i;

    printk("ipacket: add = %lx len = %d\n", add, len);
    for(i = 0; i < len; i++) {
        if(!(i % 16))
                printk("\n");
        printk(" %.2x", *(((unsigned char *)add) + i));
    }
    printk("\n");
}
#endif

static int oeth_ioctl(struct net_device *dev, struct ifreq *ifr, int cmd)
{
	struct oeth_private *priv = netdev_priv(dev);
	struct mii_ioctl_data *mdio = if_mii(ifr);
	struct phy_device *phy = NULL;
	int ret;

	if (!netif_running(dev))
		return -EINVAL;

	if (cmd != SIOCGMIIPHY) {
		if (mdio->phy_id >= PHY_MAX_ADDR)
			return -ERANGE;

		phy = priv->mdio->phy_map[mdio->phy_id];
		if (!phy)
			return -ENODEV;
	} else {
		phy = priv->phy;
	}

	ret = phy_mii_ioctl(phy, mdio, cmd);
#if 0
	if (ret != 0)
		printk("phy_mii_ioctl(phy=%x, cmd=%x) = %d\n", phy, cmd, ret);
#endif

	return ret;
}

static int oeth_mdio_read(struct mii_bus *bus, int phy, int reg)
{
	unsigned long timeout = jiffies + OETH_MII_TIMEOUT;
	struct oeth_private *priv = bus->priv;
	volatile oeth_regs* regs = priv->regs;

	regs->miiaddress = (reg << 8) | phy;
	regs->miicommand = OETH_MIICOMMAND_RSTAT;

	while (time_before(jiffies, timeout)) {
		u32 status = regs->miistatus;
		if (!(status & OETH_MIISTATUS_BUSY)) {
			u32 data = regs->miirx_data;
			/* reset MII command register */
			regs->miicommand = 0;
			return data;
		}

		schedule();
	}

	return -EBUSY;
}

static int oeth_mdio_write(struct mii_bus *bus, int phy, int reg, u16 val)
{
	unsigned long timeout = jiffies + OETH_MII_TIMEOUT;
	struct oeth_private *priv = bus->priv;
	volatile oeth_regs* regs = priv->regs;

	regs->miiaddress = (reg << 8) | phy;
	regs->miitx_data = val;
	regs->miicommand = OETH_MIICOMMAND_WCTRLDATA;

	while (time_before(jiffies, timeout)) {
		u32 stat = regs->miistatus;
		if (!(stat & OETH_MIISTATUS_BUSY))
			return 0;

		schedule();
	}

	return -EBUSY;
}

static int oeth_mdio_reset(struct mii_bus *bus)
{
	return 0;
}

static void oeth_mdio_poll(struct net_device *dev)
{
}

static int oeth_mdio_probe(struct net_device *dev)
{
	struct oeth_private *priv = netdev_priv(dev);
	struct phy_device *phy;
	int i;

	for (i = 0; i <PHY_MAX_ADDR; i++) {
		phy = priv->mdio->phy_map[i];
		if (phy) {
			if (priv->phy_id != -1) {
				/* attach to specified PHY */
				if (priv->phy_id == phy->addr)
					break;
			} else {
				/* autoselect PHY if none was specified */
				if (phy->addr != 0)
					break;
			}
		}
	}

	if (!phy) {
		dev_err(&dev->dev, "no PHY found\n");
		return -ENXIO;
	}

	phy = phy_connect(dev, dev_name(&phy->dev), &oeth_mdio_poll, 0,
			PHY_INTERFACE_MODE_GMII);
	if (IS_ERR(phy)) {
		dev_err(&dev->dev, "could not attach to PHY\n");
		return PTR_ERR(phy);
	}

	priv->phy = phy;
	return 0;
}

/*
    Entered at interrupt level
*/
static void
oeth_tx(unsigned long devn)
{
	struct net_device *dev = (void *)devn;
	volatile struct oeth_private *priv = (struct oeth_private *)netdev_priv(dev);
	volatile oeth_bd *bdp;

	// Cycles over the TX BDs, starting at the first one that would've been sent. -TS
	while (1)
	{
		bdp = priv->tx_bd_base + priv->tx_last;

		// Stops once it runs into the first one that's ready for transmit (and thus hasn't been sent yet), or once it has checked all BDs (which would occur if all have been transmitted). -TS
        if ((bdp->len_status & OETH_TX_BD_READY) ||
            ((priv->tx_last == priv->tx_next) && !priv->tx_full))
            break;

        /* Check status for errors */
		if (bdp->len_status & 0x1ff)
			printk("oeth: tx%02d,%08x\n", priv->tx_last,bdp->len_status);

        if (bdp->len_status & OETH_TX_BD_LATECOL)
            priv->stats.tx_window_errors++;

        if (bdp->len_status & OETH_TX_BD_RETLIM)
            priv->stats.tx_aborted_errors++;

        if (bdp->len_status & OETH_TX_BD_UNDERRUN)
            priv->stats.tx_fifo_errors++;

        if (bdp->len_status & OETH_TX_BD_CARRIER)
            priv->stats.tx_carrier_errors++;

        if (bdp->len_status & (OETH_TX_BD_LATECOL   |
                               OETH_TX_BD_RETLIM    |
                               OETH_TX_BD_UNDERRUN))
            priv->stats.tx_errors++;

        priv->stats.tx_packets++;
        priv->stats.tx_bytes += bdp->len_status >> 16;
        priv->stats.collisions += (bdp->len_status >> 4) & 0x000f;

		if (priv->tx_full)
            priv->tx_full = 0;
		priv->tx_last = (priv->tx_last + 1) & OETH_TXBD_NUM_MASK;

      }

      if(((priv->tx_next + 1) & OETH_TXBD_NUM_MASK) != priv->tx_last)
	netif_wake_queue(dev);
}

/*
    Entered at interrupt level
*/
static void
oeth_rx(unsigned long devn)
{
    struct net_device*            dev = (void *)devn;
    volatile struct oeth_private* priv = (struct oeth_private *)netdev_priv(dev);
    volatile        oeth_bd*      bdp;
    struct          sk_buff*      skb;
    int                           pkt_len;
    int                           bad;
    int                           netif_rx_rtnsts;

    /* First, grab all of the stats for the incoming packet.
     * These get messed up if we get called due to a busy condition.
     */
    for (;;priv->rx_cur = (priv->rx_cur + 1) & OETH_RXBD_NUM_MASK)
    {
        bad = 0;                                                //;dgt
        bdp = priv->rx_bd_base + priv->rx_cur;

        if (bdp->len_status & OETH_RX_BD_EMPTY)
            break;

        pkt_len = bdp->len_status >> 16;


        /* Check status for errors.
         */
		//if (bdp->len_status & 0x1ff) printk("oeth: rx%02d,%08x\n",priv->rx_cur,bdp->len_status);

        if (bdp->len_status & (OETH_RX_BD_TOOLONG | OETH_RX_BD_SHORT)) {
            priv->stats.rx_length_errors++;
            bad = 1;
        }
        if (bdp->len_status & OETH_RX_BD_DRIBBLE) {
            priv->stats.rx_frame_errors++;
            bad = 1;
        }
        if (bdp->len_status & OETH_RX_BD_CRCERR) {
            priv->stats.rx_crc_errors++;
            bad = 1;
        }
        if (bdp->len_status & OETH_RX_BD_OVERRUN) {
            priv->stats.rx_crc_errors++;
            bad = 1;
        }
        if (bdp->len_status & OETH_RX_BD_MISS) {
            //;dgt - identifies a packet received in promiscuous
            //;dgt    mode (would not have otherwise been accepted)
        }
        if (bdp->len_status & OETH_RX_BD_LATECOL) {
            priv->stats.rx_frame_errors++;
            bad = 1;
        }
        if (bdp->len_status & OETH_RX_BD_INVSIMB) {             //;dgt
            priv->stats.rx_frame_errors++;                       //;dgt
            bad = 1;                                            //;dgt
        }                                                       //;dgt
        if (bdp->len_status & (OETH_RX_BD_TOOLONG   |           //;dgt
                               OETH_RX_BD_SHORT     |           //;dgt
                               OETH_RX_BD_CRCERR    |           //;dgt
                               OETH_RX_BD_OVERRUN)              //;dgt
            )
            priv->stats.rx_errors++;                             //;dgt

        if (bad) {
            bdp->len_status &= ~OETH_RX_BD_STATS;
            bdp->len_status |= OETH_RX_BD_EMPTY;

            continue;
        }

        /* Process the incoming frame.
         */

	/* Strip the CRC. It is not supposed to be passed by Linux Ethernet
	   drivers. (Many things will work regardless, but not all; e.g.,
	   802.1d bridging.) */
		pkt_len -= 4;

#ifdef CONFIG_NIOS2

        //;dgt Over allocate 2 extra bytes to
        //;dgt  32 bit align Nios 32 bit
        //;dgt  IP/TCP fields.
        //;dgt Over allocate 3 extra bytes to
        //;dgt  allow packet to be treated as
        //;dgt  as an even number of bytes or
        //;dgt  16 bit words if so desired.
        //;dgt Plus another extra 4 paranoia bytes.
	    skb = netdev_alloc_skb(dev, pkt_len + 2 + 3 + 4);   //;dgt
#else
		skb = netdev_alloc_skb(dev, pkt_len);
#endif

	    if (skb == NULL)
		{
			printk("%s: Memory squeeze, dropping packet.\n", dev->name);
			priv->stats.rx_dropped++;
		}
	    else
	    {
		skb->dev = dev;
#if OETH_DEBUG
	        printk("RX\n");
	        oeth_print_packet((unsigned long)phys_to_virt(bdp->addr), pkt_len);
#endif
#ifdef CONFIG_NIOS2
			{
	            unsigned short     *dst;
	            unsigned short     *src;

		skb_reserve( skb, 2 );
              //;dgt 32 bit align Nios 32 bit IP/TCP fields

	            dst = ((unsigned short *) (skb_put(skb, pkt_len)));
	            src = ((unsigned short *) (__va(bdp->addr)));

				memcpy(dst, src, pkt_len);
			}
#else
			memcpy(skb_put(skb, pkt_len), __va(bdp->addr), pkt_len);
#endif // CONFIG_NIOS2

            skb->protocol = eth_type_trans(skb,dev);

            netif_rx_rtnsts = netif_rx(skb);                    //;dgt

            switch (netif_rx_rtnsts)                          //;dgt
            {                                               //;dgt
            case NET_RX_DROP:                     // 0x01 //;dgt
		priv->stats.rx_dropped++;                    //;dgt
                break;                                      //;dgt
            }                                               //;dgt

            priv->stats.rx_packets++; // This is the only thing that increments the packet stat
		    priv->stats.rx_bytes += pkt_len;
        }

		bdp->len_status &= ~OETH_RX_BD_STATS;
        bdp->len_status |= OETH_RX_BD_EMPTY;
	}
}


/*-----------------------------------------------------------
 | Driver entry point
 |
 | Entry condition: Cpu interrupts DISabled.
 */
static irqreturn_t oeth_interrupt(int irq, void* dev_id)
{
	struct net_device*		dev = dev_id;
	struct oeth_private*	priv = (struct oeth_private *)netdev_priv(dev);
	volatile oeth_regs*		regs = priv->regs;
	uint					int_events;

    /* Get the interrupt events that caused us to be here.
     */
    int_events = regs->int_src;
    regs->int_src = int_events;

    /* Handle receive event in its own function.
     */
    if (int_events & (OETH_INT_RXF | OETH_INT_RXE | OETH_INT_BUSY))
        tasklet_schedule(&priv->oeth_rx_tasklet);

    /* Handle transmit event in its own function.
     */
    if (int_events & (OETH_INT_TXB | OETH_INT_TXE))
        tasklet_schedule(&priv->oeth_tx_tasklet);

    /* Check for receive busy, i.e. packets coming but no place to
     * put them.
     */
    if (int_events & OETH_INT_BUSY)
      {
        priv->stats.rx_dropped++;                                //;dgt
      }

    return IRQ_HANDLED;
}

static int
oeth_open(struct net_device *dev)
{
    struct oeth_private* priv = (struct oeth_private *)netdev_priv(dev);
    volatile oeth_regs*  regs = priv->regs;
    void __iomem*        mem_addr = priv->membase;
    volatile oeth_bd*    tx_bd = (oeth_bd *)(priv->iobase + OETH_BD_OFS);
    volatile oeth_bd*    rx_bd = tx_bd + OETH_TXBD_NUM;
	unsigned int         mode;
    int                  i;
	int                  ret = 0;

    /* Install our interrupt handler.
     */
	regs->int_mask = 0;	// make sure we won't be interrupted yet
	ret = request_irq(dev->irq, oeth_interrupt, IRQF_SHARED,
			dev->name, dev);
	if (ret)
		return ret;

    /* Initialize TXBDs.
     */
    for (i=0; i<OETH_TXBD_NUM; i++) {
		tx_bd[i].len_status = OETH_TX_BD_PAD | OETH_TX_BD_CRC | OETH_RX_BD_IRQ;
		tx_bd[i].addr = __pa(mem_addr);
		mem_addr += OETH_TX_BUFF_SIZE;
    }
    tx_bd[OETH_TXBD_NUM - 1].len_status |= OETH_TX_BD_WRAP;

    /* Initialize RXBDs.
     */
    for(i=0; i<OETH_RXBD_NUM; i++) {
	rx_bd[i].len_status = OETH_RX_BD_EMPTY | OETH_RX_BD_IRQ;
	rx_bd[i].addr = __pa(mem_addr);
	mem_addr += OETH_RX_BUFF_SIZE;
    }
    rx_bd[OETH_RXBD_NUM - 1].len_status |= OETH_RX_BD_WRAP;

	/* enable FCS generation and automatic padding */
	mode = regs->moder;
	mode |= OETH_MODER_CRCEN | OETH_MODER_PAD /*| OETH_MODER_IFG*/;
	regs->moder = mode;

	/* set full-duplex mode */
	mode = regs->moder;
	mode |= OETH_MODER_FULLD;
	regs->moder = mode;

	regs->ipgt = 0x00000015;


    /* Zero the BD pointers.
     */
    priv->rx_cur = 0;
    priv->tx_next = 0;
    priv->tx_last = 0;
    priv->tx_full = 0;

	if (netif_queue_stopped(dev)) {
		dev_dbg(&dev->dev, " resuming queue\n");
		netif_wake_queue(dev);
	} else {
		dev_dbg(&dev->dev, " starting queue\n");
		netif_start_queue(dev);
	}

	phy_start(priv->phy);

    tasklet_init(&priv->oeth_rx_tasklet, oeth_rx, (unsigned long)dev);
    tasklet_init(&priv->oeth_tx_tasklet, oeth_tx, (unsigned long)dev);

    /* Clear all pending interrupts */
    regs->int_src = 0xffffffff;

    /* Enable interrupt sources */
    regs->int_mask = OETH_INT_MASK_TXB   |
                     OETH_INT_MASK_TXE   |
                     OETH_INT_MASK_RXF   |
                     OETH_INT_MASK_RXE   |
                     OETH_INT_MASK_BUSY  |
                     OETH_INT_MASK_TXC   |
                     OETH_INT_MASK_RXC;

    /* Enable receiver and transmiter */
    regs->moder |= OETH_MODER_RXEN | OETH_MODER_TXEN;

    return ret;
}

static int
oeth_close(struct net_device *dev)
{
    struct oeth_private *priv = (struct oeth_private *)netdev_priv(dev);
    volatile oeth_regs *regs = (oeth_regs *)dev->base_addr;
    volatile oeth_bd *bdp;
    int i;

    tasklet_kill(&priv->oeth_tx_tasklet);
    tasklet_kill(&priv->oeth_rx_tasklet);

	if (priv->phy)
		phy_stop(priv->phy);

    /* Free interrupt hadler
     */
    free_irq(dev->irq, (void *)dev);

    /* Disable receiver and transmitesr
     */
    regs->moder &= ~(OETH_MODER_RXEN | OETH_MODER_TXEN);

	if (!netif_queue_stopped(dev))
	    netif_stop_queue(dev);

    bdp = priv->rx_bd_base;
    for (i = 0; i < OETH_RXBD_NUM; i++) {
        bdp->len_status &= ~(OETH_RX_BD_STATS | OETH_RX_BD_EMPTY);
        bdp++;
    }

    bdp = priv->tx_bd_base;
    for (i = 0; i < OETH_TXBD_NUM; i++) {
        bdp->len_status &= ~(OETH_TX_BD_STATS | OETH_TX_BD_READY);
        bdp++;
    }

    return 0;
}

// Queues a packet for transmission by the OETH. -TS
static int
oeth_start_xmit(struct sk_buff *skb, struct net_device *dev)
{
    volatile struct oeth_private *priv = (struct oeth_private *)netdev_priv(dev);
    volatile        oeth_bd      *bdp;
    unsigned        int           lenSkbDataByts;

    netif_stop_queue(dev);

    if (priv->tx_full) {
        //;dgt-"Impossible", but in any event, queue may have been
        //;dgt- reawakened by now.
        /* All transmit buffers are full.  Bail out.
         */
        printk("%s: tx queue full!.\n", dev->name);
        return 1;
    }

    lenSkbDataByts = skb->len;

    /* Fill in a Tx ring entry
     */
    bdp = priv->tx_bd_base + priv->tx_next;

    /* Clear all of the status flags.
     */
    bdp->len_status &= ~OETH_TX_BD_STATS;

    /* If the frame is short, tell CPM to pad it.
     */
    if (lenSkbDataByts <= ETH_ZLEN)
        bdp->len_status |= OETH_TX_BD_PAD;
    else
        bdp->len_status &= ~OETH_TX_BD_PAD;

#if OETH_DEBUG
    printk("TX\n");
    oeth_print_packet((unsigned long)skb->data, lenSkbDataByts);
#endif  // OETH_DEBUG

    /* Copy data in preallocated buffer */
    if (lenSkbDataByts > OETH_TX_BUFF_SIZE)
      {
        printk("%s: %d byte tx frame too long (max:%d)!.\n",
               dev->name,
               lenSkbDataByts,
               OETH_TX_BUFF_SIZE);

        dev_kfree_skb(skb);                     //;dgt
        netif_wake_queue(dev);
        return 0;                               //;dgt
      }
      else
      {
#ifdef CONFIG_NIOS2
          if((((unsigned long) (skb->data)) & 1) == 0)
            {
              unsigned short     *dst;
              unsigned short     *src;

              dst = ((unsigned short *) (bdp->addr));
              src = ((unsigned short *) (skb->data));

              memcpy(phys_to_virt(dst), src, lenSkbDataByts);
            }
            else
            {
              memcpy(phys_to_virt((unsigned char *)bdp->addr),
                     skb->data,
                     lenSkbDataByts);
            }

#else
          memcpy(phys_to_virt(bdp->addr), skb->data, lenSkbDataByts);
#endif  // CONFIG_NIOS2
      }

    bdp->len_status =   (bdp->len_status & 0x0000ffff)
                      | (lenSkbDataByts << 16);

    dev_kfree_skb(skb);

    /* Send it on its way.  Tell controller its ready, interrupt when done,
     * and to put the CRC on the end.
     */

    local_bh_disable();

    priv->tx_next = (priv->tx_next + 1) & OETH_TXBD_NUM_MASK;     //;dgt

    if (priv->tx_next == priv->tx_last)
        priv->tx_full = 1;

    bdp->len_status |= (  0
                        | OETH_TX_BD_READY
                        | OETH_TX_BD_IRQ
                        | OETH_TX_BD_CRC
                       );

    dev->trans_start = jiffies;

    if (priv->tx_next != priv->tx_last)
    {
        if(((priv->tx_next + 1) & OETH_TXBD_NUM_MASK) != priv->tx_last)
            netif_wake_queue(dev);
    }
    local_bh_enable();

    return 0;
}

static struct net_device_stats *oeth_get_stats(struct net_device *dev)
{
    struct oeth_private *priv = (struct oeth_private *)netdev_priv(dev);

    return &priv->stats;
}

static void oeth_set_multicast_list(struct net_device *dev)
{
    volatile oeth_regs *regs = (oeth_regs *)dev->base_addr;

    if (dev->flags & IFF_PROMISC) {

        /* Log any net taps.
         */
        printk("%s: Promiscuous mode enabled.\n", dev->name);
        regs->moder |= OETH_MODER_PRO;
    } else {

        regs->moder &= ~OETH_MODER_PRO;

        if (dev->flags & IFF_ALLMULTI) {

            /* Catch all multicast addresses, so set the
             * filter to all 1's.
             */
            regs->hash_addr0 = 0xffffffff;
            regs->hash_addr1 = 0xffffffff;
        }
        else if (netdev_mc_count(dev)) {

          #if 1                                         //;dgt
            // FIXME...for now, until broken            //;dgt
            //  calc_crc(...) fixed...                  //;dgt
            regs->hash_addr0 = 0xffffffff;              //;dgt
            regs->hash_addr1 = 0xffffffff;              //;dgt
          #else                                         //;dgt
            struct  dev_mc_list *dmi;
            int                  i;

            /* Clear filter and add the addresses in the list.
             */
            regs->hash_addr0 = 0x00000000;
            regs->hash_addr1 = 0x00000000;              //;dgt

            dmi = dev->mc_list;

            for (i = 0; i < dev->mc_count; i++) {

                int hash_b;

                /* Only support group multicast for now.
                 */
                if (!(dmi->dmi_addr[0] & 1))
                    continue;

                hash_b = calc_crc(dmi->dmi_addr);
                if(hash_b >= 32)
                    regs->hash_addr1 |= 1 << (hash_b - 32);
                else
                    regs->hash_addr0 |= 1 << hash_b;
            }
          #endif                                        //;dgt
        }
    }
}

static void oeth_store_mac_addr(struct net_device *dev)
{
    volatile oeth_regs *regs = (oeth_regs *)dev->base_addr;

    regs->mac_addr1 = (dev->dev_addr[0]) <<  8 |
                      (dev->dev_addr[1]);
    regs->mac_addr0 = (dev->dev_addr[2]) << 24 |
                      (dev->dev_addr[3]) << 16 |
                      (dev->dev_addr[4]) <<  8 |
                      (dev->dev_addr[5]);
}

static int oeth_set_mac_addr(struct net_device *dev, void *p)
{
    struct sockaddr* addr = (struct sockaddr*)p;

    memcpy(dev->dev_addr, addr->sa_data, dev->addr_len);
    oeth_store_mac_addr(dev);

    return 0;
}

static const struct net_device_ops oeth_netdev_ops = {
	.ndo_open               = oeth_open,
	.ndo_stop               = oeth_close,
	.ndo_do_ioctl           = oeth_ioctl,
	.ndo_set_mac_address    = oeth_set_mac_addr,
	.ndo_set_multicast_list = oeth_set_multicast_list,
	.ndo_get_stats          = oeth_get_stats,
	.ndo_start_xmit         = oeth_start_xmit,
};

static void oeth_mac_init(struct oeth_private *priv)
{
    volatile oeth_regs* regs = priv->regs;
    
    /* Reset the controller.
     */
    regs->moder  =  OETH_MODER_RST;     /* Reset ON */
    regs->moder &= ~OETH_MODER_RST;     /* Reset OFF */

    /* Setting TXBD base to OETH_TXBD_NUM.
     */
    regs->tx_bd_num = OETH_TXBD_NUM;

    /* Initialize TXBD pointer
     */
    priv->tx_bd_base = (oeth_bd *)(priv->iobase + OETH_BD_OFS);

    /* Initialize RXBD pointer
     */
    priv->rx_bd_base = ((oeth_bd *)(priv->iobase + OETH_BD_OFS)) + OETH_TXBD_NUM;

    /* Set min/max packet length
     */
    regs->packet_len = 0x00400600;

    /* Set IPGR1 register to recomended value
     */
    regs->ipgr1 = 0x0000000c;

    /* Set IPGR2 register to recomended value
     */
    regs->ipgr2 = 0x00000012;

    /* Set COLLCONF register to recomended value
     */
    regs->collconf = 0x000f003f;

    /* Set control module mode
     */
    regs->ctrlmoder = OETH_CTRLMODER_TXFLOW | OETH_CTRLMODER_RXFLOW;

	/* Set MAC Address */
	oeth_store_mac_addr(priv->netdev);
}

static int oeth_probe(struct platform_device *pdev)
{
	struct net_device *netdev = NULL;
	struct resource *res = NULL;
	struct resource *mmio = NULL;
	struct oeth_private *priv = NULL;
	unsigned long 	base_addr;
	unsigned int phy;
	int ret = 0;

	/* allocate networking device */
	netdev = alloc_etherdev(sizeof(struct oeth_private));
	if (!netdev) {
		dev_err(&pdev->dev, "cannot allocate network device\n");
		ret = -ENOMEM;
		goto out;
	}

	SET_NETDEV_DEV(netdev, &pdev->dev);
	platform_set_drvdata(pdev, netdev);


	/* obtain I/O memory space */
	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res) {
		dev_err(&pdev->dev, "cannot obtain I/O memory space\n");
		ret = -ENXIO;
		goto free;
	}
	mmio = devm_request_mem_region(&pdev->dev, res->start,
			res->end - res->start + 1, res->name);
	if (!mmio) {
		dev_err(&pdev->dev, "cannot request I/O memory space\n");
		ret = -ENXIO;
		goto free;
	}

	base_addr = mmio->start;

	/* obtain device IRQ number */
	res = platform_get_resource(pdev, IORESOURCE_IRQ, 0);
	if (!res) {
		dev_err(&pdev->dev, "cannot obtain IRQ\n");
		ret = -ENXIO;
		goto free;
	}

	netdev->irq = res->start;

	/* setup driver-private data */
	priv = netdev_priv(netdev);
	priv->netdev = netdev;
	priv->dma_alloc = 0;

	priv->iobase = devm_ioremap_nocache(&pdev->dev, base_addr,
			mmio->end - mmio->start + 1);
	if (!priv->iobase) {
		dev_err(&pdev->dev, "cannot remap I/O memory space\n");
		ret = -ENXIO;
		goto error;
	}

	/* Allocate buffer memory */
	priv->membase = dma_alloc_coherent(NULL,
		buffer_size, (void *)&netdev->mem_start,
		GFP_KERNEL);
	if (!priv->membase) {
		dev_err(&pdev->dev, "cannot allocate %dB buffer\n",
			buffer_size);
		ret = -ENOMEM;
		goto error;
	}
	netdev->mem_end = netdev->mem_start + buffer_size;
	priv->dma_alloc = buffer_size;

	/* Allow the platform setup code to pass in a MAC address. */
	if (pdev->dev.platform_data) {
		struct oeth_platform_data *pdata =
			(struct oeth_platform_data *)pdev->dev.platform_data;
		memcpy(netdev->dev_addr, pdata->hwaddr, IFHWADDRLEN);
		priv->phy_id = pdata->phy_id;
	}

#if 0
	/* Check that the given MAC address is valid. If it isn't, read the
	 * current MAC from the controller. */
	if (!is_valid_ether_addr(netdev->dev_addr))
		oeth_get_mac_address(netdev, netdev->dev_addr);

	/* Check the MAC again for validity, if it still isn't choose and
	 * program a random one. */
	if (!is_valid_ether_addr(netdev->dev_addr))
		random_ether_addr(netdev->dev_addr);

	oeth_set_mac_address(netdev, netdev->dev_addr);
#endif

	priv->regs = (oeth_regs*)priv->iobase;
	netdev->base_addr = (unsigned long)priv->iobase;
	oeth_mac_init(priv);

	/* register MII bus */
	priv->mdio = mdiobus_alloc();
	if (!priv->mdio) {
		ret = -ENOMEM;
		goto free;
	}

	priv->mdio->name = "oeth-mdio";
	snprintf(priv->mdio->id, MII_BUS_ID_SIZE, "%s-%d",
			priv->mdio->name, pdev->id);
	priv->mdio->read = oeth_mdio_read;
	priv->mdio->write = oeth_mdio_write;
	priv->mdio->reset = oeth_mdio_reset;
	priv->mdio->priv = priv;

	priv->mdio->irq = kmalloc(sizeof(int) * PHY_MAX_ADDR, GFP_KERNEL);
	if (!priv->mdio->irq) {
		ret = -ENOMEM;
		goto free_mdio;
	}

	for (phy = 0; phy < PHY_MAX_ADDR; phy++)
		priv->mdio->irq[phy] = PHY_POLL;

	ret = mdiobus_register(priv->mdio);
	if (ret) {
		dev_err(&netdev->dev, "failed to register MDIO bus\n");
		goto free_mdio;
	}

	ret = oeth_mdio_probe(netdev);
	if (ret) {
		dev_err(&netdev->dev, "failed to probe MDIO bus\n");
		goto error;
	}

    /* Fill in the fields of the device structure with ethernet values.
     */
    ether_setup(netdev);

    /* The Open Ethernet specific entries in the device structure.
     */
    netdev->netdev_ops = &oeth_netdev_ops;


#ifdef CONFIG_NIOS2
    printk("oeth_probe: %d RX, %d TX buffs\n", OETH_RXBD_NUM, OETH_TXBD_NUM);
#endif

	ret = register_netdev(netdev);
	if (ret < 0) {
		dev_err(&netdev->dev, "failed to register interface\n");
		goto error;
	}

	goto out;

error:
	mdiobus_unregister(priv->mdio);
free_mdio:
	kfree(priv->mdio->irq);
	mdiobus_free(priv->mdio);

free:
	if (priv->dma_alloc)
		dma_free_coherent(NULL, priv->dma_alloc, priv->membase,
			netdev->mem_start);
	free_netdev(netdev);
out:
	return ret;
}

/**
 * oeth_remove() - shutdown OpenCores ethernet MAC
 * @pdev:	platform device
 */
static int oeth_remove(struct platform_device *pdev)
{
	struct net_device *netdev = platform_get_drvdata(pdev);
    struct oeth_private* priv = (struct oeth_private *)netdev_priv(netdev);

	platform_set_drvdata(pdev, NULL);

	if (netdev) {
		phy_disconnect(priv->phy);
		priv->phy = NULL;

		if (priv->mdio) {
			mdiobus_unregister(priv->mdio);
			kfree(priv->mdio->irq);
			mdiobus_free(priv->mdio);
		}
		if (priv->dma_alloc)
			dma_free_coherent(NULL, priv->dma_alloc, priv->membase,
				netdev->mem_start);
		unregister_netdev(netdev);
		free_netdev(netdev);
	}

	return 0;
}

#ifdef CONFIG_PM
static int oeth_suspend(struct platform_device *pdev, pm_message_t state)
{
	return -ENOSYS;
}

static int oeth_resume(struct platform_device *pdev)
{
	return -ENOSYS;
}
#else
# define oeth_suspend NULL
# define oeth_resume  NULL
#endif

static struct platform_driver oeth_driver = {
	.probe   = oeth_probe,
	.remove  = oeth_remove,
	.suspend = oeth_suspend,
	.resume  = oeth_resume,
	.driver  = {
		.name = "oeth",
	},
};

static int __init oeth_init(void)
{
	return platform_driver_register(&oeth_driver);
}

static void __exit oeth_exit(void)
{
	platform_driver_unregister(&oeth_driver);
}

module_init(oeth_init);
module_exit(oeth_exit);

MODULE_AUTHOR("Simon Srot (simons@opencores.org)");
MODULE_DESCRIPTION("OpenCore Ethernet MAC driver");
MODULE_LICENSE("GPL v2");
