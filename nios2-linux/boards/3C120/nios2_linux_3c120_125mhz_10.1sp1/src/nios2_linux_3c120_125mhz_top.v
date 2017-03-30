module nios2_linux_3c120_125mhz_top
(
   // clocks and resets
   input                clkin_50,
   input                clkin_125,
   
   input                cpu_resetn,
   
   // ddr2 i/f
   inout                ddr2bot_clk_n,
   inout                ddr2bot_clk,
   output   [  3 : 0 ]  ddr2bot_dm,
   inout    [ 31 : 0 ]  ddr2bot_dq,
   inout    [  3 : 0 ]  ddr2bot_dqs,
   
   output   [ 12 : 0 ]  ddr2bot_addr,
   output               ddr2bot_active,
   output   [  1 : 0 ]  ddr2bot_ba,
   output               ddr2bot_cas_n,
   output               ddr2bot_cke,
   output               ddr2bot_cs_n,
   output               ddr2bot_odt,
   output               ddr2bot_ras_n,
   output               ddr2bot_we_n,
   
   // ethernet i/f
   input                enet_led_link1000,
   output               enet_mdc,
   inout                enet_mdio,
   output               enet_resetn,
   input                enet_rx_clk,
   input                enet_rx_dv,
   input    [  3 : 0 ]  enet_rxd,
   output               enet_gtx_clk,
   output               enet_tx_en,
   output   [  3 : 0 ]  enet_txd,
   
   // flash i/f
   output               flash_cen,
   output               flash_oen,
   input                flash_rdybsyn,
   output               flash_resetn,
   output               flash_wen,
   output   [ 24 : 0 ]  fsa,
   inout    [ 31 : 0 ]  fsd,
   
   // user dipsw, led, and push button
   input    [  7 : 0 ]  user_dipsw,
   output   [  7 : 0 ]  user_led,
   input    [  3 : 0 ]  user_pb,
   
   // uart
   input    [ 10 : 10 ] hsmb_tx_d_n,
   output   [ 11 : 11 ] hsmb_tx_d_p
);

//
// Declare a localparam for the number of reset sources that exist in this design.
// This parameter will be used by the global_reset_generator module.
//
localparam RESET_SOURCES_COUNT = 3;

//
// define the wires required for the top level stitching
//

reg [(RESET_SOURCES_COUNT - 1):0]   resetn_sources;

wire              reset_phy_clk_n_from_the_ddr2_lo_latency_128m;
wire              locked_from_the_enet_pll;

wire  [ 25 : 0 ]  address_to_the_cfi_flash_64m;

wire              mdio_oen_from_the_tse_mac;
wire              mdio_out_from_the_tse_mac;
wire              eth_mode_from_the_tse_mac;
wire              ena_10_from_the_tse_mac;
wire              enet_tx_125;
wire              enet_tx_25;
wire              enet_tx_2p5;

wire              tx_clk_to_the_tse_mac;
wire              global_resetn;

//
// Shift the flash byte address from the SOPC system down one bit for the 16-bit flash device.
//
assign fsa = address_to_the_cfi_flash_64m[25:1];
assign flash_resetn = global_resetn;

//
// This assignments should illuminate the ddr active LED.
//
assign ddr2bot_active = 0;

//
// Ethernet interface assignments
//
assign enet_resetn = global_resetn;

enet_gtx_clk_ddio_buffer   enet_gtx_clk_ddio_buffer_inst (
   .aclr       ( !global_resetn ),
   .datain_h   ( 1'b1 ),
   .datain_l   ( 1'b0 ),
   .outclock   ( tx_clk_to_the_tse_mac ),
   .dataout    ( enet_gtx_clk )
);
    
assign tx_clk_to_the_tse_mac =   ( eth_mode_from_the_tse_mac ) ? ( enet_tx_125 ) :  // GbE Mode = 125MHz clock
                                 ( ena_10_from_the_tse_mac ) ? ( enet_tx_2p5 ) :    // 10Mb Mode = 2.5MHz clock
                                 ( enet_tx_25 );                                    // 100Mb Mode = 25MHz clock
                        
assign enet_mdio = ( !mdio_oen_from_the_tse_mac ) ? ( mdio_out_from_the_tse_mac ) : ( 1'bz );

//
// Tie the reset sources from the system into the global_reset_generator module.
// The reset counter width of 8 should provide a 256 clock assertion of global reset
// which at 50MHz should be 5.12us long.
//
always @ ( * ) begin
   resetn_sources[( RESET_SOURCES_COUNT - 1 )] <= cpu_resetn;
   resetn_sources[( RESET_SOURCES_COUNT - 2 )] <= locked_from_the_enet_pll;
   resetn_sources[( RESET_SOURCES_COUNT - 3 )] <= reset_phy_clk_n_from_the_ddr2_lo_latency_128m;
end

global_reset_generator 
#(
   .RESET_SOURCES_WIDTH  ( RESET_SOURCES_COUNT ),
   .RESET_COUNTER_WIDTH  ( 8 )
) global_reset_generator_inst
(
   .clk            ( clkin_50 ),
   .resetn_sources ( resetn_sources ),
   .global_resetn  ( global_resetn )
);

//
// The SOPC system instantiation.
//
nios2_linux_3c120_125mhz_sys_sopc nios2_linux_3c120_125mhz_sys_sopc_inst
(
   // 1) global signals:
   .clkin_125                                          ( clkin_125 ),
   .clkin_50                                           ( clkin_50 ),
   .ddr2bot_lo_latency_128m_aux_full_rate_clk_out      ( ),
   .ddr2bot_lo_latency_128m_aux_half_rate_clk_out      ( ),
   .ddr2bot_lo_latency_128m_phy_clk_out                ( ),
   .enet_pll_c0_out                                    ( enet_tx_125 ),
   .enet_pll_c1_out                                    ( enet_tx_25 ),
   .enet_pll_c2_out                                    ( enet_tx_2p5 ),
   .reset_n                                            ( global_resetn ),
   
   // the_user_dipsw_pio_8in
   .in_port_to_the_user_dipsw_pio_8in                  ( user_dipsw ),
   
   // the_user_led_pio_8out
   .out_port_from_the_user_led_pio_8out                ( user_led ),
   
   // the_user_pb_pio_4in
   .in_port_to_the_user_pb_pio_4in                     ( user_pb ),
   
   // the_cfi_flash_atb_avalon_slave
   .address_to_the_cfi_flash_64m                       ( address_to_the_cfi_flash_64m ),
   .data_to_and_from_the_cfi_flash_64m                 ( fsd ),
   .read_n_to_the_cfi_flash_64m                        ( flash_oen ),
   .select_n_to_the_cfi_flash_64m                      ( flash_cen ),
   .write_n_to_the_cfi_flash_64m                       ( flash_wen ),
   
   // the_ddr2_lo_latency_128m
   .global_reset_n_to_the_ddr2_lo_latency_128m         ( global_resetn ),
   .local_init_done_from_the_ddr2_lo_latency_128m      ( ),
   .local_refresh_ack_from_the_ddr2_lo_latency_128m    ( ),
   .local_wdata_req_from_the_ddr2_lo_latency_128m      ( ),
   .mem_addr_from_the_ddr2_lo_latency_128m             ( ddr2bot_addr ),
   .mem_ba_from_the_ddr2_lo_latency_128m               ( ddr2bot_ba ),
   .mem_cas_n_from_the_ddr2_lo_latency_128m            ( ddr2bot_cas_n ),
   .mem_cke_from_the_ddr2_lo_latency_128m              ( ddr2bot_cke ),
   .mem_clk_n_to_and_from_the_ddr2_lo_latency_128m     ( ddr2bot_clk_n ),
   .mem_clk_to_and_from_the_ddr2_lo_latency_128m       ( ddr2bot_clk ),
   .mem_cs_n_from_the_ddr2_lo_latency_128m             ( ddr2bot_cs_n ),
   .mem_dm_from_the_ddr2_lo_latency_128m               ( ddr2bot_dm ),
   .mem_dq_to_and_from_the_ddr2_lo_latency_128m        ( ddr2bot_dq ),
   .mem_dqs_to_and_from_the_ddr2_lo_latency_128m       ( ddr2bot_dqs ),
   .mem_odt_from_the_ddr2_lo_latency_128m              ( ddr2bot_odt ),
   .mem_ras_n_from_the_ddr2_lo_latency_128m            ( ddr2bot_ras_n ),
   .mem_we_n_from_the_ddr2_lo_latency_128m             ( ddr2bot_we_n ),
   .reset_phy_clk_n_from_the_ddr2_lo_latency_128m      ( reset_phy_clk_n_from_the_ddr2_lo_latency_128m ),    
   
   // the_enet_pll
   .areset_to_the_enet_pll                             ( !global_resetn ),
   .locked_from_the_enet_pll                           ( locked_from_the_enet_pll ),
   
   // the_tse_mac
   .ena_10_from_the_tse_mac                            ( ena_10_from_the_tse_mac ),
   .eth_mode_from_the_tse_mac                          ( eth_mode_from_the_tse_mac ),
   .mdc_from_the_tse_mac                               ( enet_mdc ),
   .mdio_in_to_the_tse_mac                             ( enet_mdio ),
   .mdio_oen_from_the_tse_mac                          ( mdio_oen_from_the_tse_mac ),
   .mdio_out_from_the_tse_mac                          ( mdio_out_from_the_tse_mac ),
   .rgmii_in_to_the_tse_mac                            ( enet_rxd ),
   .rgmii_out_from_the_tse_mac                         ( enet_txd ),
   .rx_clk_to_the_tse_mac                              ( enet_rx_clk ),
   .rx_control_to_the_tse_mac                          ( enet_rx_dv ),
   .set_1000_to_the_tse_mac                            ( ),
   .set_10_to_the_tse_mac                              ( ),
   .tx_clk_to_the_tse_mac                              ( tx_clk_to_the_tse_mac ),
   .tx_control_from_the_tse_mac                        ( enet_tx_en ),
   
   // the_uart
   .rxd_to_the_uart                                    ( hsmb_tx_d_n[10] ),
   .txd_from_the_uart                                  ( hsmb_tx_d_p[11] )
);

endmodule
