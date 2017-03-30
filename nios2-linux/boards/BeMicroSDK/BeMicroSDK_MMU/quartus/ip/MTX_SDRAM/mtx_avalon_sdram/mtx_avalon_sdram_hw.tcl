# Microtronix Avalon SDRAM Controller mtx_avalon_sdram_hw.tcl
# For more information and updates please visit http://www.microtronix.com

# Build: 169/9.7.2010

# +-----------------------------------
# | module mtx_avalon_sdram
# | 
set_module_property NAME mtx_avalon_sdram
set_module_property VERSION 5.4
set_module_property INTERNAL false
set_module_property GROUP "Memories and Memory Controllers/SDRAM,Microtronix"
set_module_property AUTHOR Microtronix
#set_module_property DISPLAY_NAME "Microtronix Multi-port Avalon SDRAM Memory Controller"
set_module_property DISPLAY_NAME "Multi-port SDRAM Controller"
set_module_property DESCRIPTION "Multi-port memory controller supporting SDR, DDR, DDR2 and Mobile DDR<br>Build: 169/9.7.2010"
set_module_property LIBRARIES {ieee.std_logic_1164.all mtx_sdram.mtx_sdram_package.all std.standard.all}
set_module_property INSTANTIATE_IN_SYSTEM_MODULE true
set_module_property EDITABLE false
set_module_property VALIDATION_CALLBACK mtx_avalon_sdram_validation
set_module_property ELABORATION_CALLBACK mtx_avalon_sdram_elaboration
set_module_property GENERATION_CALLBACK mtx_avalon_sdram_generation
set_module_property HELPER_JAR "mtx_avalon_sdram_gui.jar"
set_module_property DATASHEET_URL "http://www.microtronix.com/products/?product_id=92"
# | 
# +-----------------------------------

# +-----------------------------------
# | files
# | 
add_file mtx_sdram_package.vhd {SYNTHESIS}
add_file mtx_sdram_controller.vhd {SYNTHESIS}
add_file mtx_sdram_controller.ocp {SYNTHESIS}
add_file mtx_sdram_controller_scheduler.vhd {SYNTHESIS}
add_file mtx_sdram_sdr_bidir.vhd {SYNTHESIS}
add_file mtx_sdram_ddr_bidir.vhd {SYNTHESIS}
add_file mtx_sdram_ddr_clk_ctrl.vhd {SYNTHESIS}
add_file mtx_sdram_ddr_output.vhd {SYNTHESIS}
add_file mtx_sdram_avalon_random.vhd {SYNTHESIS}
add_file mtx_sdram_avalon_burst.vhd {SYNTHESIS}
# | 
# +-----------------------------------

# +-----------------------------------
# | parameters
# | 
set ARCH_SDR 0
set ARCH_DDR 1
set ARCH_DDR2 2
set ARCH_MDDR 3

set PORT_A 0
set PORT_B 1
set PORT_C 2
set PORT_D 3
set PORT_E 4
set PORT_F 5
set PORT_G 6
set PORT_H 7
set PORT_I 8
set PORT_J 9
set PORT_K 10
set PORT_L 11
set PORT_M 12
set PORT_N 13
set PORT_O 14
set PORT_P 15

set MODE_DISABLED 0
set MODE_BURST 1
set MODE_RANDOM 2
		
set ports($PORT_A) port_a
set ports($PORT_B) port_b
set ports($PORT_C) port_c
set ports($PORT_D) port_d
set ports($PORT_E) port_e
set ports($PORT_F) port_f
set ports($PORT_G) port_g
set ports($PORT_H) port_h
set ports($PORT_I) port_i
set ports($PORT_J) port_j
set ports($PORT_K) port_k
set ports($PORT_L) port_l
set ports($PORT_M) port_m
set ports($PORT_N) port_n
set ports($PORT_O) port_o
set ports($PORT_P) port_p

set ports_cap($PORT_A) PORT_A
set ports_cap($PORT_B) PORT_B
set ports_cap($PORT_C) PORT_C
set ports_cap($PORT_D) PORT_D
set ports_cap($PORT_E) PORT_E
set ports_cap($PORT_F) PORT_F
set ports_cap($PORT_G) PORT_G
set ports_cap($PORT_H) PORT_H
set ports_cap($PORT_I) PORT_I
set ports_cap($PORT_J) PORT_J
set ports_cap($PORT_K) PORT_K
set ports_cap($PORT_L) PORT_L
set ports_cap($PORT_M) PORT_M
set ports_cap($PORT_N) PORT_N
set ports_cap($PORT_O) PORT_O
set ports_cap($PORT_P) PORT_P

for { set i 0 } { $i < ($PORT_P+1) } { incr i 1 } {
	if { $i > 0 } {
		add_parameter $ports($i)_mode_string string "Disabled"
		set_parameter_property $ports($i)_mode_string ALLOWED_RANGES {Disabled Burst Random}
		add_parameter $ports($i)_mode int 0
		set_parameter_property $ports($i)_mode DERIVED true
		set_parameter_property $ports($i)_mode VISIBLE false
	} else {
		add_parameter $ports($i)_mode_string string "Random"
		set_parameter_property $ports($i)_mode_string ALLOWED_RANGES {Burst Random}
		add_parameter $ports($i)_mode int 2
		set_parameter_property $ports($i)_mode DERIVED true
		set_parameter_property $ports($i)_mode VISIBLE false
	}
	set_parameter_property $ports($i)_mode_string DISPLAY_NAME "Port [format %c [expr $i+65]] Type"

	add_parameter $ports($i)_buffer_size int 16
	set_parameter_property $ports($i)_buffer_size ALLOWED_RANGES {4 8 16 32 64 128 256 512 1024 2048 4096}
	set_parameter_property $ports($i)_buffer_size DISPLAY_NAME "Port [format %c [expr $i+65]] Buffer Size (words)"

	add_parameter $ports($i)_max_burst positive 16
	set_parameter_property $ports($i)_max_burst DISPLAY_NAME "Port [format %c [expr $i+65]] Maximum Burst Length (words)"

	add_parameter $ports($i)_data_width int 16
	set_parameter_property $ports($i)_data_width ALLOWED_RANGES {16 32}
	set_parameter_property $ports($i)_data_width DISPLAY_NAME "Avalon Data Width"

	add_parameter $ports($i)_disable_cache boolean false
	set_parameter_property $ports($i)_disable_cache DISPLAY_NAME "Force buffer reload on every Avalon read"
	set_parameter_property $ports($i)_disable_cache DISPLAY_HINT boolean
	set_parameter_property $ports($i)_disable_cache AFFECTS_ELABORATION false
}

add_parameter mem_architecture_string string "DDR"
set_parameter_property mem_architecture_string ALLOWED_RANGES {"SDR" "DDR" "DDR2" "Mobile DDR"}
set_parameter_property mem_architecture_string DISPLAY_NAME "Memory Achitecture"
set_parameter_property mem_architecture_string DISPLAY_HINT radio
add_parameter mem_architecture int 1
set_parameter_property mem_architecture DERIVED true
set_parameter_property mem_architecture VISIBLE false

add_parameter mem_scheduler_parameter int 0
set_parameter_property mem_scheduler_parameter DISPLAY_NAME "Custom Scheduler Parameter"
set_parameter_property mem_scheduler_parameter AFFECTS_ELABORATION false

add_parameter mem_data_width int 16
set_parameter_property mem_data_width ALLOWED_RANGES {8 16 32 64}
set_parameter_property mem_data_width DISPLAY_NAME "Total Data Width (bits)"

add_parameter mem_bank_bits int 2
set_parameter_property mem_bank_bits ALLOWED_RANGES {1 2 3}
set_parameter_property mem_bank_bits DISPLAY_NAME "Bank Address Bits"

add_parameter mem_row_bits int 12
set_parameter_property mem_row_bits ALLOWED_RANGES {10 11 12 13 14}
set_parameter_property mem_row_bits DISPLAY_NAME "Row Address Bits"

add_parameter mem_column_bits int 8
set_parameter_property mem_column_bits ALLOWED_RANGES {7 8 9 10}
set_parameter_property mem_column_bits DISPLAY_NAME "Column Address Bits"

add_parameter mem_prefix string "sdram_"
set_parameter_property mem_prefix DISPLAY_NAME "Pin Prefix"
set_parameter_property mem_prefix AFFECTS_ELABORATION false

add_parameter mem_devices int 1
set_parameter_property mem_devices ALLOWED_RANGES {1 2 4 8}
set_parameter_property mem_devices DISPLAY_NAME "Total Number of Discrete Memory Devices"

add_parameter mem_dqs int 1
set_parameter_property mem_dqs ALLOWED_RANGES {1 2 4}
set_parameter_property mem_dqs DISPLAY_NAME "DQS Pins per Discrete Memory Device"

add_parameter mem_diff_dqs boolean 0
set_parameter_property mem_diff_dqs DISPLAY_NAME "Differential DQS"
set_parameter_property mem_diff_dqs DISPLAY_HINT boolean

add_parameter mem_clocks int 1
set_parameter_property mem_clocks ALLOWED_RANGES {1 2 3 4}
set_parameter_property mem_clocks DISPLAY_NAME "Total Number of Clock Pairs"

add_parameter mem_reduce_drive_strength boolean 0
set_parameter_property mem_reduce_drive_strength DISPLAY_NAME "Reduce Drive Strength of Memory"
set_parameter_property mem_reduce_drive_strength DISPLAY_HINT boolean

add_parameter mem_phase_shift boolean 0
set_parameter_property mem_phase_shift DISPLAY_NAME "Use Phase-Shifted Address/Command Clock"
set_parameter_property mem_phase_shift DISPLAY_HINT boolean

add_parameter mem_deep_power_down boolean 0
set_parameter_property mem_deep_power_down DISPLAY_NAME "Deep Power Down Support"
set_parameter_property mem_deep_power_down DISPLAY_HINT boolean

add_parameter mem_odt_string string "Disabled"
set_parameter_property mem_odt_string ALLOWED_RANGES {"Disabled" "50 ohms" "75 ohms" "150 ohms"}
set_parameter_property mem_odt_string DISPLAY_NAME "Memory On-Die Termination"
add_parameter mem_odt int 0
set_parameter_property mem_odt DERIVED true
set_parameter_property mem_odt VISIBLE false

add_parameter mem_manual_scripts boolean 0
set_parameter_property mem_manual_scripts DISPLAY_NAME "Run Assignment Scripts Manually"
set_parameter_property mem_manual_scripts DISPLAY_HINT boolean
set_parameter_property mem_manual_scripts AFFECTS_ELABORATION false

add_parameter mem_disable_dedicated_ddr_blocks boolean 0
set_parameter_property mem_disable_dedicated_ddr_blocks DISPLAY_NAME "Disable use of dedicated DDR hardware blocks"
set_parameter_property mem_disable_dedicated_ddr_blocks DISPLAY_HINT boolean
set_parameter_property mem_disable_dedicated_ddr_blocks AFFECTS_ELABORATION false

add_parameter timing_frequency int 167
set_parameter_property timing_frequency ALLOWED_RANGES {100 133 167 200 233 267 300 333}
set_parameter_property timing_frequency DISPLAY_NAME "SDRAM Frequency (MHz)"
set_parameter_property timing_frequency AFFECTS_ELABORATION false

add_parameter timing_tinit natural 200
set_parameter_property timing_tinit DISPLAY_NAME "Memory Initialization Time (tINIT) (ns)"
set_parameter_property timing_tinit AFFECTS_ELABORATION false

add_parameter timing_trp natural 15
set_parameter_property timing_trp DISPLAY_NAME "Precharge Command Period (tRP) (ns)"
set_parameter_property timing_trp AFFECTS_ELABORATION false

add_parameter timing_tmrd natural 12
set_parameter_property timing_tmrd DISPLAY_NAME "Load Mode Register Command Period (tMRD) (ns)"
set_parameter_property timing_tmrd AFFECTS_ELABORATION false

add_parameter timing_trfc natural 105
set_parameter_property timing_trfc DISPLAY_NAME "Auto Refresh Command Period (tRFC) (ns)"
set_parameter_property timing_trfc AFFECTS_ELABORATION false

add_parameter timing_trefi natural 7800
set_parameter_property timing_trefi DISPLAY_NAME "Average Periodic Refresh Interval (tREFI) (ns)"
set_parameter_property timing_trefi AFFECTS_ELABORATION false

add_parameter timing_trcd natural 15
set_parameter_property timing_trcd DISPLAY_NAME "Active to Write or Read Delay (tRCD) (ns)"
set_parameter_property timing_trcd AFFECTS_ELABORATION false

add_parameter timing_twr natural 15
set_parameter_property timing_twr DISPLAY_NAME "Write Recovery Time (tWR) (ns)"
set_parameter_property timing_twr AFFECTS_ELABORATION false

add_parameter timing_tras natural 40
set_parameter_property timing_tras DISPLAY_NAME "Active to Precharge Command (tRAS) (ns)"
set_parameter_property timing_tras AFFECTS_ELABORATION false

add_parameter timing_trc natural 55
set_parameter_property timing_trc DISPLAY_NAME "Active to Active Command Period (tRC) (ns)"
set_parameter_property timing_trc AFFECTS_ELABORATION false

add_parameter timing_tac string "5.5"
set_parameter_property timing_tac DISPLAY_NAME "Access Time (tAC) (ns)"
set_parameter_property timing_tac AFFECTS_ELABORATION false

add_parameter timing_toh string "2.5"
set_parameter_property timing_toh DISPLAY_NAME "Output Data Hold Time (tOH) (ns)"
set_parameter_property timing_toh AFFECTS_ELABORATION false

add_parameter timing_pcb_delay natural 100
set_parameter_property timing_pcb_delay DISPLAY_NAME "PCB Trace Delay (ps)"
set_parameter_property timing_pcb_delay AFFECTS_ELABORATION false

add_parameter sdc_enable boolean 0
set_parameter_property sdc_enable DISPLAY_NAME "Enable SDC file creation"
set_parameter_property sdc_enable DISPLAY_HINT boolean
set_parameter_property sdc_enable AFFECTS_ELABORATION false
add_parameter sdc_enable_derived boolean 0
set_parameter_property sdc_enable_derived DERIVED true
set_parameter_property sdc_enable_derived VISIBLE false

add_parameter sdc_tck string "6"
set_parameter_property sdc_tck DISPLAY_NAME "Clock cycle time (ns)"
set_parameter_property sdc_tck AFFECTS_ELABORATION false

add_parameter sdc_tds string "0.3"
set_parameter_property sdc_tds DISPLAY_NAME "DQ and DM input setup time relative to DQS (tDS) (ns)"
set_parameter_property sdc_tds AFFECTS_ELABORATION false

add_parameter sdc_tdh string "0.3"
set_parameter_property sdc_tdh DISPLAY_NAME "DQ and DM input hold time relative to DQS (tDH) (ns)"
set_parameter_property sdc_tdh AFFECTS_ELABORATION false

add_parameter sdc_tis string "0.375"
set_parameter_property sdc_tis DISPLAY_NAME "Address and command input setup time (tIS) (ns)"
set_parameter_property sdc_tis AFFECTS_ELABORATION false

add_parameter sdc_tih string "0.375"
set_parameter_property sdc_tih DISPLAY_NAME "Address and command input hold time (tIS) (ns)"
set_parameter_property sdc_tih AFFECTS_ELABORATION false

add_parameter sdc_tdss string "0.2"
set_parameter_property sdc_tdss DISPLAY_NAME "DQS falling edge to CK rising - setup time (tDSS) (tCK)"
set_parameter_property sdc_tdss AFFECTS_ELABORATION false

add_parameter sdc_tdsh string "0.2"
set_parameter_property sdc_tdsh DISPLAY_NAME "DQS falling edge to CK rising - hold time (tDSH) (tCK)"
set_parameter_property sdc_tdsh AFFECTS_ELABORATION false

add_parameter sdc_tdqsq string "0.2"
set_parameter_property sdc_tdqsq DISPLAY_NAME "DQS-DQ skew, DQS to last DQ valid, per group, per access (tDQSQ) (ns)"
set_parameter_property sdc_tdqsq AFFECTS_ELABORATION false

add_parameter sdc_tqhs string "0.3"
set_parameter_property sdc_tqhs DISPLAY_NAME "Data hold skew factor (tQHS) (ns)"
set_parameter_property sdc_tqhs AFFECTS_ELABORATION false

add_parameter sdc_tdqsck string "0.3"
set_parameter_property sdc_tdqsck DISPLAY_NAME "DQS output access time from CK/CK# (tDQSCK) (ns)"
set_parameter_property sdc_tdqsck AFFECTS_ELABORATION false

add_parameter sdc_tdqss string "0.25"
set_parameter_property sdc_tdqss DISPLAY_NAME "Write command to first DQS latching transition (tDQSCK) (tCK)"
set_parameter_property sdc_tdqss AFFECTS_ELABORATION false

add_parameter sdc_input string "20"
set_parameter_property sdc_input DISPLAY_NAME "PLL input clock period (ns)"
set_parameter_property sdc_input AFFECTS_ELABORATION false

# | 
# +-----------------------------------

# +-----------------------------------
# | ports
# | 
add_interface clock_$ports($PORT_A) clock end
set_interface_property clock_$ports($PORT_A) ENABLED true
#add_interface_port clock_$ports($PORT_A) AS_$ports_cap($PORT_A)_CLK clk Input 1
#add_interface_port clock_$ports($PORT_A) AS_$ports_cap($PORT_A)_RST reset Input 1

add_interface $ports($PORT_A) avalon end
set_interface_property $ports($PORT_A) ASSOCIATED_CLOCK clock_$ports($PORT_A)
set_interface_property $ports($PORT_A) ENABLED true
set_interface_property $ports($PORT_A) addressGroup 1

for { set i 1 } { $i < ($::PORT_P+1) } { incr i 1 } {
	add_interface clock_$ports($i) clock end

	add_interface $ports($i) avalon end
	set_interface_property $ports($i) ASSOCIATED_CLOCK clock_$ports($i)

	set_interface_property clock_$ports($i) ENABLED false
	set_interface_property $ports($i) ENABLED false
	set_interface_property $ports($i) addressGroup 1
}

add_interface sdram_interface conduit end
set_interface_property sdram_interface ENABLED true

# | 
# +-----------------------------------

proc mtx_avalon_sdram_validation {} {
	# Set value of derived parameters
	set port_mode_string [get_parameter_value $::ports($::PORT_A)_mode_string]

	if { $port_mode_string == "Burst" } {
		set_parameter_value $::ports($::PORT_A)_mode $::MODE_BURST
	} elseif { $port_mode_string == "Random" } {
		set_parameter_value $::ports($::PORT_A)_mode $::MODE_RANDOM
	}

	set mem_architecture_string [get_parameter_value mem_architecture_string]
	if { $mem_architecture_string == "SDR" } {
		set_parameter_value mem_architecture $::ARCH_SDR
	} elseif { $mem_architecture_string == "DDR"  } {
		set_parameter_value mem_architecture $::ARCH_DDR
	} elseif { $mem_architecture_string == "DDR2"  } {
		set_parameter_value mem_architecture $::ARCH_DDR2
	} else {
		set_parameter_value mem_architecture $::ARCH_MDDR
	}

	set mem_odt_string [get_parameter_value mem_odt_string]
	if { $mem_odt_string == "Disabled" } {
		set_parameter_value mem_odt 0
	} elseif { $mem_odt_string == "50 ohms"  } {
		set_parameter_value mem_odt 50
	} elseif { $mem_odt_string == "75 ohms"  } {
		set_parameter_value mem_odt 75
	} else {
		set_parameter_value mem_odt 150
	}

	set scheduler [get_parameter_value mem_scheduler_parameter]
	if { [expr $scheduler < 0] } {
		send_message Error "Custom Scheduler Parameter must be positive or zero."
	}

	# check decimal values in strings
	set tac [get_parameter_value timing_tac]
	if { [string is double -strict $tac] } {
		if { [expr $tac < 0.0] } {
			send_message Error "tAC must be positive."
		}
	} else {
		send_message Error "tAC must be a number."
	}

	set toh [get_parameter_value timing_toh]
	if { [string is double -strict $toh] } {
		if { [expr $toh < 0.0] } {
			send_message Error "tOH must be positive."
		}
	} else {
		send_message Error "tOH must be a number."
	}

	set tck [get_parameter_value sdc_tck]
	if { [string is double -strict $tck] } {
		if { [expr $tck < 0.0] } {
			send_message Error "Clock cycle time must be positive."
		}
	} else {
		send_message Error "Clock cycle time must be a number."
	}

	set tds [get_parameter_value sdc_tds]
	if { [string is double -strict $tds] } {
		if { [expr $tds < 0.0] } {
			send_message Error "tDS must be positive."
		}
	} else {
		send_message Error "tDS must be a number."
	}

	set tdh [get_parameter_value sdc_tdh]
	if { [string is double -strict $tdh] } {
		if { [expr $tdh < 0.0] } {
			send_message Error "tDH must be positive."
		}
	} else {
		send_message Error "tDH must be a number."
	}

	set tis [get_parameter_value sdc_tis]
	if { [string is double -strict $tis] } {
		if { [expr $tis < 0.0] } {
			send_message Error "tIS must be positive."
		}
	} else {
		send_message Error "tIS must be a number."
	}

	set tih [get_parameter_value sdc_tih]
	if { [string is double -strict $tih] } {
		if { [expr $tih < 0.0] } {
			send_message Error "tIH must be positive."
		}
	} else {
		send_message Error "tIH must be a number."
	}

	set tdss [get_parameter_value sdc_tdss]
	if { [string is double -strict $tdss] } {
		if { [expr $tdss < 0.0] } {
			send_message Error "tDSS must be positive."
		}
	} else {
		send_message Error "tDSS must be a number."
	}

	set tdsh [get_parameter_value sdc_tdsh]
	if { [string is double -strict $tdsh] } {
		if { [expr $tdsh < 0.0] } {
			send_message Error "tDSH must be positive."
		}
	} else {
		send_message Error "tDSH must be a number."
	}

	set tdqsq [get_parameter_value sdc_tdqsq]
	if { [string is double -strict $tdqsq] } {
		if { [expr $tdqsq < 0.0] } {
			send_message Error "tDQSQ must be positive."
		}
	} else {
		send_message Error "tDQSQ must be a number."
	}

	set tqhs [get_parameter_value sdc_tqhs]
	if { [string is double -strict $tqhs] } {
		if { [expr $tqhs < 0.0] } {
			send_message Error "tQHS must be positive."
		}
	} else {
		send_message Error "tQHS must be a number."
	}

	set tdqsck [get_parameter_value sdc_tdqsck]
	if { [string is double -strict $tdqsck] } {
		if { [expr $tdqsck < 0.0] } {
			send_message Error "tDQSCK must be positive."
		}
	} else {
		send_message Error "tDQSCK must be a number."
	}

	set tdqss [get_parameter_value sdc_tdqss]
	if { [string is double -strict $tdqss] } {
		if { [expr $tdqss < 0.0] } {
			send_message Error "tDQSS must be positive."
		}
	} else {
		send_message Error "tDQSS must be a number."
	}

	set sdc_in [get_parameter_value sdc_input]
	if { [string is double -strict $sdc_in] } {
		if { [expr $sdc_in < 0.0] } {
			send_message Error "Input clock period must be positive."
		}
	} else {
		send_message Error "Input clock period must be a number."
	}

	# disable ports above the first disabled port
	set found_disabled 99

	for { set i 1 } { $i < ($::PORT_P+1) } { incr i 1 } {
		set port_mode_string [get_parameter_value $::ports($i)_mode_string]

		if { $found_disabled < $i } {
			set_parameter_value $::ports($i)_mode $::MODE_DISABLED
			set_parameter_property $::ports($i)_mode_string ENABLED false
		} else {
			if { $port_mode_string == "Burst" } {
				set_parameter_value $::ports($i)_mode $::MODE_BURST
			} elseif { $port_mode_string == "Random" } {
				set_parameter_value $::ports($i)_mode $::MODE_RANDOM
			} else {
				set_parameter_value $::ports($i)_mode $::MODE_DISABLED
			}
			set_parameter_property $::ports($i)_mode_string ENABLED true
		}

		if { $found_disabled == 99 && $port_mode_string == "Disabled" } {
			set_parameter_value $::ports($i)_mode $::MODE_DISABLED
			set found_disabled [expr $i]
		}

	}

	# enable and disable port parameters as needed
	for { set i 0 } { $i < ($::PORT_P+1) } { incr i 1 } {
		set port_mode [get_parameter_value $::ports($i)_mode]
		set arch [get_parameter_value mem_architecture]
		set mem_width [get_parameter_value mem_data_width]

		if { $port_mode == $::MODE_DISABLED } {
			set_parameter_property $::ports($i)_buffer_size ENABLED false
			set_parameter_property $::ports($i)_max_burst ENABLED false
			set_parameter_property $::ports($i)_data_width ENABLED false
			set_parameter_property $::ports($i)_disable_cache ENABLED false
		} else {
			set_parameter_property $::ports($i)_buffer_size ENABLED true
		}

		if { $port_mode == $::MODE_BURST } {
			set_parameter_property $::ports($i)_max_burst ENABLED true
			if { $arch != $::ARCH_SDR && $mem_width == 8 } {
				set_parameter_property $::ports($i)_data_width ENABLED true
			} else {
				set_parameter_property $::ports($i)_data_width ENABLED false
			}
			set_parameter_property $::ports($i)_disable_cache ENABLED false
		} elseif { $port_mode == $::MODE_RANDOM } {
			set_parameter_property $::ports($i)_max_burst ENABLED false
			set_parameter_property $::ports($i)_data_width ENABLED false
			set_parameter_property $::ports($i)_disable_cache ENABLED true
		}
	}

	set arch [get_parameter_value mem_architecture]
	set sdc_enable [get_parameter_value sdc_enable]
	if { $arch == $::ARCH_SDR } {
		set_parameter_property mem_clocks ENABLED false
		set_parameter_property mem_dqs ENABLED false
		set_parameter_property mem_reduce_drive_strength ENABLED false
		set_parameter_property mem_phase_shift ENABLED false

		set_parameter_property timing_tac ENABLED true
		set_parameter_property timing_toh ENABLED true
		set_parameter_property timing_pcb_delay ENABLED true

		set_parameter_property sdc_enable ENABLED false

	} else {
		set_parameter_property mem_clocks ENABLED true
		set_parameter_property mem_dqs ENABLED true
		set_parameter_property mem_reduce_drive_strength ENABLED true
		set_parameter_property mem_phase_shift ENABLED true

		set_parameter_property timing_tac ENABLED false
		set_parameter_property timing_toh ENABLED false
		set_parameter_property timing_pcb_delay ENABLED false

		set_parameter_property sdc_enable ENABLED true
	}

	if { $arch == $::ARCH_MDDR } {
		set_parameter_property mem_deep_power_down ENABLED true
	} else {
		set_parameter_property mem_deep_power_down ENABLED false
	}

	if { $arch == $::ARCH_DDR2 } {
		set_parameter_property mem_odt_string ENABLED true
	} else {
		set_parameter_property mem_odt_string ENABLED false
	}

	set family [get_project_property DEVICE_FAMILY_NAME]
	if { $family != "STRATIXII" && $family != "STRATIXIIGX" && $family != "STRATIXIII" && $family != "STRATIXIIGXLITE" && $family != "ARRIAII" && $family != "CYCLONEII" && $family != "CYCLONEIII" && $family != "CYCLONEIVE" && $family != "STINGRAY" && $family != "HARDCOPY" && $family != "HARDCOPYII" && $family != "HARDCOPYIII" } {
		send_message Error "Device family $family is not supported."
	}

	if { $arch == $::ARCH_DDR2 && $family == "STRATIXIII" } {
		set_parameter_property mem_diff_dqs ENABLED true
	} else {
#		set_parameter_value mem_diff_dqs false
		set_parameter_property mem_diff_dqs ENABLED false
	}

	if { $family == "HARDCOPYII" || $family == "HARDCOPYIII" } {
		set_parameter_property mem_disable_dedicated_ddr_blocks ENABLED true
	} else {
		set_parameter_property mem_disable_dedicated_ddr_blocks ENABLED false
	}


	set sdc_enable [get_parameter_value sdc_enable]
	if { $sdc_enable == "false" || $arch == $::ARCH_SDR } {
		set_parameter_value sdc_enable_derived false
		set_parameter_property sdc_tck ENABLED false
		set_parameter_property sdc_tds ENABLED false
		set_parameter_property sdc_tdh ENABLED false
		set_parameter_property sdc_tis ENABLED false
		set_parameter_property sdc_tih ENABLED false
		set_parameter_property sdc_tdss ENABLED false
		set_parameter_property sdc_tdsh ENABLED false
		set_parameter_property sdc_tdqsq ENABLED false
		set_parameter_property sdc_tqhs ENABLED false
		set_parameter_property sdc_tdqsck ENABLED false
		set_parameter_property sdc_tdqss ENABLED false
		set_parameter_property sdc_input ENABLED false
	} else {
		set_parameter_value sdc_enable_derived true
		set_parameter_property sdc_tck ENABLED true
		set_parameter_property sdc_tds ENABLED true
		set_parameter_property sdc_tdh ENABLED true
		set_parameter_property sdc_tis ENABLED true
		set_parameter_property sdc_tih ENABLED true
		set_parameter_property sdc_tdss ENABLED true
		set_parameter_property sdc_tdsh ENABLED true
		set_parameter_property sdc_tdqsq ENABLED true
		set_parameter_property sdc_tqhs ENABLED true
		set_parameter_property sdc_tdqsck ENABLED true
		set_parameter_property sdc_tdqss ENABLED true
		set_parameter_property sdc_input ENABLED true
	}

	set mem_data_width [get_parameter_value mem_data_width]
	set mem_bank_bits [get_parameter_value mem_bank_bits]
	set mem_row_bits [get_parameter_value mem_row_bits]
	set mem_column_bits [get_parameter_value mem_column_bits]
	set num_bits [expr {pow(2, $mem_row_bits) * pow(2, $mem_column_bits) * pow(2, $mem_bank_bits) * $mem_data_width}]
	set num_megabytes [expr round($num_bits / 1024 / 1024 / 8)]
	send_message Info "SDRAM Memory Capacity: $num_megabytes Mbytes"

}

proc mtx_avalon_sdram_elaboration {} {
	set mem_architecture [get_parameter_value mem_architecture]
	set mem_data_width [get_parameter_value mem_data_width]
	set mem_bank_bits [get_parameter_value mem_bank_bits]
	set mem_row_bits [get_parameter_value mem_row_bits]
	set mem_column_bits [get_parameter_value mem_column_bits]
	set avalon_data_width 0

	for { set i 0 } { $i < ($::PORT_P+1) } { incr i 1 } {
		set port_mode [get_parameter_value $::ports($i)_mode]
		set port_data_width [get_parameter_value $::ports($i)_data_width]

		if { $port_mode == $::MODE_DISABLED } {
			set_interface_property clock_$::ports($i) ENABLED false
			set_interface_property $::ports($i) ENABLED false
		} else {
			set_interface_property clock_$::ports($i) ENABLED true
			set_interface_property $::ports($i) ENABLED true

			set_interface_property $::ports($i) addressAlignment DYNAMIC
			set_interface_property $::ports($i) bridgesToMaster ""
			set_interface_property $::ports($i) burstOnBurstBoundariesOnly false
			set_interface_property $::ports($i) holdTime 0
			set_interface_property $::ports($i) isMemoryDevice true
			set_interface_property $::ports($i) isNonVolatileStorage false
			set_interface_property $::ports($i) linewrapBursts false
			set_interface_property $::ports($i) printableDevice false
			set_interface_property $::ports($i) readWaitStates 0
			set_interface_property $::ports($i) readWaitTime 0
			set_interface_property $::ports($i) setupTime 0
			set_interface_property $::ports($i) timingUnits Cycles
			set_interface_property $::ports($i) writeWaitTime 0

			add_interface_port clock_$::ports($i) AS_$::ports_cap($i)_CLK clk Input 1
			add_interface_port clock_$::ports($i) AS_$::ports_cap($i)_RST reset Input 1

			add_interface_port $::ports($i) AS_$::ports_cap($i)_WAITREQ waitrequest Output 1
			add_interface_port $::ports($i) AS_$::ports_cap($i)_CS chipselect Input 1
			add_interface_port $::ports($i) AS_$::ports_cap($i)_RD read Input 1
			add_interface_port $::ports($i) AS_$::ports_cap($i)_WR write Input 1

			set num_bits [expr {pow(2, $mem_row_bits) * pow(2, $mem_column_bits) * pow(2, $mem_bank_bits) * $mem_data_width}]

			if { $mem_architecture == $::ARCH_SDR } {
				set avalon_data_width [expr $mem_data_width]
			} elseif { $mem_architecture != $::ARCH_SDR && $port_mode == $::MODE_BURST && $mem_data_width == 8 && $port_data_width == 32 } {
				set avalon_data_width 32
			} else {
				set avalon_data_width [expr $mem_data_width * 2]
			}

			set avalon_words [expr $num_bits / $avalon_data_width]
			set avalon_addr_width [expr round(ceil(log($avalon_words)/log(2)))]
			add_interface_port $::ports($i) AS_$::ports_cap($i)_DATA_IN writedata Input $avalon_data_width
			add_interface_port $::ports($i) AS_$::ports_cap($i)_DATA_OUT readdata Output $avalon_data_width

			if { $avalon_data_width > 8 } {
				add_interface_port $::ports($i) AS_$::ports_cap($i)_BE byteenable Input [expr $avalon_data_width/8]
			}
			add_interface_port $::ports($i) AS_$::ports_cap($i)_ADDR address Input $avalon_addr_width

			if { $port_mode == $::MODE_BURST } {
				set max_burst [get_parameter_value $::ports($i)_max_burst]
				set burst_width_temp [expr {log($max_burst+1)/log(2)}]
				set burst_width [expr {round(ceil(log($max_burst+1)/log(2)))}]

				set_interface_property $::ports($i) readLatency 0
				set_interface_property $::ports($i) maximumPendingReadTransactions 8
				add_interface_port $::ports($i) AS_$::ports_cap($i)_BURSTCOUNT burstcount Input $burst_width
				add_interface_port $::ports($i) AS_$::ports_cap($i)_DATAVALID readdatavalid Output 1

			} else {
				set_interface_property $::ports($i) readLatency 1
				set_interface_property $::ports($i) maximumPendingReadTransactions 0
			}
		}
	}

	add_interface_port sdram_interface CLK export Input 1
	add_interface_port sdram_interface CKE export Output 1
	add_interface_port sdram_interface CS export Output 1
	add_interface_port sdram_interface WE export Output 1
	add_interface_port sdram_interface CAS export Output 1
	add_interface_port sdram_interface RAS export Output 1

	add_interface_port sdram_interface BA export Output $mem_bank_bits
	add_interface_port sdram_interface A export Output $mem_row_bits

	add_interface_port sdram_interface DQ export Bidir $mem_data_width
	add_interface_port sdram_interface DQM export Output [expr $mem_data_width/8]

	if { $mem_architecture == $::ARCH_SDR} {
		add_interface_port sdram_interface CAPTURE_CLK export Input 1
	} else {
		add_interface_port sdram_interface WRITE_CLK export Input 1

		set mem_clocks [get_parameter_value mem_clocks]
		add_interface_port sdram_interface CLK_OUT_P export Output $mem_clocks
		add_interface_port sdram_interface CLK_OUT_N export Output $mem_clocks

		set mem_devices [get_parameter_value mem_devices]
		set mem_dqs [get_parameter_value mem_dqs]
		set mem_diff_dqs [get_parameter_value mem_diff_dqs]
		set mem_phase_shift [get_parameter_value mem_phase_shift]

		if { $mem_architecture == $::ARCH_DDR2 && $mem_diff_dqs == "true" } {
			add_interface_port sdram_interface DQS_N export Bidir [expr $mem_devices*$mem_dqs]
			add_interface_port sdram_interface DQS_P export Bidir [expr $mem_devices*$mem_dqs]
		} else {
			add_interface_port sdram_interface DQS export Bidir [expr $mem_devices*$mem_dqs]
		}

		if { $mem_phase_shift == "true" } {
			add_interface_port sdram_interface CMD_CLK export Input 1
		}

	}

	if { $mem_architecture == $::ARCH_DDR2} {
		add_interface_port sdram_interface ODT export Output 1
	}

}

proc mtx_avalon_sdram_generation {} {
	set mem_architecture [get_parameter_value mem_architecture]
	set device_family [get_project_property DEVICE_FAMILY_NAME]
	set outdir [get_generation_property OUTPUT_DIRECTORY]
	set outputname [get_generation_property OUTPUT_NAME]
	set project [get_project_property QUARTUS_PROJECT_NAME]

	# On windows, this perl exists:
	set qdr [ get_project_property QUARTUS_ROOTDIR ]
	set qdr [ string trimright $qdr / ]
	puts $qdr
	# Windows: use the version of perl which shipped with Quartus
	set perl $qdr/bin/perl/bin/perl.exe
	if { ! [ file executable $perl ] } {
		# If that didn't work, maybe perl can be found in the path:
		set perl "perl"
	}
	# Windows: use quartus_sh.exe
	set quartus_sh $qdr/bin/quartus_sh.exe
	if { ! [ file executable $quartus_sh ] } {
		# Try the Linux version
		set quartus_sh $qdr/bin/quartus_sh
	}

	set command_line [list $perl "mtx_avalon_sdram.pl" "${outdir}${outputname}.vhd" $outdir $outputname $quartus_sh $device_family $project]

	for { set i 0 } { $i < ($::PORT_P+1) } { incr i 1 } {
		set port_mode [get_parameter_value $::ports($i)_mode]
		set port_buffer_size [get_parameter_value $::ports($i)_buffer_size]
		set port_max_burst [get_parameter_value $::ports($i)_max_burst]
		set port_data_width [get_parameter_value $::ports($i)_data_width]
		set port_disable_cache [get_parameter_value $::ports($i)_disable_cache]

		lappend command_line $port_mode $port_buffer_size $port_max_burst $port_data_width $port_disable_cache
	}

	lappend command_line [get_parameter_value mem_architecture]
	lappend command_line [get_parameter_value mem_scheduler_parameter]
	lappend command_line [get_parameter_value mem_data_width]
	lappend command_line [get_parameter_value mem_bank_bits]
	lappend command_line [get_parameter_value mem_row_bits]
	lappend command_line [get_parameter_value mem_column_bits]
	lappend command_line [get_parameter_value mem_prefix]
	lappend command_line [get_parameter_value mem_devices]
	lappend command_line [get_parameter_value mem_dqs]
	lappend command_line [get_parameter_value mem_diff_dqs]
	lappend command_line [get_parameter_value mem_clocks]
	lappend command_line [get_parameter_value mem_reduce_drive_strength]
	lappend command_line [get_parameter_value mem_phase_shift]
	lappend command_line [get_parameter_value mem_deep_power_down]
	lappend command_line [get_parameter_value mem_odt]
	lappend command_line [get_parameter_value mem_disable_dedicated_ddr_blocks]
	lappend command_line [get_parameter_value mem_manual_scripts]

	lappend command_line [get_parameter_value timing_frequency]
	lappend command_line [get_parameter_value timing_tinit]
	lappend command_line [get_parameter_value timing_trp]
	lappend command_line [get_parameter_value timing_tmrd]
	lappend command_line [get_parameter_value timing_trfc]
	lappend command_line [get_parameter_value timing_trefi]
	lappend command_line [get_parameter_value timing_trcd]
	lappend command_line [get_parameter_value timing_twr]
	lappend command_line [get_parameter_value timing_tras]
	lappend command_line [get_parameter_value timing_trc]
	lappend command_line [get_parameter_value timing_tac]
	lappend command_line [get_parameter_value timing_toh]
	lappend command_line [get_parameter_value timing_pcb_delay]

	lappend command_line [get_parameter_value sdc_enable_derived]
	lappend command_line [get_parameter_value sdc_tck]
	lappend command_line [get_parameter_value sdc_tds]
	lappend command_line [get_parameter_value sdc_tdh]
	lappend command_line [get_parameter_value sdc_tis]
	lappend command_line [get_parameter_value sdc_tih]
	lappend command_line [get_parameter_value sdc_tdss]
	lappend command_line [get_parameter_value sdc_tdsh]
	lappend command_line [get_parameter_value sdc_tdqsq]
	lappend command_line [get_parameter_value sdc_tqhs]
	lappend command_line [get_parameter_value sdc_tdqsck]
	lappend command_line [get_parameter_value sdc_tdqss]
	lappend command_line [get_parameter_value sdc_input]

	# need to use eval so that each part of the command line is separate (not quoted together)
	eval exec $command_line

	# call assignment script created by generator
	set mem_manual_scripts [get_parameter_value mem_manual_scripts]
	if { $mem_manual_scripts == "false" } {
		set command_line "${quartus_sh} --no_banner -t ${outdir}${outputname}_assignments.tcl"
		eval exec $command_line
	}

	add_file ${outdir}${outputname}.vhd {SYNTHESIS SIMULATION}
}
