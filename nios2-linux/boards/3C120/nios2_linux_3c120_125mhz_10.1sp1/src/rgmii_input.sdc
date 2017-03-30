#
# This file is intended to be sourced from a top.sdc
#

post_message -type info "Reading file: \'rgmii_input.sdc\'"

#**************************************************************
# Input Delay Constraints (Center aligned, Same Edge Analysis)
#**************************************************************
set Tco_max 0.350
set Tco_min -0.350
set Td_max 1.0
set Td_min 0.9
set longest_src_clk 0.0
set shortest_src_clk 0.0
set longest_dest_clk 1.0
set shortest_dest_clk 0.9

set Tcs_largest [expr $shortest_dest_clk - $longest_src_clk]
set Tcs_smallest [expr $longest_dest_clk - $shortest_src_clk]

set IMD [expr $Td_max + $Tco_max - $Tcs_largest]
post_message -type info "Input Max Delay = $IMD"
set ImD [expr $Td_min + $Tco_min - $Tcs_smallest]
post_message -type info "Input Min Delay = $ImD"

# Constraint the path to the rising edge of the phy clock
set_input_delay -add_delay -clock virtual_phy_clk -max $IMD [get_ports {enet_rx_dv enet_rxd*}]
set_input_delay -add_delay -clock virtual_phy_clk -min $ImD [get_ports {enet_rx_dv enet_rxd*}]

# Constraint the path to the falling edge of the phy clock
set_input_delay -add_delay -clock virtual_phy_clk -max -clock_fall $IMD [get_ports {enet_rx_dv enet_rxd*}]
set_input_delay -add_delay -clock virtual_phy_clk -min -clock_fall $ImD [get_ports {enet_rx_dv enet_rxd*}]

# On a same edge capture DDR interface the paths from RISE to FALL and
# from FALL to RISE on not valid for setup analysis
set_false_path \
    -setup \
    -rise_from [get_clocks {virtual_phy_clk}] \
    -fall_to [get_clocks {enet_rx_clk}]

set_false_path \
    -setup \
    -fall_from [get_clocks {virtual_phy_clk}] \
    -rise_to [get_clocks {enet_rx_clk}]

# On a same edge capture DDR interface the paths from RISE to RISE and
# FALL to FALL are not avlid for hold analysis
set_false_path \
    -hold \
    -rise_from [get_clocks {virtual_phy_clk}] \
    -rise_to [get_clocks {enet_rx_clk}]

set_false_path \
    -hold \
    -fall_from [get_clocks {virtual_phy_clk}] \
    -fall_to [get_clocks {enet_rx_clk}]
