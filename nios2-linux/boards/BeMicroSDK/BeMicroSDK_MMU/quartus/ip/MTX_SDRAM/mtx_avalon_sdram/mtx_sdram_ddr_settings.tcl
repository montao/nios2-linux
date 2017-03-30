###
# Microtronix SDRAM Controller
# DDR location Assigments
# Build : 169/9.7.2010
###

###
# DDR Clock
# The clock atoms are placed two labs away from the I/O (same assignment as the DQ output cells
###
proc place_cii_clk_cells {pin_name hierarchy_name} {

	set panel {*Output Pins}
	set pin_id [get_report_panel_id $panel]

	set panel {*All Package Pins}
	set io_type_id [get_report_panel_id $panel]

	set column_name_x 	{X coordinate}
	set column_name_y 	{Y coordinate}
	set column_name_pin 	{Pin #}
	set io_type_name 	{I/O Type}

	set column_index_x	 	[get_report_panel_column_index -id $pin_id $column_name_x]
	set column_index_y	 	[get_report_panel_column_index -id $pin_id $column_name_y]
	set column_index_pin		[get_report_panel_column_index -id $pin_id $column_name_pin]
	set column_io_type_index	[get_report_panel_column_index -id $io_type_id $io_type_name]

	set x  	[get_report_panel_data -id $pin_id -row_name $pin_name -col $column_index_x]
	set y  	[get_report_panel_data -id $pin_id -row_name $pin_name -col $column_index_y]
	set pin	[get_report_panel_data -id $pin_id -row_name $pin_name -col $column_index_pin]
	set io 	[get_report_panel_data -id $io_type_id -row_name $pin -col $column_io_type_index]

	if { [string equal -nocase -length 6 $io column] == 1 } {
		if { $y == 0 } {
			set y [expr $y+2]
		} else {
			set y [expr $y-2]
		}
	} else {
		if { $x == 0 } {
			set x [expr $x+2]
		} else {
			set x [expr $x-2]
		}
	}

	set lab LAB_X$x\_Y$y

	set_location_assignment $lab -to "$hierarchy_name|mux";
	set_location_assignment $lab -to "$hierarchy_name|reg_high";	
	set_location_assignment $lab -to "$hierarchy_name|reg_low";
}


###
# DDR DQ
# The DQ input cells are placed adjecent to the I/O
# The DQ output cells are placed two lab away from the I/O 
###
proc place_cii_dq_cells {pin_name hierarchy_name pin_index} {

	set panel {*Bidir Pins}
	set pin_id [get_report_panel_id $panel]

	set panel {*All Package Pins}
	set io_type_id [get_report_panel_id $panel]

	set column_name_x 	{X coordinate}
	set column_name_y 	{Y coordinate}
	set column_name_pin 	{Pin #}
	set io_type_name 	{I/O Type}

	set column_index_x	 	[get_report_panel_column_index -id $pin_id $column_name_x]
	set column_index_y	 	[get_report_panel_column_index -id $pin_id $column_name_y]
	set column_index_pin		[get_report_panel_column_index -id $pin_id $column_name_pin]
	set column_io_type_index	[get_report_panel_column_index -id $io_type_id $io_type_name]

	#set name_ids [get_names -filter sdram_clk_out_n -node_type pin]
	#foreach_in_collection name_id $name_ids {

	#	set pin_name [get_name_info -info short_full_path $name_id]

		set x  	[get_report_panel_data -id $pin_id -row_name $pin_name -col $column_index_x]
		set y  	[get_report_panel_data -id $pin_id -row_name $pin_name -col $column_index_y]
		set pin	[get_report_panel_data -id $pin_id -row_name $pin_name -col $column_index_pin]
		set io 	[get_report_panel_data -id $io_type_id -row_name $pin -col $column_io_type_index]

		if { [string equal -nocase -length 6 $io column] == 1 } {
			if { $y == 0 } {
				set pin_location bottom
				incr y 1 
			} else {
				set pin_location top
				set y [expr $y-1]
			}
		} else {
			if { $x == 0 } {
				set pin_location left
				incr x 1
			} else {
				set pin_location right
				set x [expr $x-1]
			}
		}

		set lab LAB_X$x\_Y$y

		set_location_assignment $lab -to "$hierarchy_name|dq_reg_in_latch[$pin_index]";
		set_location_assignment $lab -to "$hierarchy_name|dq_reg_in_high1[$pin_index]";
		set_location_assignment $lab -to "$hierarchy_name|dq_reg_in_high2[$pin_index]";
		set_location_assignment $lab -to "$hierarchy_name|dq_reg_in_low[$pin_index]";

		if { [string equal $pin_location bottom] == 1 } {
			incr y 1
		} elseif { [string equal $pin_location top] == 1 } {
			set y [expr $y-1]
		} elseif { [string equal $pin_location left] == 1 } {
			incr x 1
		} else {
			set x [expr $x-1]
		}
		set lab LAB_X$x\_Y$y
		
		set_location_assignment $lab -to "$hierarchy_name|dq_mux[$pin_index]";
		set_location_assignment $lab -to "$hierarchy_name|dq_oe[$pin_index]";
		set_location_assignment $lab -to "$hierarchy_name|dq_reg_out_high[$pin_index]";
		set_location_assignment $lab -to "$hierarchy_name|dq_reg_out_low[$pin_index]";
	#}
}

###
# DDR DQS
# The dqs cells are placed two labs from the I/O
###
proc place_cii_dqs_cells {pin_name hierarchy_name} {

	set panel {*Bidir Pins}
	set pin_id [get_report_panel_id $panel]

	set panel {*All Package Pins}
	set io_type_id [get_report_panel_id $panel]

	set column_name_x 	{X coordinate}
	set column_name_y 	{Y coordinate}
	set column_name_pin 	{Pin #}
	set io_type_name 	{I/O Type}

	set column_index_x	 	[get_report_panel_column_index -id $pin_id $column_name_x]
	set column_index_y	 	[get_report_panel_column_index -id $pin_id $column_name_y]
	set column_index_pin		[get_report_panel_column_index -id $pin_id $column_name_pin]
	set column_io_type_index	[get_report_panel_column_index -id $io_type_id $io_type_name]

	#set name_ids [get_names -filter sdram_clk_out_n -node_type pin]
	#foreach_in_collection name_id $name_ids {

	#	set pin_name [get_name_info -info short_full_path $name_id]

		set x  	[get_report_panel_data -id $pin_id -row_name $pin_name -col $column_index_x]
		set y  	[get_report_panel_data -id $pin_id -row_name $pin_name -col $column_index_y]
		set pin	[get_report_panel_data -id $pin_id -row_name $pin_name -col $column_index_pin]
		set io 	[get_report_panel_data -id $io_type_id -row_name $pin -col $column_io_type_index]

		if { [string equal -nocase -length 6 $io column] == 1 } {
			if { $y == 0 } {
				set pin_location bottom
				set y [expr $y+2] 
			} else {
				set pin_location top
				set y [expr $y-2]
			}
		} else {
			if { $x == 0 } {
				set pin_location left
				set x [expr $x+2]
			} else {
				set pin_location right
				set x [expr $x-2]
			}
		}

		set lab LAB_X$x\_Y$y
		#puts "$pin_name : $lab\n"

		set_location_assignment $lab -to "$hierarchy_name|dqs_mux";
		set_location_assignment $lab -to "$hierarchy_name|dqs_oe";
		#set_location_assignment $lab -to "$hierarchy_name|dqs_reg_high";
		#set_location_assignment $lab -to "$hierarchy_name|dqs_reg_low";
	#}
}

###
# DDR DQM
# The dqm cells are placed two labs from the I/O
###
proc place_cii_dqm_cells {pin_name hierarchy_name} {

	set panel {*Output Pins}
	set pin_id [get_report_panel_id $panel]

	set panel {*All Package Pins}
	set io_type_id [get_report_panel_id $panel]

	set column_name_x 	{X coordinate}
	set column_name_y 	{Y coordinate}
	set column_name_pin 	{Pin #}
	set io_type_name 	{I/O Type}

	set column_index_x	 	[get_report_panel_column_index -id $pin_id $column_name_x]
	set column_index_y	 	[get_report_panel_column_index -id $pin_id $column_name_y]
	set column_index_pin		[get_report_panel_column_index -id $pin_id $column_name_pin]
	set column_io_type_index	[get_report_panel_column_index -id $io_type_id $io_type_name]

	set x  	[get_report_panel_data -id $pin_id -row_name $pin_name -col $column_index_x]
	set y  	[get_report_panel_data -id $pin_id -row_name $pin_name -col $column_index_y]
	set pin	[get_report_panel_data -id $pin_id -row_name $pin_name -col $column_index_pin]
	set io 	[get_report_panel_data -id $io_type_id -row_name $pin -col $column_io_type_index]

	if { [string equal -nocase -length 6 $io column] == 1 } {
		if { $y == 0 } {
			set y [expr $y+2] 
		} else {
			set y [expr $y-2]
		}
	} else {
		if { $x == 0 } {
			set x [expr $x+2]
		} else {
			set x [expr $x-2]
		}
	}

	set lab LAB_X$x\_Y$y

	set_location_assignment $lab -to "$hierarchy_name|mux";
	set_location_assignment $lab -to "$hierarchy_name|reg_high";	
	set_location_assignment $lab -to "$hierarchy_name|reg_low";
}

###
# Stratix II DDR DQ
# The DQ input cells are placed adjecent to the I/O
# The DQ output cells are placed in the I/O 
###
proc place_sii_dq_cells {pin_name hierarchy_name pin_index} {

	set panel {*Bidir Pins}
	set pin_id [get_report_panel_id $panel]

	set panel {*All Package Pins}
	set io_type_id [get_report_panel_id $panel]

	set column_name_x 	{X coordinate}
	set column_name_y 	{Y coordinate}
	set column_name_pin 	{Pin #}
	set io_type_name 	{I/O Type}

	set column_index_x	 	[get_report_panel_column_index -id $pin_id $column_name_x]
	set column_index_y	 	[get_report_panel_column_index -id $pin_id $column_name_y]
	set column_index_pin		[get_report_panel_column_index -id $pin_id $column_name_pin]
	set column_io_type_index	[get_report_panel_column_index -id $io_type_id $io_type_name]

	#set name_ids [get_names -filter sdram_clk_out_n -node_type pin]
	#foreach_in_collection name_id $name_ids {

	#	set pin_name [get_name_info -info short_full_path $name_id]

		set x  	[get_report_panel_data -id $pin_id -row_name $pin_name -col $column_index_x]
		set y  	[get_report_panel_data -id $pin_id -row_name $pin_name -col $column_index_y]
		set pin	[get_report_panel_data -id $pin_id -row_name $pin_name -col $column_index_pin]
		set io 	[get_report_panel_data -id $io_type_id -row_name $pin -col $column_io_type_index]

		if { [string equal -nocase -length 6 $io column] == 1 } {
			if { $y == 0 } {
				set pin_location bottom
				incr y 1 
			} else {
				set pin_location top
				set y [expr $y-1]
			}
		} else {
			if { $x == 0 } {
				set pin_location left
				incr x 1
			} else {
				set pin_location right
				set x [expr $x-1]
			}
		}

		set lab LAB_X$x\_Y$y

		set_location_assignment $lab -to "$hierarchy_name|dq_reg_in_latch[$pin_index]";
		set_location_assignment $lab -to "$hierarchy_name|dq_reg_in_high1[$pin_index]";
		set_location_assignment $lab -to "$hierarchy_name|dq_reg_in_high2[$pin_index]";
		set_location_assignment $lab -to "$hierarchy_name|dq_reg_in_low[$pin_index]";

		if { [string equal $pin_location bottom] == 1 } {
			incr y 1
		} elseif { [string equal $pin_location top] == 1 } {
			set y [expr $y-1]
		} elseif { [string equal $pin_location left] == 1 } {
			incr x 1
		} else {
			set x [expr $x-1]
		}
	#}
}

###
# Stratix II DQS
# The dqs clk active cells are placed two labs from the I/O
###
proc place_sii_dqs_cells {pin_name hierarchy_name} {

	set panel {*Bidir Pins}
	set pin_id [get_report_panel_id $panel]

	set panel {*All Package Pins}
	set io_type_id [get_report_panel_id $panel]

	set column_name_x 	{X coordinate}
	set column_name_y 	{Y coordinate}
	set column_name_pin 	{Pin #}
	set io_type_name 	{I/O Type}

	set column_index_x	 	[get_report_panel_column_index -id $pin_id $column_name_x]
	set column_index_y	 	[get_report_panel_column_index -id $pin_id $column_name_y]
	set column_index_pin		[get_report_panel_column_index -id $pin_id $column_name_pin]
	set column_io_type_index	[get_report_panel_column_index -id $io_type_id $io_type_name]

	#set name_ids [get_names -filter sdram_clk_out_n -node_type pin]
	#foreach_in_collection name_id $name_ids {

	#	set pin_name [get_name_info -info short_full_path $name_id]

		set x  	[get_report_panel_data -id $pin_id -row_name $pin_name -col $column_index_x]
		set y  	[get_report_panel_data -id $pin_id -row_name $pin_name -col $column_index_y]
		set pin	[get_report_panel_data -id $pin_id -row_name $pin_name -col $column_index_pin]
		set io 	[get_report_panel_data -id $io_type_id -row_name $pin -col $column_io_type_index]

		if { [string equal -nocase -length 6 $io column] == 1 } {
			if { $y == 0 } {
				set pin_location bottom
				set y [expr $y+2] 
			} else {
				set pin_location top
				set y [expr $y-2]
			}
		} else {
			if { $x == 0 } {
				set pin_location left
				set x [expr $x+2]
			} else {
				set pin_location right
				set x [expr $x-2]
			}
		}

		set lab LAB_X$x\_Y$y
		#puts "$pin_name : $lab\n"

		set_location_assignment $lab -to "$hierarchy_name|dqs_clk_active";
	#}
}


###
# Cyclone III DDR DQ
# The DQ input cells are placed adjecent to the I/O
# The DQ output cells are placed in the I/O 
###
proc place_ciii_dq_cells {pin_name hierarchy_name pin_index} {

	set panel {*Bidir Pins}
	set pin_id [get_report_panel_id $panel]

	set panel {*All Package Pins}
	set io_type_id [get_report_panel_id $panel]

	set column_name_x 	{X coordinate}
	set column_name_y 	{Y coordinate}
	set column_name_pin 	{Pin #}
	set io_type_name 	{I/O Type}

	set column_index_x	 	[get_report_panel_column_index -id $pin_id $column_name_x]
	set column_index_y	 	[get_report_panel_column_index -id $pin_id $column_name_y]
	set column_index_pin		[get_report_panel_column_index -id $pin_id $column_name_pin]
	set column_io_type_index	[get_report_panel_column_index -id $io_type_id $io_type_name]

	#set name_ids [get_names -filter sdram_clk_out_n -node_type pin]
	#foreach_in_collection name_id $name_ids {

	#	set pin_name [get_name_info -info short_full_path $name_id]

		set x  	[get_report_panel_data -id $pin_id -row_name $pin_name -col $column_index_x]
		set y  	[get_report_panel_data -id $pin_id -row_name $pin_name -col $column_index_y]
		set pin	[get_report_panel_data -id $pin_id -row_name $pin_name -col $column_index_pin]
		set io 	[get_report_panel_data -id $io_type_id -row_name $pin -col $column_io_type_index]

		if { [string equal -nocase -length 6 $io column] == 1 } {
			if { $y == 0 } {
				set pin_location bottom
				incr y 1 
			} else {
				set pin_location top
				set y [expr $y-1]
			}
		} else {
			if { $x == 0 } {
				set pin_location left
				incr x 1
			} else {
				set pin_location right
				set x [expr $x-1]
			}
		}

		set lab LAB_X$x\_Y$y

		set_location_assignment $lab -to "$hierarchy_name|dq_reg_in_latch[$pin_index]";
		set_location_assignment $lab -to "$hierarchy_name|dq_reg_in_high1[$pin_index]";
		set_location_assignment $lab -to "$hierarchy_name|dq_reg_in_high2[$pin_index]";
		set_location_assignment $lab -to "$hierarchy_name|dq_reg_in_low[$pin_index]";

		if { [string equal $pin_location bottom] == 1 } {
			incr y 1
		} elseif { [string equal $pin_location top] == 1 } {
			set y [expr $y-1]
		} elseif { [string equal $pin_location left] == 1 } {
			incr x 1
		} else {
			set x [expr $x-1]
		}
	#}
}

###
# Cyclone III DQS
# The dqs clk active cells are placed two labs from the I/O
###
proc place_ciii_dqs_cells {pin_name hierarchy_name} {

	set panel {*Bidir Pins}
	set pin_id [get_report_panel_id $panel]

	set panel {*All Package Pins}
	set io_type_id [get_report_panel_id $panel]

	set column_name_x 	{X coordinate}
	set column_name_y 	{Y coordinate}
	set column_name_pin 	{Pin #}
	set io_type_name 	{I/O Type}

	set column_index_x	 	[get_report_panel_column_index -id $pin_id $column_name_x]
	set column_index_y	 	[get_report_panel_column_index -id $pin_id $column_name_y]
	set column_index_pin		[get_report_panel_column_index -id $pin_id $column_name_pin]
	set column_io_type_index	[get_report_panel_column_index -id $io_type_id $io_type_name]

	#set name_ids [get_names -filter sdram_clk_out_n -node_type pin]
	#foreach_in_collection name_id $name_ids {

	#	set pin_name [get_name_info -info short_full_path $name_id]

		set x  	[get_report_panel_data -id $pin_id -row_name $pin_name -col $column_index_x]
		set y  	[get_report_panel_data -id $pin_id -row_name $pin_name -col $column_index_y]
		set pin	[get_report_panel_data -id $pin_id -row_name $pin_name -col $column_index_pin]
		set io 	[get_report_panel_data -id $io_type_id -row_name $pin -col $column_io_type_index]

		if { [string equal -nocase -length 6 $io column] == 1 } {
			if { $y == 0 } {
				set pin_location bottom
				set y [expr $y+2] 
			} else {
				set pin_location top
				set y [expr $y-2]
			}
		} else {
			if { $x == 0 } {
				set pin_location left
				set x [expr $x+2]
			} else {
				set pin_location right
				set x [expr $x-2]
			}
		}

		set lab LAB_X$x\_Y$y
		#puts "$pin_name : $lab\n"

		set_location_assignment $lab -to "$hierarchy_name|dqs_clk_active";
	#}
}

###
# Check valid pin name and find the hierarchy name
###
proc find_full_hierarchy { hierarchy pin } {

	set pin_name_id [get_names -filter $pin -node_type pin]
	set pin_name_found 0
	set hierarchy_name 0
	foreach_in_collection name_id $pin_name_id {
		set pin_name [get_name_info -info full_path $name_id]
		if { $pin_name == $pin } {
			set pin_name_found 1
		}
	}

	if {$pin_name_found} {
		set hierarchy_name_id [get_names -filter *$hierarchy -node_type hierarchy -observable_type all]
		foreach_in_collection name_id $hierarchy_name_id {
			# Get the full path name of the node
			set hierarchy_name [get_name_info -info full_path $name_id]
		}
	} else {
		set hierarchy_name -1
		post_message -type critical_warning "Unable to find sdram pin $pin. Please check the sdram pin names."
	}
return $hierarchy_name
}



