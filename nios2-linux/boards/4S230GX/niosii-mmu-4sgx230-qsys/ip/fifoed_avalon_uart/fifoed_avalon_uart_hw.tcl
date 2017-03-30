# +-----------------------------------
# |
# | fifoed_avalon_uart "FIFOed UART (RS-232 serial port)9.3" v9.3
# | null 2009.05.22.08:44:31
# |
# |
#  22 May 2009  CJR Added the ability to detect the clock frequency.
#  22 May 2009  CJR Added add_file in the generation phase.
#  26 May 2009  CJR Added timout functionality
#  13 May 2010  CJR Added timestamp and error in fifo functinality
#  16 Nov 2011  SRC Added console port option, changed name
#
# +-----------------------------------
proc validate {} {
#  if  { [get_parameter_value "hw_cts"] }  {
#     set_parameter_property use_cts_rts ENABLED false
#     set_parameter_value use_cts_rts false
#  } else {
#     set_parameter_property use_cts_rts ENABLED true
#  }

#  if  { [get_parameter_value "use_cts_rts"] }  {
#     set_parameter_property hw_cts ENABLED false
#  } else {
#     set_parameter_property hw_cts ENABLED true
#  }

  if  { [get_parameter_value "use_rx_fifo"] }  {
     set_parameter_property fifo_size_rx ENABLED true
     set_parameter_property fifo_export_used ENABLED true
     set_parameter_property rx_IRQ_Threshold ENABLED true
     set_parameter_property use_timout ENABLED true
     set_parameter_property use_gap_detection ENABLED true
     set_parameter_property gap_value ENABLED true
     set_parameter_property rx_fifo_LE ENABLED true
     set_parameter_property use_timestamp ENABLED true
     if  { [get_parameter_value "use_timestamp"] }  {
         set_parameter_property use_ext_timestamp ENABLED true
         set_parameter_property timestamp_width ENABLED true
         } else {
         set_parameter_property use_ext_timestamp ENABLED false
         set_parameter_property timestamp_width ENABLED false
     }
     set_parameter_property add_error_bits ENABLED true
     if  { [get_parameter_value "add_error_bits"] }  {
         set_parameter_property driver_pass_error_bits ENABLED true
         } else {
         set_parameter_property driver_pass_error_bits ENABLED false
     }
  } else {
     set_parameter_property fifo_size_rx ENABLED false
     set_parameter_property fifo_export_used ENABLED false
     set_parameter_property rx_IRQ_Threshold ENABLED false
     set_parameter_property use_timout ENABLED false
     set_parameter_property use_gap_detection ENABLED false
     set_parameter_property gap_value ENABLED false
     set_parameter_property rx_fifo_LE ENABLED false
     set_parameter_property use_timestamp ENABLED false
     set_parameter_property use_ext_timestamp ENABLED false
     set_parameter_property timestamp_width ENABLED false
     set_parameter_property add_error_bits ENABLED false
     set_parameter_property driver_pass_error_bits ENABLED false
 #    set_parameter_value use_timout false
  }

  if  { [get_parameter_value "use_tx_fifo"] }  {
     set_parameter_property fifo_size_tx ENABLED true
     set_parameter_property tx_IRQ_Threshold ENABLED true
  } else {
     set_parameter_property fifo_size_tx ENABLED false
     set_parameter_property tx_IRQ_Threshold ENABLED false
  }

  if  { [get_parameter_value "use_timout"] }  {
     set_parameter_property timeout_value ENABLED true
  } else {
     set_parameter_property timeout_value ENABLED false
  }

   # Get the current value of parameters we care about
  set rx [get_parameter_value fifo_size_rx]
  set tx [get_parameter_value fifo_size_tx]
  set rx_IRQ [get_parameter_value rx_IRQ_Threshold]
  set tx_IRQ [get_parameter_value tx_IRQ_Threshold]
  if { [get_parameter_value "use_rx_fifo"]  &&  [ expr $rx_IRQ > ($rx -1 ) ] } {
    send_message error "rx_IRQ_Threshold must be at least one less than rx fifo size"
    }
  if { [get_parameter_value "use_tx_fifo"]  &&  [ expr $tx_IRQ > ($tx -1 ) ] } {
    send_message error "tx_IRQ_Threshold must be at least one less than tx fifo size"
    }

  set_module_assignment "embeddedsw.CMacro.BAUDRATE" [get_parameter_value "baud"]
  set_module_assignment "embeddedsw.CMacro.DATA_BITS"  [get_parameter_value "data_bits"]
#  set_module_assignment "embeddedsw.CMacro.PARITY"  [get_parameter_value "parity"]
  set_module_assignment "embeddedsw.CMacro.FIXED_BAUD"  [string match {true} [get_parameter_value "fixed_baud"] ]
  set_module_assignment "embeddedsw.CMacro.STOP_BITS"  [get_parameter_value "stop_bits"]
  set_module_assignment "embeddedsw.CMacro.USE_CTS_RTS" [string match {true} [get_parameter_value "use_cts_rts"] ]
  set_module_assignment "embeddedsw.CMacro.USE_EOP_REGISTER"  [string match {true} [get_parameter_value "use_eop_register"] ]
  set_module_assignment "embeddedsw.CMacro.USE_TX_FIFO" [string match {true} [get_parameter_value "use_tx_fifo"] ]
  set_module_assignment "embeddedsw.CMacro.TX_FIFO_SIZE" [get_parameter_value "fifo_size_tx"]
  set_module_assignment "embeddedsw.CMacro.TX_FIFO_LE" [string match {true} [get_parameter_value "tx_fifo_LE"] ]
  set_module_assignment "embeddedsw.CMacro.TX_IRQ_THRESHOLD"  [get_parameter_value "tx_IRQ_Threshold"]
  set_module_assignment "embeddedsw.CMacro.USE_RX_FIFO" [string match {true} [get_parameter_value "use_rx_fifo"] ]
  set_module_assignment "embeddedsw.CMacro.RX_FIFO_SIZE" [get_parameter_value "fifo_size_rx"]
  set_module_assignment "embeddedsw.CMacro.RX_FIFO_LE" [string match {true} [get_parameter_value "rx_fifo_LE"] ]
  set_module_assignment "embeddedsw.CMacro.RX_IRQ_THRESHOLD"  [get_parameter_value "rx_IRQ_Threshold"]
  set_module_assignment "embeddedsw.CMacro.USE_TIMESTAMP" [string match {true} [get_parameter_value "use_timestamp"] ]
  set_module_assignment "embeddedsw.CMacro.ADD_ERROR_BITS" [string match {true} [get_parameter_value "add_error_bits"] ]
  set_module_assignment "embeddedsw.CMacro.FIFO_EXPORT_USED" [string match {true} [get_parameter_value "fifo_export_used"] ]
  set_module_assignment "embeddedsw.CMacro.HAS_IRQ" [string match {true} [get_parameter_value "Has_IRQ"] ]
  set_module_assignment "embeddedsw.CMacro.UHW_CTS" [string match {true} [get_parameter_value "hw_cts"] ]
  set_module_assignment "embeddedsw.CMacro.TRANSMIT_PIN" [string match {true} [get_parameter_value "trans_pin"] ]
  set_module_assignment "embeddedsw.CMacro.CONSOLE_PORT" [string match {true} [get_parameter_value "console_port"] ]
  set_module_assignment "embeddedsw.CMacro.USE_RX_TIMEOUT" [string match {true} [get_parameter_value "use_timout"] ]
  set_module_assignment "embeddedsw.CMacro.TIMEOUT_VALUE"  [get_parameter_value "timeout_value"]
  set_module_assignment "embeddedsw.CMacro.USE_GAP_DETECTION" [string match {true} [get_parameter_value "use_gap_detection"] ]
  set_module_assignment "embeddedsw.CMacro.GAP_VALUE"  [get_parameter_value "gap_value"]
  set_module_assignment "embeddedsw.CMacro.USE_EXT_TIMESTAMP" [string match {true} [get_parameter_value "use_ext_timestamp"] ]
  set_module_assignment "embeddedsw.CMacro.TIMESTAMP_WIDTH"  [get_parameter_value "timestamp_width"]
  set_module_assignment "embeddedsw.CMacro.ADD_ERROR_BITS" [string match {true} [get_parameter_value "add_error_bits"] ]
  set_module_assignment "embeddedsw.CMacro.PASS_ERROR_BITS" [string match {true} [get_parameter_value "driver_pass_error_bits"] ]

 #DTS Stuff
# set_module_assignment embeddedsw.dts.vendor "ALTR"
# set_module_assignment embeddedsw.dts.name "uart"
# set_module_assignment embeddedsw.dts.group "serial"
# set_module_assignment embeddedsw.dts.compatible "uart-11.0,uart-1.0"
## set_module_assignment embeddedsw.dts.compatible "uart-1.0"
# set_module_assignment embeddedsw.dts.param.current-speed [get_parameter_value "baud"]
# set_module_assignment embeddedsw.dts.param.clock-frequency ->clk/s1_clock
# set_module_assignment embeddedsw.dts.param.console [get_parameter_value "console_port"]

}


proc elaborate {} {
# +-----------------------------------
# | connection point s1_clock
# |
#add_interface s1_clock clock end
#set_interface_property s1_clock ptfSchematicName ""
#
#add_interface_port s1_clock clk clk Input 1
#add_interface_port s1_clock reset_n reset_n Input 1
# |
# +-----------------------------------

# +-----------------------------------
# | connection point global
# |
add_interface g1 conduit end

add_interface_port g1 txd export Output 1
add_interface_port g1 rxd export Input 1
# |
# +-----------------------------------

# +-----------------------------------
# | connection point s1
# |
add_interface s1 avalon end
set_interface_property s1 addressAlignment NATIVE
set_interface_property s1 addressSpan 16
set_interface_property s1 bridgesToMaster ""
set_interface_property s1 burstOnBurstBoundariesOnly false
set_interface_property s1 holdTime 0
set_interface_property s1 isMemoryDevice false
set_interface_property s1 isNonVolatileStorage false
set_interface_property s1 linewrapBursts false
set_interface_property s1 maximumPendingReadTransactions 0
set_interface_property s1 minimumUninterruptedRunLength 1
set_interface_property s1 printableDevice true
set_interface_property s1 readLatency 0
set_interface_property s1 readWaitTime 1
set_interface_property s1 setupTime 0
set_interface_property s1 timingUnits Cycles
set_interface_property s1 writeWaitStates 1
set_interface_property s1 writeWaitTime 1

set_interface_property s1 ASSOCIATED_CLOCK s1_clock

add_interface_port s1 address address Input 4
add_interface_port s1 begintransfer begintransfer Input 1
add_interface_port s1 chipselect chipselect Input 1
add_interface_port s1 read_n read_n Input 1
add_interface_port s1 write_n write_n Input 1
add_interface_port s1 writedata writedata Input 32
#add_interface_port s1 dataavailable dataavailable Output 1
add_interface_port s1 readdata readdata Output 32
#add_interface_port s1 readyfordata readyfordata Output 1
# |
# +-----------------------------------

# +-----------------------------------
# | connection point s1_irq
# |
if  { [get_parameter_value "Has_IRQ"] }  {
    add_interface s1_irq interrupt end
    set_interface_property s1_irq associatedAddressablePoint s1
    set_interface_property s1_irq ASSOCIATED_CLOCK s1_clock
    add_interface_port s1_irq irq irq Output 1
    } else {
    add_interface_port g1 irqexport export Output 1
    }
if  { [get_parameter_value "fifo_export_used"] }  {
#expr log(1000)/log(10)
     set tx_fifo_address_bits [expr {int( 1 + (log ( [ get_parameter_value "fifo_size_tx" ] ) / log(2) ) ) } ]
     set rx_fifo_address_bits [expr {int( 1 + (log ( [ get_parameter_value "fifo_size_rx" ] ) / log(2) ) ) } ]
     add_interface_port g1 rxused export Output $rx_fifo_address_bits
     add_interface_port g1 txused export Output  $tx_fifo_address_bits
}
if { [get_parameter_value "use_cts_rts"]  } {
     add_interface_port g1 cts_n export Input 1
     add_interface_port g1 rts_n export Output 1
}

if {  [get_parameter_value "hw_cts"] } {
     add_interface_port g1 cts_n export Input 1
}
if  { [get_parameter_value "trans_pin"] }   {
     add_interface_port g1 transmitting export Output 1
}
if { [get_parameter_value "use_timestamp"] && [get_parameter_value "use_ext_timestamp"] }  {
     add_interface_port g1 timestamp_in export Input [get_parameter_value "timestamp_width"]
}

# |
# +-----------------------------------
 }

 proc generate {} {
 send_message info "Starting FIFOed UART Generation"
 set sim_dir [ get_generation_property SIMULATION_DIRECTORY ]
  set outdir [ get_generation_property OUTPUT_DIRECTORY ]
  set output_name [ get_generation_property OUTPUT_NAME ]
  set language [ get_generation_property HDL_LANGUAGE ]

#  set clock_freq  [get_parameter_property s1_clock clock_rate]
#  set_parameter_property clock_freq SYSTEM_INFO {CLOCK_RATE <my_clk>}
  #set clock_rate [get_interface_property s1_clock clock_rate]

#  set clock_freq 64000000
  set  clock_freq  [get_parameter_value "clock_freq"]
  set  use_tx_fifo [string match {true} [get_parameter_value "use_tx_fifo"] ]
  set  use_rx_fifo [string match {true} [get_parameter_value "use_rx_fifo"] ]
  set  baud [get_parameter_value "baud"]
  set  data_bits [get_parameter_value "data_bits"]
  set  fixed_baud [string match {true} [get_parameter_value "fixed_baud"]  ]
  set  parity [get_parameter_value "parity"]
  set  stop_bits [get_parameter_value "stop_bits"]
  set  use_cts_rts [string match {true} [get_parameter_value "use_cts_rts"] ]
  set  use_eop_register [string match {true} [get_parameter_value "use_eop_register"] ]
  set  sim_true_baud [string match {true} [get_parameter_value "sim_true_baud"] ]
  set  sim_char_stream [get_parameter_value "sim_char_stream"]
 # set sim_char_file "\"\""
  # now make a file out of it
  # check to see if simulation directory is assigned if not. then nothing to do.
   if  { [string length $sim_dir ] } {
#   puts "|$sim_dir|"
     set sim_char_end "_char_file.txt"
     set sim_char_file  "$sim_dir/$output_name$sim_char_end"
     set fileid [open $sim_char_file w ]
     puts $fileid $sim_char_stream
#     puts $fileid "|$sim_dir|"
     close $fileid
     set generate_sim 1
   } else {
#    puts "zero|$sim_dir|"
   #nothnig to be done.
   set sim_dir ""
   set generate_sim 0
   # zero the file name
  }
#  set sim_dir "./"
#  set  sim_char_stream "fred"
  set  fifo_export_used [string match {true} [get_parameter_value "fifo_export_used"] ]
  set  Has_IRQ [string match {true} [get_parameter_value "Has_IRQ"] ]
  set  hw_cts [string match {true} [get_parameter_value "hw_cts"]]
  set  trans_pin [string match {true} [get_parameter_value "trans_pin"] ]
  set  console_port [string match {true} [get_parameter_value "console_port"] ]
  set  fifo_size_tx [get_parameter_value "fifo_size_tx"]
  set  fifo_size_rx [get_parameter_value "fifo_size_rx"]
  set  use_timout [string match {true} [get_parameter_value "use_timout"] ]
   if  { [string match {false} [get_parameter_value "use_rx_fifo"] ] } {
   set use_timout 0
   };
  set  timeout_value [get_parameter_value "timeout_value"]
  set  rx_IRQ_Threshold [get_parameter_value "rx_IRQ_Threshold"]
  set  tx_IRQ_Threshold [get_parameter_value "tx_IRQ_Threshold"]
  set  rx_fifo_LE [string match {true} [get_parameter_value "rx_fifo_LE"] ]
  set  tx_fifo_LE [string match {true} [get_parameter_value "tx_fifo_LE"] ]
  set gap_value [get_parameter_value "gap_value"]
  set  use_gap_detection [string match {true} [get_parameter_value "use_gap_detection"] ]
   if  { [string match {false} [get_parameter_value "use_rx_fifo"] ] } {
      set use_gap_detection 0 };
  set use_timestamp [string match {true} [get_parameter_value "use_timestamp"] ]
  set use_ext_timestamp [string match {true} [get_parameter_value "use_ext_timestamp"] ]
  set timestamp_width  [get_parameter_value "timestamp_width"]
  set add_error_bits  [string match {true} [get_parameter_value "add_error_bits"] ]



  # On windows, this perl exists:
  set qdr [ get_project_property QUARTUS_ROOTDIR ]
#  puts $qdr
  set sopc_builder_bin_dir "$qdr/sopc_builder/bin/"
#  puts $sopc_builder_bin_dir
  set europa_dir "$sopc_builder_bin_dir/europa/"
#  puts $europa_dir
#  set perllib_dir "$sopc_builder_bin_dir/perl_lib/"
  set perllib_dir "$qdr/common/ip/altera/common/perl/5.8.3/"
#  puts $perllib_dir
  # Windows: use the version of perl which shipped with Quartus
  set perl $qdr/bin/perl/bin/perl.exe
  if { ! [ file executable $perl ] } {
    # If that didn<92>t work, maybe perl can be found in the path:
    #linux?
    set perl $qdr/linux/perl/bin/perl
  }
  if { ! [ file executable $perl ] } {
    # If that didn<92>t work, maybe perl can be found in the path:
    set perl "perl"
  }
 # set generate_sim 0
#  puts $perl
      set exec_list [ list \
      exec $perl \
      -I . \
      -I $sopc_builder_bin_dir \
      -I $europa_dir \
      -I $perllib_dir \
      mk_em_uart_sa.pl \
      --_system_directory=$outdir \
      --language=$language \
      --generate_sim=$generate_sim \
      --name=$output_name \
      --clock_freq=$clock_freq \
      --use_tx_fifo=$use_tx_fifo \
      --use_rx_fifo=$use_rx_fifo \
      --baud=$baud \
      --data_bits=$data_bits \
      --fixed_baud=$fixed_baud \
      --parity=$parity \
      --stop_bits=$stop_bits \
      --use_cts_rts=$use_cts_rts \
      --use_eop_register=$use_eop_register \
      --sim_true_baud=$sim_true_baud \
      --fifo_export_used=$fifo_export_used \
      --Has_IRQ=$Has_IRQ \
      --hw_cts=$hw_cts \
      --trans_pin=$trans_pin \
      --fifo_size_tx=$fifo_size_tx \
      --fifo_size_rx=$fifo_size_rx \
      --use_timout=$use_timout \
      --timeout_value=$timeout_value \
      --rx_IRQ_Threshold=$rx_IRQ_Threshold \
      --tx_IRQ_Threshold=$tx_IRQ_Threshold \
      --tx_fifo_LE=$tx_fifo_LE \
      --rx_fifo_LE=$rx_fifo_LE \
      --use_gap_detection=$use_gap_detection \
      --gap_value=$gap_value \
      --use_timestamp=$use_timestamp \
      --use_ext_timestamp=$use_ext_timestamp \
      --timestamp_width=$timestamp_width  \
      --add_error_bits=$add_error_bits  \
  ]

  if {$generate_sim} {
  lappend  exec_list "--sim_directory=$sim_dir"
  lappend  exec_list "--sim_char_file=$sim_char_file"
  }
 send_message info $exec_list
 eval $exec_list
#set script "$filename"
#eval $script
# i removed the sim
#       --sim_char_stream=$sim_char_stream \
# line becuase it was causing me problems.
 if { [string match {verilog} $language ] }  {
   add_file $outdir/$output_name.v {SIMULATION SYNTHESIS }
 } else {
   add_file $outdir/$output_name.vhd {SYNTHESIS SIMULATION}
 }
 if { $generate_sim } {
   add_file $sim_char_file {SIMULATION}
   add_file ftail-f.pl {SIMULATION}
   add_file fuart.pl {SIMULATION}
   add_file {$sim_dir}{$name}"_log_module.txt"  {SIMULATION}
   add_file {$sim_dir}{$name}"_log.bat"    {SIMULATION}
   add file {$sim_dir}{$name}"_input_data_stream.dat" {SIMULATION}
   add file {$sim_dir}{$name}"_input_data_mutex.dat" {SIMULATION}

  }
}
 # +-----------------------------------
# | module fifoed_avalon_uart
# |



set_module_property NAME fifoed_avalon_uart
set_module_property VERSION 11.1
set_module_property GROUP "Interface Protocols/Serial"
set_module_property DISPLAY_NAME "FIFOed UART (RS-232 serial port)11.0"
set_module_property DATASHEET_URL http://www.altera.com/literature/hb/nios2/n2cpu_nii51010.pdf
#set_module_property INSTANTIATE_IN_SYSTEM_MODULE false
#set_module_property EDITABLE false
set_module_property VALIDATION_CALLBACK validate
set_module_property ELABORATION_CALLBACK elaborate
set_module_property GENERATION_CALLBACK generate

# add_file ftail-f.pl {SIMULATION}
#   add_file fuart.pl {SIMULATION}
# +-----------------------------------
# | parameters
# |
add_parameter clock_freq INTEGER
set_parameter_property clock_freq SYSTEM_INFO {CLOCK_RATE "s1_clock"}

add_parameter baud INTEGER 115200 ""
set_parameter_property baud DISPLAY_NAME "Baud Rate (bps): "
set_parameter_property baud UNITS None
set_parameter_property baud ALLOWED_RANGES {5000000 2000000 921600 460800 230400 115200 57600 38400 31250 28800 19200 14400 9600 4800 2400 1200 300}
set_parameter_property baud GROUP "Configuration/Baud Rate"
set_parameter_property baud DESCRIPTION ""
set_parameter_property baud AFFECTS_ELABORATION true

add_parameter data_bits INTEGER 8 ""
set_parameter_property data_bits DISPLAY_NAME "data bits"
set_parameter_property data_bits UNITS None
set_parameter_property data_bits ALLOWED_RANGES {7 8 9 10}
set_parameter_property data_bits GROUP Configuration
set_parameter_property data_bits DESCRIPTION ""
set_parameter_property data_bits AFFECTS_ELABORATION true

add_parameter fixed_baud BOOLEAN true ""
set_parameter_property fixed_baud DISPLAY_NAME "Fixed baud rate ( no software control)"
set_parameter_property fixed_baud UNITS None
set_parameter_property fixed_baud GROUP "Configuration/Baud Rate"
set_parameter_property fixed_baud DESCRIPTION ""
set_parameter_property fixed_baud AFFECTS_ELABORATION true

add_parameter parity STRING N ""
set_parameter_property parity DISPLAY_NAME parity
set_parameter_property parity UNITS None
set_parameter_property parity ALLOWED_RANGES {N:None E:Even O:Odd}
set_parameter_property parity GROUP Configuration
set_parameter_property parity DESCRIPTION ""
set_parameter_property parity AFFECTS_ELABORATION true

add_parameter stop_bits INTEGER 1 ""
set_parameter_property stop_bits DISPLAY_NAME "stop bits"
set_parameter_property stop_bits UNITS None
set_parameter_property stop_bits ALLOWED_RANGES {1 2}
set_parameter_property stop_bits GROUP Configuration
set_parameter_property stop_bits DESCRIPTION ""
set_parameter_property stop_bits AFFECTS_ELABORATION true

add_parameter use_cts_rts BOOLEAN false ""
set_parameter_property use_cts_rts DISPLAY_NAME "Include CTS/RTS pins and control register bits"
set_parameter_property use_cts_rts UNITS None
set_parameter_property use_cts_rts GROUP "Configuration/Flow Control"
set_parameter_property use_cts_rts DESCRIPTION ""
set_parameter_property use_cts_rts AFFECTS_ELABORATION true

add_parameter use_eop_register BOOLEAN false "UART will automatically detect an end-of-packet character <br> and terminate a streaming (DMA) transfer."
set_parameter_property use_eop_register DISPLAY_NAME "Include end-of-packet register"
set_parameter_property use_eop_register UNITS None
set_parameter_property use_eop_register GROUP "Configuration/Streaming Data (DMA) control"
set_parameter_property use_eop_register DESCRIPTION "UART will automatically detect an end-of-packet character <br> and terminate a streaming (DMA) transfer."
set_parameter_property use_eop_register AFFECTS_ELABORATION true

add_parameter sim_true_baud INTEGER 0 ""
set_parameter_property sim_true_baud DISPLAY_NAME "Simulated transmitter Baud Rate"
set_parameter_property sim_true_baud UNITS None
set_parameter_property sim_true_baud ALLOWED_RANGES {"0:accelerated (use divisor = 2)" "1:actual (use true baud divisor)"}
set_parameter_property sim_true_baud GROUP Simulation
set_parameter_property sim_true_baud DESCRIPTION ""
set_parameter_property sim_true_baud AFFECTS_ELABORATION true

add_parameter sim_char_stream STRING "DEADBEEF" ""
set_parameter_property sim_char_stream DISPLAY_NAME "Simulated RXD-input character stream"
set_parameter_property sim_char_stream UNITS None
set_parameter_property sim_char_stream GROUP Simulation
set_parameter_property sim_char_stream DESCRIPTION ""
set_parameter_property sim_char_stream AFFECTS_ELABORATION true

add_parameter use_tx_fifo BOOLEAN false ""
set_parameter_property use_tx_fifo DISPLAY_NAME "Include transmit FIFOs"
set_parameter_property use_tx_fifo UNITS None
set_parameter_property use_tx_fifo GROUP "TX FIFO usage"
set_parameter_property use_tx_fifo DESCRIPTION ""
set_parameter_property use_tx_fifo AFFECTS_ELABORATION true

add_parameter fifo_size_tx INTEGER 8 ""
set_parameter_property fifo_size_tx DISPLAY_NAME "TX FIFO depth (words): "
set_parameter_property fifo_size_tx UNITS None
set_parameter_property fifo_size_tx ALLOWED_RANGES {2 4 8 16 32 64 128 256 512 1024 2048 4096 8192}
set_parameter_property fifo_size_tx GROUP "TX FIFO usage"
set_parameter_property fifo_size_tx DESCRIPTION ""
set_parameter_property fifo_size_tx AFFECTS_ELABORATION true

add_parameter tx_fifo_LE BOOLEAN false ""
set_parameter_property tx_fifo_LE DISPLAY_NAME "Build FIFOs from LEs"
set_parameter_property tx_fifo_LE UNITS None
set_parameter_property tx_fifo_LE GROUP "TX FIFO usage"
set_parameter_property tx_fifo_LE DESCRIPTION ""
set_parameter_property tx_fifo_LE AFFECTS_ELABORATION false



add_parameter tx_IRQ_Threshold INTEGER 1 ""
set_parameter_property tx_IRQ_Threshold DISPLAY_NAME "TX IRQ Threshold (words): "
set_parameter_property tx_IRQ_Threshold UNITS None
set_parameter_property tx_IRQ_Threshold ALLOWED_RANGES {1:8191}
set_parameter_property tx_IRQ_Threshold GROUP "TX FIFO usage"
set_parameter_property tx_IRQ_Threshold DESCRIPTION ""
set_parameter_property tx_IRQ_Threshold AFFECTS_ELABORATION true

add_parameter use_rx_fifo BOOLEAN false ""
set_parameter_property use_rx_fifo DISPLAY_NAME "Include receive  FIFOs"
set_parameter_property use_rx_fifo UNITS None
set_parameter_property use_rx_fifo GROUP "RX FIFO usage"
set_parameter_property use_rx_fifo DESCRIPTION ""
set_parameter_property use_rx_fifo AFFECTS_ELABORATION true

add_parameter fifo_size_rx INTEGER 8 ""
set_parameter_property fifo_size_rx DISPLAY_NAME "RX FIFO depth (words): "
set_parameter_property fifo_size_rx UNITS None
set_parameter_property fifo_size_rx ALLOWED_RANGES {2 4 8 16 32 64 128 256 512 1024 2048 4096 8192}
set_parameter_property fifo_size_rx GROUP "RX FIFO usage"
set_parameter_property fifo_size_rx DESCRIPTION ""
set_parameter_property fifo_size_rx AFFECTS_ELABORATION true

add_parameter rx_fifo_LE BOOLEAN false ""
set_parameter_property rx_fifo_LE DISPLAY_NAME "Build FIFOs from LEs"
set_parameter_property rx_fifo_LE UNITS None
set_parameter_property rx_fifo_LE GROUP "RX FIFO usage"
set_parameter_property rx_fifo_LE DESCRIPTION ""
set_parameter_property rx_fifo_LE AFFECTS_ELABORATION false

add_parameter rx_IRQ_Threshold INTEGER 1 ""
set_parameter_property rx_IRQ_Threshold DISPLAY_NAME "RX IRQ Threshold (words): "
set_parameter_property rx_IRQ_Threshold UNITS None
set_parameter_property rx_IRQ_Threshold ALLOWED_RANGES {1:8191}
set_parameter_property rx_IRQ_Threshold GROUP "RX FIFO usage"
set_parameter_property rx_IRQ_Threshold DESCRIPTION ""
set_parameter_property rx_IRQ_Threshold AFFECTS_ELABORATION true


add_parameter fifo_export_used BOOLEAN false ""
set_parameter_property fifo_export_used DISPLAY_NAME "Export FIFO used signals "
set_parameter_property fifo_export_used UNITS None
set_parameter_property fifo_export_used GROUP "FIFO Exorts"
set_parameter_property fifo_export_used DESCRIPTION ""
set_parameter_property fifo_export_used AFFECTS_ELABORATION true

add_parameter Has_IRQ BOOLEAN true ""
set_parameter_property Has_IRQ DISPLAY_NAME "Connect the IRQ to the avalon fabric"
set_parameter_property Has_IRQ UNITS None
set_parameter_property Has_IRQ GROUP "MISC"
set_parameter_property Has_IRQ DESCRIPTION "This should be normally checked"
set_parameter_property Has_IRQ AFFECTS_ELABORATION true

add_parameter hw_cts BOOLEAN false ""
set_parameter_property hw_cts DISPLAY_NAME "Create hardware CTS input ( only valid with fifos) "
set_parameter_property hw_cts UNITS None
set_parameter_property hw_cts GROUP "MISC"
set_parameter_property hw_cts DESCRIPTION ""
set_parameter_property hw_cts AFFECTS_ELABORATION true

add_parameter trans_pin BOOLEAN false ""
set_parameter_property trans_pin DISPLAY_NAME "Create hardware which asserts only when uart is transmiting.  usefull for RS485"
set_parameter_property trans_pin UNITS None
set_parameter_property trans_pin GROUP "MISC"
set_parameter_property trans_pin DESCRIPTION ""
set_parameter_property trans_pin AFFECTS_ELABORATION true

add_parameter console_port BOOLEAN false ""
set_parameter_property console_port DISPLAY_NAME "Use for console port"
set_parameter_property console_port UNITS None
set_parameter_property console_port GROUP "MISC"
set_parameter_property console_port DESCRIPTION ""
set_parameter_property console_port AFFECTS_ELABORATION true

add_parameter use_timout BOOLEAN false "If character in the input fifo but no other chars have been recieve for the period described below the interrupt will fire"
set_parameter_property use_timout DISPLAY_NAME "Enable Rx Timeout "
set_parameter_property use_timout UNITS None
set_parameter_property use_timout GROUP "RX FIFO usage"
set_parameter_property use_timout DESCRIPTION ""
set_parameter_property use_timout AFFECTS_ELABORATION false

add_parameter timeout_value INTEGER 4 "IF 4 was select and there were a char in the rx fifo and no new char has been in 4 characters periods then the rx_interrupt will fire"
set_parameter_property timeout_value DISPLAY_NAME "Timeout in Character periods."
set_parameter_property timeout_value UNITS None
#conter is only 5 bits so can't go larger than 31
set_parameter_property timeout_value ALLOWED_RANGES {1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31}
set_parameter_property timeout_value GROUP "RX FIFO usage"
set_parameter_property timeout_value DESCRIPTION ""
set_parameter_property timeout_value AFFECTS_ELABORATION false


add_parameter use_gap_detection BOOLEAN false "If no chars have been recieve for the period described below the interrupt will fire"
set_parameter_property use_gap_detection DISPLAY_NAME "Enable Rx Gap detection "
set_parameter_property use_gap_detection UNITS None
set_parameter_property use_gap_detection GROUP "RX FIFO usage"
set_parameter_property use_gap_detection DESCRIPTION ""
set_parameter_property use_gap_detection AFFECTS_ELABORATION false

add_parameter gap_value INTEGER 4 "IF 4 was select and there were nocharacters recieved for  4 characters periods then the rx_gap irq will fire"
set_parameter_property gap_value DISPLAY_NAME "    Timeout in Character periods."
set_parameter_property gap_value UNITS None
#conter is only 5 bits so can't go larger than 31
set_parameter_property gap_value ALLOWED_RANGES {1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31}
set_parameter_property gap_value GROUP "RX FIFO usage"
set_parameter_property gap_value DESCRIPTION ""
set_parameter_property gap_value AFFECTS_ELABORATION false

# time stamp stuff
add_parameter use_timestamp BOOLEAN false "FIFO will be added to accomodate the extra data."
set_parameter_property use_timestamp DISPLAY_NAME "Enable Timestamp register"
set_parameter_property use_timestamp UNITS None
set_parameter_property use_timestamp GROUP "RX FIFO usage"
set_parameter_property use_timestamp DESCRIPTION ""
set_parameter_property use_timestamp AFFECTS_ELABORATION true

add_parameter use_ext_timestamp BOOLEAN false "Siganls will be exported to allow user logic to drive the fifo input"
set_parameter_property use_ext_timestamp DISPLAY_NAME "    Use external logic for timestamp. Internal is  the baudrate/8"
set_parameter_property use_ext_timestamp UNITS None
set_parameter_property use_ext_timestamp GROUP "RX FIFO usage"
set_parameter_property use_ext_timestamp DESCRIPTION ""
set_parameter_property use_ext_timestamp AFFECTS_ELABORATION true

add_parameter timestamp_width INTEGER 8 "Width of the counter and fifo associated with the timestamp"
set_parameter_property timestamp_width DISPLAY_NAME "     Width of timestamp fifo and register."
set_parameter_property timestamp_width UNITS None
set_parameter_property timestamp_width ALLOWED_RANGES {8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31}
set_parameter_property timestamp_width GROUP "RX FIFO usage"
set_parameter_property timestamp_width DESCRIPTION ""
set_parameter_property timestamp_width AFFECTS_ELABORATION true

#error bits in fifo
add_parameter add_error_bits BOOLEAN false "Theses will be bits 10-13. "
set_parameter_property add_error_bits DISPLAY_NAME "Enable PE, FE, BRK, ROE, GAP, RxEMPTY fifo data"
set_parameter_property add_error_bits UNITS None
set_parameter_property add_error_bits GROUP "RX FIFO usage"
set_parameter_property add_error_bits DESCRIPTION ""
set_parameter_property add_error_bits AFFECTS_ELABORATION false

add_parameter driver_pass_error_bits BOOLEAN false "Allows your local code to interpret these bits and driver will nothing with them."
set_parameter_property driver_pass_error_bits DISPLAY_NAME "     Driver: Pass error bits with data bits."
set_parameter_property driver_pass_error_bits UNITS None
set_parameter_property driver_pass_error_bits GROUP "RX FIFO usage"
set_parameter_property driver_pass_error_bits DESCRIPTION ""
set_parameter_property driver_pass_error_bits AFFECTS_ELABORATION false



add_interface s1_clock clock end
add_interface_port s1_clock clk clk Input 1

add_interface reset reset end
set_interface_property reset associatedClock s1_clock
set_interface_property reset synchronousEdges DEASSERT
add_interface_port reset reset_n reset_n Input 1
