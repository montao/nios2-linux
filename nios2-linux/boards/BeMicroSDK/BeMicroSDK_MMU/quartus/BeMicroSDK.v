module BeMicroSDK
(
   // General
   input    wire  CLK_FPGA_50M,
   input    wire  CPU_RST_N,

   // Ethernet I/F National DP83848CVV
   input    wire  ETH_COL,
   input    wire  ETH_CRS,
   output   wire  ETH_RESET_N,
	//RESET: Active Low input that initializes or re-initializes the DP83848C. Asserting this pin low for at least 1 us will force a reset process to occur. All internal registers will re-initialize to their default states as specified for each bit in the Register Block section. All strap options are re-initialized as well.
   output   wire  MDC,
   inout    wire  MDIO,
   input    wire  TX_CLK,
   output   wire  TX_EN,
   output   wire  TXD_0,
   output   wire  TXD_1,
   output   wire  TXD_2,
   output   wire  TXD_3,
   input    wire  RX_CLK,
   input    wire  RX_DV,
   input    wire  RX_ER,
   input    wire  RXD_0,
   input    wire  RXD_1,
   input    wire  RXD_2,
   input    wire  RXD_3,
   
   // Extension connector I/F
   //Banks 1-6 all tied to +3v3   
   //Ordered below by where they are in the bank. May help with figuring SSO in the future.

   
   // Mobile DDR I/F
   output   wire  RAM_A0,
   output   wire  RAM_A1,
   output   wire  RAM_A2,
   output   wire  RAM_A3,
   output   wire  RAM_A4,
   output   wire  RAM_A5,
   output   wire  RAM_A6,
   output   wire  RAM_A7,
   output   wire  RAM_A8,
   output   wire  RAM_A9,
   output   wire  RAM_A10,
   output   wire  RAM_A11,
   output   wire  RAM_A12,
   output   wire  RAM_A13,
   output   wire  RAM_BA0,
   output   wire  RAM_BA1,
   output   wire  RAM_CK_N,
   output   wire  RAM_CK_P,
   output   wire  RAM_CKE,
   output   wire  RAM_CS_N,
   output   wire  RAM_WS_N,
   output   wire  RAM_RAS_N,
   output   wire  RAM_CAS_N,
   inout    wire  RAM_D0,
   inout    wire  RAM_D1,
   inout    wire  RAM_D2,
   inout    wire  RAM_D3,
   inout    wire  RAM_D4,
   inout    wire  RAM_D5,
   inout    wire  RAM_D6,
   inout    wire  RAM_D7,
   inout    wire  RAM_D8,
   inout    wire  RAM_D9,
   inout    wire  RAM_D10,
   inout    wire  RAM_D11,
   inout    wire  RAM_D12,
   inout    wire  RAM_D13,
   inout    wire  RAM_D14,
   inout    wire  RAM_D15,
   output   wire  RAM_LDM,
   output   wire  RAM_UDM,
   inout    wire  RAM_LDQS,
   inout    wire  RAM_UDQS,

   // Micro SD I/F
//   output   wire  SD_CLK,
//   inout    wire  SD_CMD, //DI
//   inout    wire  SD_DAT0, //DO
////   inout    wire  SD_DAT1,
////   inout    wire  SD_DAT2,
//   inout    wire  SD_DAT3, //CS
   output   wire  SD_CLK,
   output    wire  SD_CMD, //DI
   input    wire  SD_DAT0, //DO
   output    wire  SD_DAT3, //CS

	//GPIO
	input    wire  P22,                 //IOBANK1
   input    wire  P23,                 //IOBANK1
   input    wire  P25,                 //IOBANK1
   input    wire  P27,                 //IOBANK1
   input    wire  P44,                 //IOBANK1
   input    wire  P45,                 //IOBANK1

   input    wire  P5,                  //IOBANK2
   input    wire  P7,                  //IOBANK2
   input    wire  P9,                  //IOBANK2
   input    wire  P11,                 //IOBANK2
   input    wire  P12,                 //IOBANK2
   input    wire  P14,                 //IOBANK2
   input    wire  P17,                 //IOBANK2
   input    wire  P18,                 //IOBANK2
   input    wire  P19,                 //IOBANK2
   input    wire  P20,                 //IOBANK2
   input    wire  P47,                 //IOBANK2
   input    wire  P50,                 //IOBANK2
   input    wire  P51,                 //IOBANK2
   
   
   output    wire  RESET_EXP_N,         //IOBANK3 For the use of reseting the expansion board?
   input    wire  P3,                  //IOBANK3
   input    wire  P4,                  //IOBANK3
   input    wire  P6,                  //IOBANK3
   input    wire  P8,                  //IOBANK3
   input    wire  P10,                 //IOBANK3
   input    wire  P16,                 //IOBANK3
   input    wire  P37,                 //IOBANK3
   input    wire  P38,                 //IOBANK3
   input    wire  P39,                 //IOBANK3
   input    wire  P40,                 //IOBANK3
   input    wire  P41,                 //IOBANK3  
   input    wire  P42,                 //IOBANK3
   input    wire  P43,                 //IOBANK3
   input    wire  P49,                 //IOBANK3
   input    wire  P60,                 //IOBANK3

   input    wire  EXP_PRESENT,         //IOBANK4
   input    wire  P13,                 //IOBANK4
   input    wire  P21,                 //IOBANK4
   input    wire  P29,                 //IOBANK4
   input    wire  P52,                 //IOBANK4
   input    wire  P53,                 //IOBANK4
   input    wire  P54,                 //IOBANK4
   input    wire  P55,                 //IOBANK4
  
   input    wire  P15,                 //IOBANK5
   input    wire  P24,                 //IOBANK5
   input    wire  P26,                 //IOBANK5
   input    wire  P28,                 //IOBANK5
   input    wire  P46,                 //IOBANK5
   input    wire  P48,                 //IOBANK5
   input    wire  P56,                 //IOBANK5
   input    wire  P57,                 //IOBANK5
   input    wire  P58,                 //IOBANK5
   input    wire  P59,                 //IOBANK5
   
   input    wire  P1,                  //CLK12,DiffCLK_7n input
   input    wire  P2,                  //CLK13,DiffCLK_7p input
   input    wire  P35,                 //CLK15,DiffCLK_6n input
   input    wire  P36,                 //CLK15,DiffCLK_6p input

	// EPCS Controller
	input		wire	data0_to_the_epcs_flash_controller,
	output	wire	dclk_from_the_epcs_flash_controller,
	output	wire	sce_from_the_epcs_flash_controller,
	output	wire	sdo_from_the_epcs_flash_controller,

   // Temp. Sensor I/F
   output   wire  TEMP_CS_N,
   output   wire  TEMP_SC,       
   output   wire  TEMP_MOSI,
   input    wire  TEMP_MISO,
	
   // Misc
   input    wire  RECONFIG_SW1,
   input    wire  RECONFIG_SW2,
   input    wire  PBSW_N,
   output   wire  F_LED0,
   output   wire  F_LED1,
   output   wire  F_LED2,
   output   wire  F_LED3,
   output   wire  F_LED4,
   output   wire  F_LED5,
   output   wire  F_LED6,
   output   wire  F_LED7

);

//------------------------------------------------------------------------------
// Signal Declarations
//------------------------------------------------------------------------------
wire [3:0]  txd;
wire [3:0]  rxd;
wire [12:0] ram_addr;
wire [1:0]  ram_baddr;
wire [15:0] ram_dq;
wire [3:0]  sd_data;
wire [1:0]  dipsw;
wire [7:0]  led;

wire mdio_oen;
wire mdio_out;

wire wTEMP_MOSI;


//------------------------------------------------------------------------------
// Begin Module
//------------------------------------------------------------------------------

// Define open drain output for TEMP_MOSI
OPNDRN iTEMP_MOSI_OD (.in(wTEMP_MOSI), .out(TEMP_MOSI));


// wire up buses
assign TXD_0 = txd[0];
assign TXD_1 = txd[1];
assign TXD_2 = txd[2];
assign TXD_3 = txd[3];

assign rxd[0] = RXD_0;
assign rxd[1] = RXD_1;
assign rxd[2] = RXD_2;
assign rxd[3] = RXD_3;

wire [32:1] gpio_to_SOPC_1 = { 1'b0, 1'b0, 1'b0, 1'b0,  P60,  P59,  P58,  P57,  
                                P56,  P55,  P54,  P53,  P52,  P51,  P50,  P49,  
                                P48,  P47,  P46,  P45,  P44,  P43,  P42,  P41,  
                                P40,  P39,  P38,  P37,  P36,  P35, 1'b0, 1'b0
                             };
                  
                  
wire [32:1] gpio_to_SOPC_2 = { 1'b0, 1'b0, 1'b0, P29, P28, P27, P26, P25,  
                                P24,  P23,  P22, P21, P20, P19, P18, P17, 
                                P16,  P15,  P14, P13, P12, P11, P10, P9,   
                                 P8,   P7,   P6,  P5,  P4,  P3,  P2, P1 
                             };


assign RAM_A0  = ram_addr[ 0];
assign RAM_A1  = ram_addr[ 1];
assign RAM_A2  = ram_addr[ 2];
assign RAM_A3  = ram_addr[ 3];
assign RAM_A4  = ram_addr[ 4];
assign RAM_A5  = ram_addr[ 5];
assign RAM_A6  = ram_addr[ 6];
assign RAM_A7  = ram_addr[ 7];
assign RAM_A8  = ram_addr[ 8];
assign RAM_A9  = ram_addr[ 9];
assign RAM_A10 = ram_addr[10];
assign RAM_A11 = ram_addr[11];
assign RAM_A12 = ram_addr[12];
//assign RAM_A13 = ram_addr[13]; RR Tuesday Aug 24th
assign RAM_A13 = 1'b0; //A13 on the VFBGA package is an expansion address if you would like to use the next 64Mx16 instead of the 32Mx16

assign RAM_BA0 = ram_baddr[0];
assign RAM_BA1 = ram_baddr[1];

assign dipsw[0] = RECONFIG_SW1;
assign dipsw[1] = RECONFIG_SW2;

assign F_LED0 = led[0];
assign F_LED1 = led[1];
assign F_LED2 = led[2];
assign F_LED3 = led[3];
assign F_LED4 = led[4];
assign F_LED5 = led[5];
assign F_LED6 = led[6];
assign F_LED7 = led[7];




//The DDR PLL
wire CLK_to_the_mobile_ddr_sdram;
wire WRITE_CLK_to_the_mobile_ddr_sdram;
wire CLK_100M;
wire CLK_60M;
wire CLK_30M;

the_M_DDR_PLL the_M_DDR_PLL_inst(
   .areset(~CPU_RST_N),
   .inclk0(CLK_FPGA_50M),
   .c0(CLK_to_the_mobile_ddr_sdram),                                             //set to 167 MHz
   .c1(WRITE_CLK_to_the_mobile_ddr_sdram),                                       //Set to -90 phase shift from CLK_to_the_mobile_ddr_sdram
   .c2(CLK_100M),
	.c3(CLK_60M),
   .c4(CLK_30M)
);



wire reset_n;

reset_logic reset_logic_inst(
   .a_50_MHZ_CLK(CLK_FPGA_50M),                                                  //input
   .RESET_N(reset_n)                                                             //output
);

wire eth_reset_n_int;
wire eth_reset_n_sopc;
assign ETH_RESET_N = eth_reset_n_int & eth_reset_n_sopc;

	eth_reset_gen eth_reset_gen(
		.clk(CLK_FPGA_50M),
		.reset_n(1'b1),
		.reset_n_out(eth_reset_n_int)
	);


//SOPC system
nios2_bemicro_sopc nios2_bemicro_sopc_inst
(
   .ext_clk_100                           (CLK_100M),                         //  input            ext_clk_50;
   .ext_clk_60                           (CLK_60M),                         //  input            ext_clk_50;
   .reset_n                              (reset_n),                              //  input            reset_n;

   .CLK_to_the_mobile_ddr_sdram          (CLK_to_the_mobile_ddr_sdram),          //  input            CLK_to_the_mobile_ddr_sdram;
   .WRITE_CLK_to_the_mobile_ddr_sdram    (WRITE_CLK_to_the_mobile_ddr_sdram),    //  input            WRITE_CLK_to_the_mobile_ddr_sdram;

   .A_from_the_mobile_ddr_sdram          (ram_addr),                             //  output  [ 12: 0] A_from_the_mobile_ddr_sdram;
   .BA_from_the_mobile_ddr_sdram         (ram_baddr),                            //  output  [  1: 0] BA_from_the_mobile_ddr_sdram;
   .CAS_from_the_mobile_ddr_sdram        (RAM_CAS_N),                            //  output           CAS_from_the_mobile_ddr_sdram;
   .CKE_from_the_mobile_ddr_sdram        (RAM_CKE),                              //  output           CKE_from_the_mobile_ddr_sdram;
   .CLK_OUT_N_from_the_mobile_ddr_sdram  (RAM_CK_N),                             //  output           CLK_OUT_N_from_the_mobile_ddr_sdram;
   .CLK_OUT_P_from_the_mobile_ddr_sdram  (RAM_CK_P),                             //  output           CLK_OUT_P_from_the_mobile_ddr_sdram;
   .CS_from_the_mobile_ddr_sdram         (RAM_CS_N),                             //  output           CS_from_the_mobile_ddr_sdram;
   .DQM_from_the_mobile_ddr_sdram        ({RAM_UDM, RAM_LDM}),                   //  output  [  1: 0] DQM_from_the_mobile_ddr_sdram;
   .DQS_to_and_from_the_mobile_ddr_sdram ({RAM_UDQS, RAM_LDQS}),                 //  inout   [  1: 0] DQS_to_and_from_the_mobile_ddr_sdram;
   .DQ_to_and_from_the_mobile_ddr_sdram  ({RAM_D15, RAM_D14, RAM_D13, RAM_D12,
                                           RAM_D11, RAM_D10, RAM_D9,  RAM_D8,
                                           RAM_D7,  RAM_D6,  RAM_D5,  RAM_D4,
                                           RAM_D3,  RAM_D2,  RAM_D1,  RAM_D0}),  //  inout   [ 15: 0] DQ_to_and_from_the_mobile_ddr_sdram;
   .RAS_from_the_mobile_ddr_sdram        (RAM_RAS_N),                            //  output           RAS_from_the_mobile_ddr_sdram;
   .WE_from_the_mobile_ddr_sdram         (RAM_WS_N),                             //  output           WE_from_the_mobile_ddr_sdram;


   .ena_10_from_the_tse_mac              (),                                     //  output           ena_10_from_the_tse_mac;
   .eth_mode_from_the_tse_mac            (),                                     //  output           eth_mode_from_the_tse_mac;
   .gm_rx_d_to_the_tse_mac               (8'b0),                                 //  input   [  7: 0] gm_rx_d_to_the_tse_mac;
   .gm_rx_dv_to_the_tse_mac              (1'b0),                                 //  input            gm_rx_dv_to_the_tse_mac;
   .gm_rx_err_to_the_tse_mac             (1'b0),                                 //  input            gm_rx_err_to_the_tse_mac;
   .gm_tx_d_from_the_tse_mac             (),                                     //  output  [  7: 0] gm_tx_d_from_the_tse_mac;
   .gm_tx_en_from_the_tse_mac            (),                                     //  output           gm_tx_en_from_the_tse_mac;
   .gm_tx_err_from_the_tse_mac           (),                                     //  output           gm_tx_err_from_the_tse_mac;
   .m_rx_col_to_the_tse_mac              (ETH_COL),                              //  input            m_rx_col_to_the_tse_mac;
   .m_rx_crs_to_the_tse_mac              (ETH_CRS),                              //  input            m_rx_crs_to_the_tse_mac;
   .m_rx_d_to_the_tse_mac                (rxd),                                  //  input   [  3: 0] m_rx_d_to_the_tse_mac;
   .m_rx_en_to_the_tse_mac               (RX_DV),                                //  input            m_rx_en_to_the_tse_mac;
   .m_rx_err_to_the_tse_mac              (RX_ER),                                //  input            m_rx_err_to_the_tse_mac;
   .m_tx_d_from_the_tse_mac              (txd),                                  //  output  [  3: 0] m_tx_d_from_the_tse_mac;
   .m_tx_en_from_the_tse_mac             (TX_EN),                                //  output           m_tx_en_from_the_tse_mac;
   .m_tx_err_from_the_tse_mac            (),                                     //  output           m_tx_err_from_the_tse_mac;
   .mdc_from_the_tse_mac                 (MDC),                                  //  output           mdc_from_the_tse_mac;
   .mdio_in_to_the_tse_mac               (MDIO),                                 //  input            mdio_in_to_the_tse_mac;
   .mdio_oen_from_the_tse_mac            (mdio_oen),                             //  output           mdio_oen_from_the_tse_mac;
   .mdio_out_from_the_tse_mac            (mdio_out),                             //  output           mdio_out_from_the_tse_mac;
   .rx_clk_to_the_tse_mac                (RX_CLK),                               //  input            rx_clk_to_the_tse_mac;
   .set_1000_to_the_tse_mac              (1'b0),                                 //  input            set_1000_to_the_tse_mac;
   .set_10_to_the_tse_mac                (1'b0),                                 //  input            set_10_to_the_tse_mac;
   .tx_clk_to_the_tse_mac                (TX_CLK),                               //  input            tx_clk_to_the_tse_mac;


	// For SD card controller
//	.spi_clk_from_the_mmc_spi             (SD_CLK),
//	.spi_cs_n_from_the_mmc_spi            (SD_DAT3),
//	.spi_data_in_to_the_mmc_spi           (SD_DAT0),
//	.spi_data_out_from_the_mmc_spi        (SD_CMD),
	.MISO_to_the_mmc_spi                  (SD_DAT0),
	.MOSI_from_the_mmc_spi                (SD_CMD),
	.SCLK_from_the_mmc_spi                (SD_CLK),
	.SS_n_from_the_mmc_spi                (SD_DAT3),
	

   .in_port_to_the_EC_GPIO_1             (gpio_to_SOPC_1),                       //  input   [ 31: 0] in_port_to_the_EC_GPIO_1;
   .in_port_to_the_EC_GPIO_2             (gpio_to_SOPC_2),                        //  input   [ 31: 0] in_port_to_the_EC_GPIO_2;
   .out_port_from_the_nRESET_EXP         (RESET_EXP_N), //RR Tues, Aug 24th output RESET_EXP_N 
   .in_port_to_the_EXP_PRESENT           (EXP_PRESENT), //RR Tues, Aug 24th input EXP_PRESENT
	.data0_to_the_epcs_flash_controller   (data0_to_the_epcs_flash_controller),
   .dclk_from_the_epcs_flash_controller  (dclk_from_the_epcs_flash_controller),
   .sce_from_the_epcs_flash_controller   (sce_from_the_epcs_flash_controller),
   .sdo_from_the_epcs_flash_controller   (sdo_from_the_epcs_flash_controller),

   .SCLK_from_the_temp_sense_spi         (TEMP_SC),                              //  output           SCLK_from_the_temp_sense_spi;
   .SS_n_from_the_temp_sense_spi         (TEMP_CS_N),                            //  output           SS_n_from_the_temp_sense_spi;
   .MISO_to_the_temp_sense_spi           (TEMP_MISO),                            //  input            MISO_to_the_temp_sense_spi;
   .MOSI_from_the_temp_sense_spi         (wTEMP_MOSI),                           //  output           MOSI_from_the_temp_sense_spi; 
	
   .in_port_to_the_dipsw_pio             (dipsw),                                //  input   [  1: 0] in_port_to_the_dipsw_pio;
   .in_port_to_the_user_pio_pushbtn      (PBSW_N),                               //  input            in_port_to_the_user_pio_pushbtn;

   .out_port_from_the_led_pio            (led),                                  //  output  [  7: 0] out_port_from_the_led_pio;
   .out_port_from_the_nENET_reg_reset    (eth_reset_n_sopc) //RR Tues, Aug 24th output ETH_RESET_N 
);


// assign outputs
assign MDIO = (mdio_oen) ? 1'bz : mdio_out;		// modified as per GBI 





endmodule
