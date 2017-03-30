//
//  Nios2 Golden Hardware Reference Design top-level verilog module for 4SGX230 design variant.
//  
//

module ghrd_4sgx230
(
                 // 1) global signals:
                  clkin_50,
                  clkin_100,
                  clkin_125,
                  reset_n,

                 // the_tse_mac
                  enet_mdc,
                  enet_mdio,
                  enet_rx_p,
                  enet_tx_p,
                  enet_resetn,
  
                 // the_button_pio
                  button_pio,

                 // the_ddr3_top
                  ddr3top_addr,
                  ddr3top_ba,
                  ddr3top_cas_n,
                  ddr3top_cke,
                  ddr3top_clk_n,
                  ddr3top_clk,
                  ddr3top_cs_n,
                  ddr3top_dm,
                  ddr3top_dq,
                  ddr3top_dqs,
                  ddr3top_dqsn,
                  ddr3top_odt,
                  ddr3top_ras_n,
                  ddr3top_reset_n,
                  ddr3top_we_n,
		  //add these two more
                  oct_rdn_to_the_ddr3_top,
                  oct_rup_to_the_ddr3_top,

                 // the_dipsw_pio
                  dipsw_pio,

                 // the_led_pio
                  led_pio,

                 // the_tb_fsm_avalon_slave
                  flash_oen,
                  flash_cen,
                  fsm_a,
                  fsm_d,
                  flash_wen,

                 // the_uart
                  rxd_to_the_uart,
                  txd_from_the_uart
               )
;

//add these two more
  input            oct_rdn_to_the_ddr3_top;
  input            oct_rup_to_the_ddr3_top;

  output           enet_mdc;
  inout            enet_mdio;
  input            enet_rx_p;
  output           enet_tx_p;
  output           enet_resetn;
  output  [ 12: 0] ddr3top_addr;
  output  [  2: 0] ddr3top_ba;
  output           ddr3top_cas_n;
  output           ddr3top_cke;
  inout            ddr3top_clk_n;
  inout            ddr3top_clk;
  output           ddr3top_cs_n;
  output  [  1: 0] ddr3top_dm;
  inout   [ 15: 0] ddr3top_dq;
  inout   [  1: 0] ddr3top_dqs;
  inout   [  1: 0] ddr3top_dqsn;
  output           ddr3top_odt;
  output           ddr3top_ras_n;
  output           ddr3top_reset_n;
  output           ddr3top_we_n;
  output  [ 15: 0] led_pio;
  output           flash_oen;
  output           flash_cen;
  output  [ 25: 0] fsm_a;
  inout   [ 15: 0] fsm_d;
  output           txd_from_the_uart;
  output           flash_wen;
  input            clkin_50;
  input            clkin_100;
  input            clkin_125;
  input   [  2: 0] button_pio;
  input   [  7: 0] dipsw_pio;
  input            reset_n;
  input            rxd_to_the_uart;

//
// Common Flash interface assignments
//
wire    [24: 0] fsm_a_alignment;
wire            select_n_to_the_ext_flash;
wire            select_n_to_the_ext_flash_1;
wire            flash_cen;

assign  fsm_a[24: 0]    =   fsm_a_alignment;
assign  fsm_a[25]       =   ~select_n_to_the_ext_flash_1;
assign  flash_cen       =   select_n_to_the_ext_flash & select_n_to_the_ext_flash_1;

//
// Ethernet interface assignments
//
wire            mdio_in;
wire            mdio_oen;
wire            mdio_out;

assign mdio_in = enet_mdio;
assign enet_mdio = mdio_oen == 0 ? mdio_out : 1'bz;

//
// Declare a localparam for the number of reset sources that exist in this design.
// This parameter will be used by the global_reset_generator module.
//
// Tie the reset sources from the system into the global_reset_generator module.
// The reset counter width of 16 should provide a 2^16 clock assertion of global reset
// which at 50MHz should be 1.31ms long.
//
    localparam RESET_SOURCES_COUNT = 2;

reg [(RESET_SOURCES_COUNT - 1):0]   resetn_sources;
wire            global_resetn;
wire            reset_phy_clk_n_from_the_ddr3_top;
wire            global_reset_n_to_the_ddr3_top;

always @ (*) begin
    resetn_sources[(RESET_SOURCES_COUNT - 1)]   <=  reset_n;
    resetn_sources[(RESET_SOURCES_COUNT - 2)]   <=  reset_phy_clk_n_from_the_ddr3_top;
end

global_reset_generator 
#(
    .RESET_SOURCES_WIDTH  (RESET_SOURCES_COUNT),
    .RESET_COUNTER_WIDTH  (16)
) global_reset_generator_inst
(
    .clk            (clkin_50),
    .resetn_sources (resetn_sources),
    .global_resetn  (global_resetn),
    .pll_resetn     (global_reset_n_to_the_ddr3_top)
);

assign enet_resetn = global_resetn;

  qsys_ghrd_4sgx230 qsys_ghrd_4sgx230_inst
    (
	// 1) global signals:
        .clkin_100                               (clkin_100),
        .reset_n                                 (global_resetn),

        // the_ddr3_top
        //add these two
        .oct_rdn_to_the_ddr3_top                 (oct_rdn_to_the_ddr3_top),
        .oct_rup_to_the_ddr3_top                 (oct_rup_to_the_ddr3_top),
        // .global_reset_n_to_the_ddr3_top         (global_reset_n_to_the_ddr3_top),
        .ddr3_top_global_reset_reset_n           (global_reset_n_to_the_ddr3_top),
        //.mem_addr_from_the_ddr3_top            (ddr3top_addr),
        .mem_a_from_the_ddr3_top                 (ddr3top_addr),
        .mem_ba_from_the_ddr3_top                (ddr3top_ba),
        .mem_cas_n_from_the_ddr3_top             (ddr3top_cas_n),
        .mem_cke_from_the_ddr3_top               (ddr3top_cke),
        //.mem_clk_n_to_and_from_the_ddr3_top    (ddr3top_clk_n),
        .mem_ck_n_from_the_ddr3_top              (ddr3top_clk_n),
        //.mem_clk_to_and_from_the_ddr3_top      (ddr3top_clk),
        .mem_ck_from_the_ddr3_top                (ddr3top_clk),
        .mem_cs_n_from_the_ddr3_top              (ddr3top_cs_n),
        .mem_dm_from_the_ddr3_top                (ddr3top_dm),
        .mem_dq_to_and_from_the_ddr3_top         (ddr3top_dq),
        .mem_dqs_to_and_from_the_ddr3_top        (ddr3top_dqs),
        //.mem_dqsn_to_and_from_the_ddr3_top     (ddr3top_dqsn),
        .mem_dqs_n_to_and_from_the_ddr3_top      (ddr3top_dqsn),
        .mem_odt_from_the_ddr3_top               (ddr3top_odt),
        .mem_ras_n_from_the_ddr3_top             (ddr3top_ras_n),
        .mem_reset_n_from_the_ddr3_top           (ddr3top_reset_n),
        .mem_we_n_from_the_ddr3_top              (ddr3top_we_n),
        //.reset_phy_clk_n_from_the_ddr3_top     (reset_phy_clk_n_from_the_ddr3_top),
        .ddr3_top_afi_reset_reset_n              (reset_phy_clk_n_from_the_ddr3_top),

	// the_button_pio
        .in_port_to_the_button_pio               (button_pio),

        // the_dipsw_pio
        .in_port_to_the_dipsw_pio                (dipsw_pio),

        // the_led_pio
        .out_port_from_the_led_pio               (led_pio),

        // the_tse_mac
/*
        .mdc_from_the_tse_mac_0                  (enet_mdc),
        .mdio_in_to_the_tse_mac_0                (mdio_in),
        .mdio_oen_from_the_tse_mac_0             (mdio_oen),
        .mdio_out_from_the_tse_mac_0             (mdio_out),
        .ref_clk_to_the_tse_mac_0                (clkin_125),
        .rxp_to_the_tse_mac_0                    (enet_rx_p),
        .txp_from_the_tse_mac_0                  (enet_tx_p),
*/
        .tse_mac_0_mdio_out                      (mdio_out),
        .tse_mac_0_mdio_oen                      (mdio_oen),
        .tse_mac_0_mdio_in                       (mdio_in),
        .tse_mac_0_mdc                           (enet_mdc),
//        .tse_mac_0_led_an                        (),
//        .tse_mac_0_led_char_err                  (),
//        .tse_mac_0_led_link                      (),
//        .tse_mac_0_led_disp_err                  (),
//        .tse_mac_0_led_crs                       (),
//        .tse_mac_0_led_col                       (),
        .tse_mac_0_txp                           (enet_tx_p),
        .tse_mac_0_rxp                           (enet_rx_p),
        .tse_mac_0_ref_clk                       (clkin_125),
//        .tse_mac_0_rx_recovclkout                (),


        // the_uart
        .uart_0_rxd                              (rxd_to_the_uart),
        .uart_0_txd                              (txd_from_the_uart),

        // the_tb_fsm_avalon_slave
        .select_n_to_the_ext_flash               (select_n_to_the_ext_flash),
        .select_n_to_the_ext_flash_1             (select_n_to_the_ext_flash_1),
        .tb_fsm_address                          (fsm_a_alignment),
        .tb_fsm_data                             (fsm_d),
        .tb_fsm_readn                            (flash_oen),
        .tb_fsm_writen                           (flash_wen)

    );

endmodule
