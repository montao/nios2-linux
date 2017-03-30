/*
 *    name:	twilcd_userspace_test.c
 *    Author:       Michael Hennerich
 *    Maintained by:       Aaron Wu
 *    mail:         hennerich@blackfin.uclinux.org
 *    Description:  TWI LCD user space test code
 *
 *   Copyright (C) 2005 Michael Hennerich
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *
 ****************************************************************************
 * MODIFICATION HISTORY:
 ***************************************************************************/
#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <linux/ioctl.h>
#include <errno.h>
#include <unistd.h>
#include <getopt.h>
#include <strings.h>
#include "linux/i2c-dev.h"
#include "twi_lcd.h"

#define VERSION         "0.1"

#define TIME_MULTI_DBG 2

#define T_INIT1    5 * TIME_MULTI_DBG		/* (ms) first init sequence:  4.1 msec */
#define T_INIT2  150 * TIME_MULTI_DBG		/* second init sequence: 100 usec */
#define T_EXEC    80 * TIME_MULTI_DBG		/* normal execution time */
#define T_WRCG   120 * TIME_MULTI_DBG		/* CG RAM Write */
#define T_CLEAR    3 * TIME_MULTI_DBG		/* (ms) Clear Display */

#define SIGNAL_RW		0x20
#define SIGNAL_RS		0x10
#define SIGNAL_ENABLE 	0x40
#define SIGNAL_ENABLE2	0x80

/* LCD Driver function headers and globals */

static int currController = 0x2;

#define kLCD_Addr       0x80

#define SLAVE_ADDR       0x22


#define BusyCheck()	do { } while (0)

/*
 * Function command codes for io_ctl.
 */
#define LCD_On			1
#define LCD_Off			2
#define LCD_Clear		3
#define LCD_Reset		4
#define LCD_Cursor_Left		5
#define LCD_Cursor_Right	6
#define LCD_Disp_Left		7
#define LCD_Disp_Right		8
#define LCD_Set_Cursor		10
#define LCD_Home		11
#define LCD_Curr_Controller	12
#define LCD_Cursor_Off		14
#define LCD_Cursor_On		15
#define LCD_Set_Cursor_Pos	17
#define LCD_Blink_Off           18
#define LCD_Contr           19

#define CONTROLLER_1	0x1
#define CONTROLLER_2	0x2
#define CONTROLLER_BOTH	0x3

static unsigned char *port_id;

static int twi_fd;

static void drv_hd_i2c_nibble(unsigned char controller, unsigned char nibble)
{
	unsigned char enable;
	unsigned char command;	/* this is actually the first data byte on the PCF8574 */
	unsigned char data_block[2];
	/* enable signal: 'controller' is a bitmask */
	/* bit n .. send to controller #n */
	/* so we can send a byte to more controllers at the same time! */
	enable = 0;
	if (controller & 0x01)
		enable |= SIGNAL_ENABLE;
	if (controller & 0x02)
		enable |= SIGNAL_ENABLE2;

	command = nibble;
	data_block[0] = nibble | enable;
	data_block[1] = nibble;

	i2c_smbus_write_block_data(twi_fd, command, 2, data_block);
}

static void drv_hd_i2c_byte(const unsigned char controller,
			    const unsigned char data)
{
	/* send data with RS enabled */
	drv_hd_i2c_nibble(controller, ((data >> 4) & 0x0f) | SIGNAL_RS);
	drv_hd_i2c_nibble(controller, (data & 0x0f) | SIGNAL_RS);
	usleep(T_INIT2);
}

static void drv_hd_i2c_command(const unsigned char controller,
			       const unsigned char cmd)
{
	/* send data with RS disabled */
	drv_hd_i2c_nibble(controller, ((cmd >> 4) & 0x0f));
	drv_hd_i2c_nibble(controller, ((cmd) & 0x0f));
	usleep(T_INIT2);
}

static void drv_hd_i2c_data(const unsigned char controller, const char *string,
			    const int len)
{
	int l = len;

	/* sanity check */
	if (len <= 0)
		return;

	while (l--) {
		if (*string)
			drv_hd_i2c_byte(controller, *(string++));
	}
}

static int drv_hd_i2c_load(void)
{
	/* initialize display */
	drv_hd_i2c_nibble(CONTROLLER_BOTH, 0x03);
	usleep(T_INIT1 * 1000);	/* 4 Bit mode, wait 4.1 ms */
	drv_hd_i2c_nibble(CONTROLLER_BOTH, 0x03);
	usleep(T_INIT2);	/* 4 Bit mode, wait 100 us */
	drv_hd_i2c_nibble(CONTROLLER_BOTH, 0x03);
	usleep(T_INIT2);	/* 4 Bit mode, wait 4.1 ms */
	drv_hd_i2c_nibble(CONTROLLER_BOTH, 0x02);
	usleep(T_INIT2);	/* 4 Bit mode, wait 100 us */
	drv_hd_i2c_command(CONTROLLER_BOTH, 0x28);	/* 4 Bit mode, 1/16 duty cycle, 5x8 font */

	/* Set defaults:
	   Low 0xC0 Display On
	   Low 0x06 Cursor increment, no shift
	   Low 0x80 Display Address 0
	   Low 0x02 Cursor Home */

	usleep(T_INIT2);
	drv_hd_i2c_command(CONTROLLER_BOTH, 0x0f);
	usleep(T_INIT2);
	drv_hd_i2c_command(CONTROLLER_BOTH, 0x06);
	usleep(T_INIT2);
	drv_hd_i2c_command(CONTROLLER_BOTH, 0x80);
	usleep(T_INIT2);
	drv_hd_i2c_command(CONTROLLER_BOTH, 0x02);
	usleep(T_INIT2);
	drv_hd_i2c_command(CONTROLLER_BOTH, 0x0C);
	usleep(T_INIT2);
	drv_hd_i2c_command(CONTROLLER_BOTH, 0x01);
	usleep(T_INIT2);

	return 0;
}

static ssize_t lcd_write(const char *buf, size_t count)
{
	drv_hd_i2c_data(currController, buf, count);

	return count;
}

void
usage (FILE * fp, int rc)
{
  fprintf (fp,
	   "Usage: twilcd_test [-h?v] [-c] [-d CONTROLLER] [-p POSITION] [Message String]\n");
  fprintf (fp, "        -h?            this help\n");
  fprintf (fp, "        -v             print version info\n");
  fprintf (fp, "        -c             Clear Display\n");
  fprintf (fp, "        -d Number      use 1,2,3 for CONTROLLER 1,2,BOTH\n");
  fprintf (fp, "        -n Number      select I2C 0 and 1, use 0 for 537, 1 for 548/527\n");
  fprintf (fp, "        -p Char POS    Position where to put the string \n");
  fprintf (fp, "\nExample: twilcd_test -p 0 \"Hello World !\"\n");
  exit (rc);
}

int main (int argc, char *argv[])
{
  int err = 0;
  int c = 0;
  int clear = 0;
  int setpos = 0;
  int pos = 0;
  int contr = 0;
  int cont_num = 0;
  int id = 0;
  int i2c_port = 0;
  char *string;

  printf (" TWI LCD Test Application\n\n");

  /* Check the passed arg */

  while ((c = getopt (argc, argv, "vch?d:p:n:")) > 0)
    {
      switch (c)
	{
	case 'v':
	  printf ("%s: version %s\n", argv[0], VERSION);
	  exit (0);
	case 'c':
	  clear++;
	  break;
	case 'd':
	  contr++;
	  cont_num = atoi (optarg);
	  break;
	case 'p':
	  setpos++;
	  pos = atoi (optarg);
	  break;
	case 'n':
	  id++;
	  i2c_port = atoi (optarg);
	  break;
	case 'h':
	case '?':
	  usage (stdout, 0);
	  break;
	default:
	  fprintf (stderr, "ERROR: unkown option '%c'\n", c);
	  usage (stderr, 1);
	  break;
	}
    }

  if(i2c_port == 0)
  	port_id = "/dev/i2c-0";
  if(i2c_port == 1)
	port_id = "/dev/i2c-1";

  printf("port id = %s\n", port_id); 
  twi_fd = open (port_id, O_RDWR);
  if (twi_fd < 0){
	printf ("Can't open /dev/i2c-%d\n", cont_num);
	return -1;
  }

  err = ioctl(twi_fd, I2C_SLAVE, SLAVE_ADDR);
  if (err < 0) {
	printf ("errno = %d\n", err);
    exit(1);
  }

  drv_hd_i2c_load();

  if (contr){
		currController = cont_num;
  }

  if (clear){
	usleep(T_EXEC);
	BusyCheck();
	drv_hd_i2c_command(currController, 0x01);
	sleep (1);		/* Clearing take tong */
	printf (" Clearing Display\t: \n");
  }

  if (setpos){
	usleep(T_EXEC);
	BusyCheck();
	drv_hd_i2c_command(currController, pos | kLCD_Addr);
	printf (" Position\t\t: %d \n", pos);
  }

  string = argv[optind];

  if (string){
	printf (" Message\t\t: %s \n", string);
	lcd_write (string, strlen (string));
  }
  close (twi_fd);

  exit (0);
}
