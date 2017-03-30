#
# This file is intended to be sourced from a top.sdc
# It contains references to TCL variables that are defined in the top.sdc
#

post_message -type info "Reading file: \'rgmii_clock.sdc\'"

# Set the periods of the clocks used in the design
set PERIOD_125 8.0
set PERIOD_25 [expr 8.0 * 5]
set PERIOD_2p5 [expr 8.0 * 50]

# Create the erx_clk on the port
create_clock \
    -name enet_rx_clk \
    -period $PERIOD_125 \
    -waveform "[expr $PERIOD_125/4.0] [expr ($PERIOD_125/2.0) + ($PERIOD_125/4.0)]" \
    [get_ports {enet_rx_clk}]

# Create the external virtual PHY clock
create_clock \
    -name virtual_phy_clk \
    -period $PERIOD_125

# Define the clocks that can appear on the output of the TX clock mux
# Create the 125MHz mux output
create_generated_clock \
    -name tx_clk_125 \
    -source [get_pins "$enet_pll_125"] \
    [get_pins {tx_clk_to_the_tse_mac~clkctrl|outclk}]

# Create the 25MHz mux output
create_generated_clock \
    -name tx_clk_25 \
    -source [get_pins "$enet_pll_25"] \
    -add \
    [get_pins {tx_clk_to_the_tse_mac~clkctrl|outclk}]

# Create the 2.5MHz mux output
create_generated_clock \
    -name tx_clk_2p5 \
    -source [get_pins "$enet_pll_2p5"] \
    -add \
    [get_pins {tx_clk_to_the_tse_mac~clkctrl|outclk}]

# All of the mux outputs are exclusive
set_clock_groups \
    -exclusive \
    -group [get_clocks {tx_clk_125}] \
    -group [get_clocks {tx_clk_25}] \
    -group [get_clocks {tx_clk_2p5}]
    
# Create the 125Mhz etx_clk output
create_generated_clock \
    -name enet_gtx_clk_125 \
    -source [get_pins {tx_clk_to_the_tse_mac~clkctrl|outclk}] \
    -master_clock {tx_clk_125} \
    [get_ports {enet_gtx_clk}]

# Create the 25Mhz etx_clk output
create_generated_clock \
    -name enet_gtx_clk_25 \
    -source [get_pins {tx_clk_to_the_tse_mac~clkctrl|outclk}] \
    -master_clock {tx_clk_25} \
    -add \
    [get_ports {enet_gtx_clk}]
    
# Create the 2p5Mhz etx_clk output
create_generated_clock \
    -name enet_gtx_clk_2p5 \
    -source [get_pins {tx_clk_to_the_tse_mac~clkctrl|outclk}] \
    -master_clock {tx_clk_2p5} \
    -add \
    [get_ports {enet_gtx_clk}]

# Cut all the unrelated clock transfers to the external clocks
set_clock_groups -exclusive \
    -group [get_clocks {tx_clk_125}] \
    -group [get_clocks {enet_gtx_clk_25 enet_gtx_clk_2p5}]
    
set_clock_groups -exclusive \
    -group [get_clocks {tx_clk_25}] \
    -group [get_clocks {enet_gtx_clk_125 enet_gtx_clk_2p5}]
    
set_clock_groups -exclusive \
    -group [get_clocks {tx_clk_2p5}] \
    -group [get_clocks {enet_gtx_clk_125 enet_gtx_clk_25}]

# This path does not need to be constrained
set_false_path -to [get_ports {enet_gtx_clk}]
