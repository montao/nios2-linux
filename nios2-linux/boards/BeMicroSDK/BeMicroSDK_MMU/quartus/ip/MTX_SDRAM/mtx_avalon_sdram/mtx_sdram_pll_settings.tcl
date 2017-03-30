###
# Microtronix SDR SDRAM TCL Script v1.0
# Written by Marco Groeneveld
# Build : 169/9.7.2010
###

###
# Variables
###

set name_dq_pin		"prefix_dq"
set name_clk_pin	"prefix_clk_out"
set name_dq_reg		"data_from_padio_reg"
set clk_period		7
set trace_delay		.1
set dq_access		6
set dq_hold		2.5
set margin		0.5

###
# Load Package
###
load_package advanced_timing
load_package report

###
# Open Project
###
project_open [lindex $quartus(args) 1] -current_revision

###
# Generate Timing Netlist
###
puts "\n-- Generate Timing Netlist (Worst Case) --\n"
create_timing_netlist
create_p2p_delays


###
# Filter the pins name in the timing database.
###
foreach_in_collection node [get_timing_nodes -type pin] {
	set node_name [get_timing_node_info -info name $node]

	# DQ Pin?
	if {[regexp $name_dq_pin $node_name]} {
		set node_dq_pin $node
		}

	# Clock Pin?
	if {[regexp $name_clk_pin $node_name]} {
		set node_clk_pin $node
		}
	}


###
# Find the clock delay from the PLL to the sdram data output pin.
# Assuming all sdram outputs have the same delay (including sdram clock).
###
set delays_from_clock [get_delays_from_clocks $node_dq_pin]

foreach delay $delays_from_clock {
	regexp {^[0-9].[0-9]} [lindex $delay 1] clock_delay_to_dq_pin_max
}


###
# Find the clock delay from the PLL to the sdram clock output pin.
# Assuming all sdram outputs have the same delay (including sdram clock).
###
set delays_from_clock [get_delays_from_clocks $node_clk_pin]

foreach delay $delays_from_clock {
	regexp {^[0-9].[0-9]} [lindex $delay 1] clock_delay_to_clk_pin_max
}



###
# Find the clock delay from the PLL to the sdram capture registers
###

foreach_in_collection node [get_timing_nodes -type reg] {
	set node_name [get_timing_node_info -info name $node]

	if {[regexp $name_dq_reg $node_name]} {
		set node_dq_reg $node
		}
	}

set delays_from_clock [get_delays_from_clocks $node_dq_reg]
foreach delay $delays_from_clock {
	regexp {^[0-9].[0-9]} [lindex $delay 1] clock_delay_to_dq_reg_max
}


###
# Find the delay from the sdram input pins to the capture register
###
set delays_from_capture [get_delays_from_keepers $node_dq_reg]
foreach delay $delays_from_capture {
	regexp {^[0-9].[0-9]} [lindex $delay 1] pin_delay_to_dq_reg_max
	}


delete_timing_netlist

###
# Generate Timing Netlist
###
puts "\n-- Generate Timing Netlist (Best Case) --\n"
create_timing_netlist -fast_model
create_p2p_delays

###
# Filter the pins name in the timing database.
###
foreach_in_collection node [get_timing_nodes -type pin] {
	set node_name [get_timing_node_info -info name $node]

	# DQ Pin?
	if {[regexp $name_dq_pin $node_name]} {
		set node_dq_pin $node
		}

	# Clock Pin?
	if {[regexp $name_clk_pin $node_name]} {
		set node_clk_pin $node
		}
	}

###
# Find the clock delay from the PLL to the sdram output pin.
# Assuming all sdram outputs have the same delay (including sdram clock).
###
set delays_from_clock [get_delays_from_clocks $node_dq_pin]
foreach delay $delays_from_clock {
	regexp {^[0-9].[0-9]} [lindex $delay 1] clock_delay_to_dq_pin_min
}

###
# Find the clock delay from the PLL to the sdram clock output pin.
# Assuming all sdram outputs have the same delay (including sdram clock).
###
set delays_from_clock [get_delays_from_clocks $node_clk_pin]
foreach delay $delays_from_clock {
	regexp {^[0-9].[0-9]} [lindex $delay 1] clock_delay_to_clk_pin_min
}


###
# Find the clock delay from the PLL to the sdram capture registers
###

foreach_in_collection node [get_timing_nodes -type reg] {
	set node_name [get_timing_node_info -info name $node]

	if {[regexp $name_dq_reg $node_name]} {
		set node_dq_reg $node
		}
	}

set delays_from_clock [get_delays_from_clocks $node_dq_reg]
foreach delay $delays_from_clock {
	regexp {^[0-9].[0-9]} [lindex $delay 1] clock_delay_to_dq_reg_min
}

###
# Find the delay from the sdram input pins to the capture register
###
set delays_from_capture [get_delays_from_keepers $node_dq_reg]
foreach delay $delays_from_capture {
	regexp {^[0-9].[0-9]} [lindex $delay 1] pin_delay_to_dq_reg_min
}


puts "Worst Case Clock delay from pll to DQ pins : $clock_delay_to_dq_pin_max"
puts "Worst Case Clock delay from pll to DQ capture register : $clock_delay_to_dq_reg_max"
puts "Worst Case Routing delay from pin to DQ capture register : $pin_delay_to_dq_reg_max"
puts "Best Case Clock delay from pll to DQ pins : $clock_delay_to_dq_pin_min"
puts "Best Case Clock delay from pll to DQ capture register : $clock_delay_to_dq_reg_min"
puts "Best Case Routing delay from pin to DQ capture register : $pin_delay_to_dq_reg_min"


###
# Calculate Worst Case Delays
###

set pll_clk 0
for { set i 1 } { $i < 2} { incr i } {
	lappend pll_clk $clk_period
}

for { set i 0 } { $i < 2} { incr i } {

	# The delay to the sdram clock input is the delay from internal PLL rising edge to the output pins plus the trace delay.
	lappend sdr_clk_max [expr [lindex $pll_clk $i] + $clock_delay_to_dq_pin_max +  $trace_delay]

	# The delay to the capture register is the rising edge of the PLL clock plus the routing delay.
	lappend capture_clk_max [expr [lindex $pll_clk $i] + $clock_delay_to_dq_reg_max]
}

set dq_from_sdram_valid_max [expr [lindex $sdr_clk_max 0] + $dq_access]
set dq_from_sdram_hold_max [expr [lindex $sdr_clk_max 1] + $dq_hold]
set capture_dq_valid_max [expr $dq_from_sdram_valid_max + $trace_delay + $pin_delay_to_dq_reg_max]
set capture_dq_hold_max [expr $dq_from_sdram_hold_max + $trace_delay + $pin_delay_to_dq_reg_max]

###
# Calculate Best Case Delays
###
for { set i 0 } { $i < 2} { incr i } {
	# The delay to the sdram clock input is the delay from internal PLL rising edge to the output pins plus the trace delay.
	lappend sdr_clk_min [expr [lindex $pll_clk $i] + $clock_delay_to_dq_pin_min +  $trace_delay]

	# The delay to the capture register is the rising edge of the PLL clock plus the routing delay.
	lappend capture_clk_min [expr [lindex $pll_clk $i] + $clock_delay_to_dq_reg_min]
}

set dq_from_sdram_valid_min [expr [lindex $sdr_clk_min 0] + $dq_access]
set dq_from_sdram_hold_min [expr [lindex $sdr_clk_min 1] + $dq_hold]
set capture_dq_valid_min [expr $dq_from_sdram_valid_min + $trace_delay + $pin_delay_to_dq_reg_min]
set capture_dq_hold_min [expr $dq_from_sdram_hold_min + $trace_delay + $pin_delay_to_dq_reg_min]


# The capture clock delay is equal to the valid data from the sdram plus the margin minus the capture clock edge.
set capture_clk_shift [expr $capture_dq_valid_max - [lindex $capture_clk_max 1] + $margin]


set shift_min [expr [lindex $capture_clk_min 1] + $capture_clk_shift]
puts $shift_min

if { $shift_min < $capture_dq_hold_min } {
	puts ok
	} else {
	puts false
	}

###
# Calculate the phase shift for the sdram clock out.
###

set sdr_clk_shift_max [expr $clock_delay_to_dq_pin_max - $clock_delay_to_clk_pin_max]
set sdr_clk_shift_min [expr $clock_delay_to_dq_pin_min - $clock_delay_to_clk_pin_min]
set sdr_clk_shift [expr ($sdr_clk_shift_max + $sdr_clk_shift_min) / 2]

###
# Report
###

load_report
set folder "Microtronix SDRAM Controller"
set folder_id [create_report_panel -folder $folder]

# Set table name and id
set table "$folder||PLL Settings"
set table_id [get_report_panel_id $table]

# Check if specified table exists. If so, delete it.
if {$table_id != -1} {
delete_report_panel -id $table_id
}

# Create the specified table and get its id
set table_id [create_report_panel -table $table]

# Add Timing Analyzer Summary to the table
add_row_to_table -id $table_id {{Name} {Value}}
add_row_to_table -id $table_id "{Capture Clock Phase Shift} \{$capture_clk_shift ns\}"
add_row_to_table -id $table_id "{Clock Out Phase Shift} \{$sdr_clk_shift ns\}"

# Save the changes to the report database
save_report_database
unload_report

# Remove script
set_global_assignment -name POST_FLOW_SCRIPT_FILE -remove

project_close
