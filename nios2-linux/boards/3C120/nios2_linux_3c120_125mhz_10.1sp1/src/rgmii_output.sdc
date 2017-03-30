#
# This file is intended to be sourced from a top.sdc
#

post_message -type info "Reading file: \'rgmii_output.sdc\'"

#**************************************************************
# Output Delay Constraints (Edge Aligned, Same Edge Capture)
#**************************************************************
# Create the output constraints related to the ext_clk withn 125MHz is selected
set Tsu [expr (-1 * $PERIOD_125/4) + 0.35]
set Th [expr ($PERIOD_125/4) + 0.35]
set Td_max 0.0
set Td_min 0.0
set longest_src_clk 0.0
set shortest_src_clk 0.0
set longest_dest_clk 0.0
set shortest_dest_clk 0.0

set Tcs_largest [expr $shortest_dest_clk - $longest_src_clk]
set Tcs_smallest [expr $longest_dest_clk - $shortest_src_clk]

set OMD [expr $Td_max + $Tsu - $Tcs_largest]
set OmD [expr $Td_min - $Th - $Tcs_smallest]
set_output_delay -add_delay -clock [get_clocks enet_gtx_clk_125] -max $OMD [get_ports {enet_tx_en enet_txd*}]
set_output_delay -add_delay -clock [get_clocks enet_gtx_clk_125] -min $OmD [get_ports {enet_tx_en enet_txd*}]
set_output_delay -add_delay -clock [get_clocks enet_gtx_clk_125] -max -clock_fall $OMD [get_ports {enet_tx_en enet_txd*}]
set_output_delay -add_delay -clock [get_clocks enet_gtx_clk_125] -min -clock_fall $OmD [get_ports {enet_tx_en enet_txd*}]

# Create the output constraints related to the ext_clk withn 25MHz is selected
set Tsu [expr (-1 * $PERIOD_25/4) + 0.35]
set Th [expr ($PERIOD_25/4) + 0.35]
set OMD [expr $Td_max + $Tsu - $Tcs_largest]
set OmD [expr $Td_min - $Th - $Tcs_smallest]
set_output_delay -add_delay -clock [get_clocks enet_gtx_clk_25] -max $OMD [get_ports {enet_tx_en enet_txd*}]
set_output_delay -add_delay -clock [get_clocks enet_gtx_clk_25] -min $OmD [get_ports {enet_tx_en enet_txd*}]
set_output_delay -add_delay -clock [get_clocks enet_gtx_clk_25] -max -clock_fall $OMD [get_ports {enet_tx_en enet_txd*}]
set_output_delay -add_delay -clock [get_clocks enet_gtx_clk_25] -min -clock_fall $OmD [get_ports {enet_tx_en enet_txd*}]

# Create the output constraints related to the ext_clk withn 2p5MHz is selected
set Tsu [expr (-1 * $PERIOD_2p5/4) + 0.35]
set Th [expr ($PERIOD_2p5/4) + 0.35]
set OMD [expr $Td_max + $Tsu - $Tcs_largest]
set OmD [expr $Td_min - $Th - $Tcs_smallest]
set_output_delay -add_delay -clock [get_clocks enet_gtx_clk_2p5] -max $OMD [get_ports {enet_tx_en enet_txd*}]
set_output_delay -add_delay -clock [get_clocks enet_gtx_clk_2p5] -min $OmD [get_ports {enet_tx_en enet_txd*}]
set_output_delay -add_delay -clock [get_clocks enet_gtx_clk_2p5] -max -clock_fall $OMD [get_ports {enet_tx_en enet_txd*}]
set_output_delay -add_delay -clock [get_clocks enet_gtx_clk_2p5] -min -clock_fall $OmD [get_ports {enet_tx_en enet_txd*}]


# On same edge capture DDR interface the paths from RISE to FALL and
# from FALL to RISE on not valid for setup analysis
set_false_path \
    -setup \
    -rise_from [get_clocks {tx_clk_125 tx_clk_25 tx_clk_2p5}] \
    -fall_to [get_clocks {enet_gtx_clk_125 enet_gtx_clk_25 enet_gtx_clk_2p5}]

set_false_path \
    -setup \
    -fall_from [get_clocks {tx_clk_125 tx_clk_25 tx_clk_2p5}] \
    -rise_to [get_clocks {enet_gtx_clk_125 enet_gtx_clk_25 enet_gtx_clk_2p5}]

# On same edge capture DDR interface the paths from RISE to RISE and
# FALL to FALL are not valid for hold analysis
set_false_path \
    -hold \
    -rise_from [get_clocks {tx_clk_125 tx_clk_25 tx_clk_2p5}] \
    -rise_to [get_clocks {enet_gtx_clk_125 enet_gtx_clk_25 enet_gtx_clk_2p5}]

set_false_path \
    -hold \
    -fall_from [get_clocks {tx_clk_125 tx_clk_25 tx_clk_2p5}] \
    -fall_to [get_clocks {enet_gtx_clk_125 enet_gtx_clk_25 enet_gtx_clk_2p5}]

set_multicycle_path 0 \
    -from [get_clocks {tx_clk_125 tx_clk_25 tx_clk_2p5}] \
    -to [get_clocks {enet_gtx_clk_125 enet_gtx_clk_25 enet_gtx_clk_2p5}] \
    -setup \
    -end
set_multicycle_path -1 \
    -from [get_clocks {tx_clk_125 tx_clk_25 tx_clk_2p5}] \
    -to [get_clocks {enet_gtx_clk_125 enet_gtx_clk_25 enet_gtx_clk_2p5}] \
    -hold \
    -end
