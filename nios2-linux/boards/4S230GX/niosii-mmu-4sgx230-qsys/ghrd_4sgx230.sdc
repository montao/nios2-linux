## External clocks
create_clock -period 20.00 -name clkin_50  [ get_ports clkin_50  ]
create_clock -period 8.00 -name clkin_125 [ get_ports clkin_125 ]

## Derive all pll clocks
derive_pll_clocks
derive_clock_uncertainty

set Top_clkin_50                clkin_50
set Top_clkin_100               clkin_100
set Top_clkin_125               clkin_125

## HPDDR3 generated clocks
set HalfRate_Clock					*|ddr3_top|qsys_ghrd_4sgx230_ddr3_top_p0_pll_afi_half_clk
#set HalfRate_Clock              *|clk|half_rate.pll|altpll_component|auto_generated|*|clk[0]
#set FullRate_Clock              *|clk|half_rate.pll|altpll_component|auto_generated|*|clk[1]
set FullRate_Clock              *|ddr3_top|qsys_ghrd_4sgx230_ddr3_top_p0_pll_afi_clk
set Avl_Clock                   *|ddr3_top|qsys_ghrd_4sgx230_ddr3_top_p0_pll_avl_clock

## Constrain SGMII interface clocks
set TseSgmii_lvds_1250_Clock    *|ALTLVDS_RX_component|auto_generated|*|clk[0]
set TseSgmii_lvds_125_Clock     *|ALTLVDS_RX_component|auto_generated|*|clk[1]
set TseSgmii_lvds_rx_Clock      *|ALTLVDS_RX_component|auto_generated|rx[0]|clk0


set_clock_groups \
    -exclusive \
    -group [get_clocks $Top_clkin_50 ] \
    -group [get_clocks $Top_clkin_100 ] \
    -group [get_clocks $Top_clkin_125 ] \
    -group [get_clocks $HalfRate_Clock ] \
    -group [get_clocks $FullRate_Clock ] \
	 -group [get_clocks $Avl_Clock ] \
    -group [get_clocks $TseSgmii_lvds_1250_Clock ] \
    -group [get_clocks $TseSgmii_lvds_125_Clock ] \
    -group [get_clocks $TseSgmii_lvds_rx_Clock ]

