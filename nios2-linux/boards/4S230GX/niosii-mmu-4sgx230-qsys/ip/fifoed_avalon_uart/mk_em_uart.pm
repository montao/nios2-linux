#Copyright (C)2001-2009 Altera Corporation
#Any megafunction design, and related net list (encrypted or decrypted),
#support information, device programming or simulation file, and any other
#associated documentation or information provided by Altera or a partner
#under Altera's Megafunction Partnership Program may be used only to
#program PLD devices (but not masked PLD devices) from Altera.  Any other
#use of such megafunction design, net list, support information, device
#programming or simulation file, or any other related documentation or
#information is prohibited for any other purpose, including, but not
#limited to modification, reverse engineering, de-compiling, or use with
#any other silicon devices, unless such use is explicitly licensed under
#a separate agreement with Altera or a megafunction partner.  Title to
#the intellectual property, including patents, copyrights, trademarks,
#trade secrets, or maskworks, embodied in any such megafunction design,
#net list, support information, device programming or simulation file, or
#any other related documentation or information provided by Altera or a
#megafunction partner, remains with Altera, the megafunction partner, or
#their respective licensors.  No other licenses, including any licenses
#needed under any third party's intellectual property, are provided herein.
#Copying or modifying any file, or portion thereof, to which this notice
#is attached violates this copyright.














































































































































































































































































package mk_em_uart;

use wiz_utils;
use filename_utils;  # This is for to Perlcopy 'uart.pl'
use europa_all;      # This imports the entire Eurpoa object-library.
use strict;          # This spanks you when you're naughty.  That's
#use europa_utils qw(concatenate);
use europa_utils;

sub make ($$) {

my ($project, $WSA) = @_;

make_uart ($project,$WSA);

$project->output();

}
















sub Validate_Uart_Options
{
  my ($Options, $project) = (@_);
  





































  &validate_parameter ({hash    => $Options,
                        name    => "fixed_baud",
                        type    => "boolean",
                        default => 1,
                       });



























  &validate_parameter ({hash    => $Options,
                        name    => "use_cts_rts",
                        type    => "boolean",
                        default => 0,
                       });
































  &validate_parameter ({hash    => $Options,
                        name    => "use_eop_register",
                        type    => "boolean",
                        default => 0,
                       });












# modified to allow the number 10
  &validate_parameter ({hash      => $Options,
                        name      => "data_bits",
                        type      => "integer",
                        allowed   => [7, 8, 9, 10],
                        default   => 8,
                       });













  &validate_parameter ({hash      => $Options,
                        name      => "stop_bits",
                        type      => "integer",
                        allowed   => [1, 2],
                        default   => 2,
                       });



































  &validate_parameter ({hash      => $Options,
                        name      => "parity",
                        type      => "string",
                        allowed   => ["S0", "S1", "E", "O", "N"],
                        default   => "N",
                       });













#  &validate_parameter ({hash      => $Options,
#                        name      => "baud",
#                        type      => "integer",
#                        allowed   => [   300,
#                                        1200,
#                                        2400,
#                                        4800,
#                                        9600,
#                                       14400,
#                                       19200,
#                                       28800,
#                                       38400,
#                                       57600,
#                                      115200,],
#                        default  =>   115200,
#                        severity => "warning",
#                       });

  &validate_parameter ({hash      => $Options,
                        name      => "baud",
                        type      => "integer",
                       });

  &validate_parameter ({hash      => $Options,
                        name      => "clock_freq",
                        type      => "integer",
                       });

























  &validate_parameter ({hash      => $Options,
                        name      => "sim_true_baud",
                        type      => "boolean",
                        default   => "0",
                       });
                       
   &validate_parameter ({hash      => $Options,
                        name      => "use_rx_fifo",
                        type      => "boolean",
                        default   => "0",
                       });
  &validate_parameter ({hash      => $Options,
                        name      => "use_tx_fifo",
                        type      => "boolean",
                        default   => "0",
                       });
  &validate_parameter ({hash      => $Options,
                        name      => "rx_fifo_LE",
                        type      => "boolean",
                        default   => "0",
                       });
  &validate_parameter ({hash      => $Options,
                        name      => "tx_fifo_LE",
                        type      => "boolean",
                        default   => "0",
                       });
&validate_parameter ({hash      => $Options,
                        name      => "hw_cts",
                        type      => "boolean",
                        default   => "0",
                       });
  &validate_parameter ({hash      => $Options,
                        name      => "fifo_size_tx",
						type      => "integer",
                        allowed   => [   2,
                                         4,
                                        8,
                                        16,
                                        32,
                                        64,
                                       128,
                                       256,
                                       512,
                                       1024,
                                       2048,
                                      4096,
									  8192,],
                        default  =>   16,
                        severity => "warning",

                       });
    &validate_parameter ({hash      => $Options,
                        name      => "fifo_size_rx",
						type      => "integer",
                        allowed   => [   2,
                                         4,
                                        8,
                                        16,
                                        32,
                                        64,
                                       128,
                                       256,
                                       512,
                                       1024,
                                       2048,
                                      4096,
									  8192,],
                        default  =>   16,
                        severity => "warning",

                       });
  &validate_parameter ({hash      => $Options,
                        name      => "fifo_export_used",
                        type      => "boolean",
                        default   => "0",
                       });




  $Options->{baud_divisor_constant} =
    int ( ($Options->{clock_freq} / $Options->{baud}) + 0.5);

  $Options->{divisor_bits} = 
      &Bits_To_Encode ($Options->{baud_divisor_constant});


  if (!$Options->{fixed_baud}) {
    $Options->{divisor_bits} = max ($Options->{divisor_bits}, 32);
  }

  &validate_parameter ({hash    => $Options,
                        name    => "divisor_bits",
                        type    => "integer",
                        range   => [1,32],
                        message => "cannot make desired baud rate from clock",
                       });

  $Options->{num_control_reg_bits} =
    ($Options->{use_cts_rts} | $Options->{use_eop_register}) ? 14 : 14 ;




 #changed to add in the threshold bit.
  $Options->{num_status_reg_bits} = $Options->{use_threshold} ? 15 : 15;





  return 1;
}






sub Create_BAT_File
{

 my ($Options, $project) = (@_);
  if($Options->{generate_sim})
  {

   my $OS = $^O;
   my $bat_filename = "";
   my @bat_io_files = ();
   my $bat_exe = "perl";
   my $bat_win = "";
   my $bat_win_name = "";
  my $exe = $bat_exe;
  $Options->{in_log_file} = $Options->{name} . "_in.log";
  $Options->{out_log_file} = $Options->{name} . "_log_module.txt";
 if ($Options->{interactive_out})
 {
   $bat_filename = $Options->{name}."_log.bat";
    if ($OS =~ /win/i) {  # match 'MSWin32' or 'cygwin'
       $bat_win = "\@ start \"$Options->{out_log_file}\" cmd /t:06 /c";
       $bat_exe = "%1%/".$bat_exe if ($exe eq "perl");
    } else {
       $bat_win = "xterm -T $Options->{name} -n $Options->{name} -fg yellow -bg black -e";
       $bat_exe = "\$1/".$bat_exe if ($exe eq "perl");
    }
    open (BATFILE, ">$Options->{sim_directory}$bat_filename")
       or &ribbit ("can't open $Options->{sim_directory}/$bat_filename: $!");
    print BATFILE "$bat_win $bat_exe -- tail-f.pl $Options->{out_log_file}\n";
    close (BATFILE);

    if (0 == (chmod 0755,"$Options->{sim_directory}$bat_filename")) {
        &goldfish ("can't set execute mode on $Options->{sim_directory}/$bat_filename");
    }
 }
 my $bat_exe = "perl";
 if ($Options->{interactive_in})
 {
    $bat_filename = $Options->{name}."_drive.bat";
    @bat_io_files = ( # go get list of files to be I/O
       $Options->{char_mutex_file},
       $Options->{char_data_file},
       $Options->{in_log_file},
    );


#          $bat_exe = $this_int_section->{exe};
    if ($bat_exe =~ /nios2-terminal/)
    {  # be special:
        @bat_io_files = ();
        $bat_win_name = $bat_exe;
    } else {
        $bat_win_name = $bat_io_files[$#bat_io_files];
    }
    if ($OS =~ /win/i)
    {  # match 'MSWin32' or 'cygwin'
       $bat_win ="\@ start \"$bat_win_name\" cmd /t:02 /c";
       $bat_exe = "%1%/".$bat_exe if ($exe eq "perl");
    } else {
       $bat_win = "xterm -T $Options->{name} -n $Options->{name} -fg green -bg black -e";
       $bat_exe = "\$1/".$bat_exe if ($exe eq "perl");
    }
    open (BATFILE, ">$Options->{sim_directory}$bat_filename")
       or &ribbit ("can't open $Options->{sim_directory}$bat_filename: $!");
    print BATFILE "$bat_win $bat_exe -- fuart.pl @bat_io_files\n";
    close (BATFILE);

    if (0 == (chmod 0755,"$Options->{sim_directory}/$bat_filename")) {
        &goldfish ("can't set execute mode on $Options->{sim_directory}/$bat_filename");
    }

 }
  }
}










sub Setup_Input_Stream
{
  my ($Options, $project) = (@_);
  if(!$Options->{generate_sim})
  {
    return 1;
  }
  if (($Options->{sim_char_file}   ne "") &&
      ($Options->{sim_char_stream} ne "")) {
    &ribbit
      ("Cannot set 'sim_char_stream' parameter when using 'sim_char_file'");
  }

  my $char_stream = $Options->{sim_char_stream};

  if ($Options->{sim_char_file} ne "") {
    $char_stream = "";
    open (CHARFILE, "< $Options->{sim_char_file}")
      or &ribbit ("Cannot open input-file $Options->{sim_char_file} ($!)");
    while (<CHARFILE>) {
      $char_stream .= $_;
    }
    close CHARFILE;
  }




  my $newline      = "\n";
  my $cr           = "\n";
  my $double_quote = "\"";



  $char_stream =~ s/\\n\\r/\n/sg;

  $char_stream     =~ s/\\n/$newline/sg;
  $char_stream     =~ s/\\r/$cr/sg;
  $char_stream     =~ s/\\\"/$double_quote/sg;

  my $crlf = "\n\r";
  $char_stream =~ s/\n/$crlf/smg;

  $Options->{stream_length} = length ($char_stream);

  $Options->{char_data_file} = $Options->{name} . "_input_data_stream.dat";
  $Options->{char_mutex_file} = $Options->{name} . "_input_data_mutex.dat";

#  my $sim_dir_name = $project->simulation_directory();
  $project->{_sopc_modelsim_dir} = $Options->{sim_directory};
  my  $sim_dir_name = $Options->{sim_directory};
  &Create_Dir_If_Needed($sim_dir_name);

  open (DATFILE, "> $sim_dir_name$Options->{char_data_file}")
    or &ribbit ("couldn't open $sim_dir_name$Options->{char_data_file} ($!)");

  my $addr = 0;
  foreach my $char (split (//, $char_stream)) {
    printf DATFILE "\@%X\n", $addr;
    printf DATFILE "%X\n", ord($char);
    $addr++;
  }

  printf DATFILE "\@%X\n", $addr;
  printf DATFILE "%X\n", 0;

  close DATFILE;

  open (MUTFILE, "> $sim_dir_name$Options->{char_mutex_file}")
   or &ribbit ("couldn't open $sim_dir_name$Options->{char_mutex_file} ($!)");


  if ($Options->{interactive_in})
  { # force user to use interactive window if selected by making mutex 0
      printf MUTFILE "0\n";
  }
  else
  { # set up proper stream file size in Mutex:
      printf MUTFILE "%X\n", $addr;
  }

  close MUTFILE;
  $Options->{mutex_file_size} = $addr;
}


































sub make_uart($$)
{
  my ($project,$WSA) = (@_);

  my $module  = $project->top();
  my $Options = $WSA;



 #  my $Options = $WSA;
 my $key;
  $Options->{name}       = $module->name();
# foreach $key (sort (keys (%{$WSA}))){
#          print "$key => $WSA->{$key}\n";
#  }
#print "%WSA";
#print "$Options";
#my $clock_freq = $project->get_module_clock_frequency();
#  $clock_freq || &ribbit ("Couldn't find Clock frequency\n");
#  $Options->{clock_freq} = $clock_freq;
my $fred =  $Options->{name};
print "I am here0 $fred";
#my $temp =$project->spaceless_module_ptf($Options->{name});
#print $temp;
#my $temp = $project->spaceless_module_ptf($Options->{name})->{SIMULATION}{INTERACTIVE_IN};
#print $temp;
#  my $int_in_section =
# $project->spaceless_module_ptf($Options->{name})->{SIMULATION}{INTERACTIVE_IN};
#   my $int_out_section=
# $project->spaceless_module_ptf($Options->{name})->{SIMULATION}{INTERACTIVE_OUT};
   my $interactive_in = 1; # default to non-interactive mode.
   my $interactive_out= 1;          # should add parameters for this. but having on all the time is ok
#
#   my $int_key;
#   my $this_int_section;
#
#   foreach $int_key (sort(keys (%{$int_in_section}))) {
#       $this_int_section = $int_in_section->{$int_key};
#       $interactive_in = $this_int_section->{enable};
#
#   }
#
# #print "I am here1\n" ;
#   foreach $int_key (sort(keys (%{$int_out_section}))) {
#       $this_int_section = $int_out_section->{$int_key};
#       $interactive_out = $this_int_section->{enable};
#
#   }
# #print "I am here2\n" ;
   $Options->{interactive_in} = $interactive_in;
   $Options->{interactive_out}= $interactive_out;

  &Validate_Uart_Options ($Options, $project);

#print "I am here3\n" ;

  if ( $Options->{generate_sim})
  {
   &Setup_Input_Stream ($Options, $project);
   &Create_BAT_File ($Options, $project);
  }
 print "I am here4\n" ;



  return if $project->_software_only();





print "I am here5\n" ;

  $Options->{tx_module_name}  = $Options->{name} . "_tx";
  $Options->{rx_module_name}  = $Options->{name} . "_rx";
  $Options->{reg_module_name} = $Options->{name} . "_regs";
  $Options->{log_module_name} = $Options->{name} . "_log";

  my $tx_module  = &make_uart_tx  ($Options);
  my $rx_module  = &make_uart_rx  ($Options);
  my $reg_module = &make_uart_regs($Options);




 print "I am here6\n" ;


  my $marker = e_default_module_marker->new ($module);





 print "I am here7\n" ;

  e_assign->add(["clk_en", 1]);

  e_instance->add ({module => $tx_module });
  e_instance->add ({module => $rx_module });
  e_instance->add ({module => $reg_module});
  if ( $Options->{generate_sim})
  {
    print "I am here7.1\n" ;
    my $log_instance = &make_uart_log ($Options);
        $module->add_contents ( $log_instance );

    if ($Options->{interactive_in})
    {

#      &Perlcopy ($project->_module_lib_dir . "/fuart.pl",
      &Perlcopy ( "fuart.pl",
		 $Options->{sim_directory} . "/fuart.pl" );

    }


    if ($Options->{interactive_out})
    {
#      &Perlcopy ($project->_module_lib_dir . "/ftail-f.pl",
      &Perlcopy ("ftail-f.pl",
                 $Options->{sim_directory} . "/ftail-f.pl" );
#      &Perlcopy ( "fifoed_uart_log.bat",
#		 $project->{sim_dir} . "/fifoed_uart_log.bat" );

    }

   &Create_BAT_File ($Options, $project);
  }

#   print "I am here8\n" ;





  e_avalon_slave->add ({name => "s1"});

  return $module;
}







sub make_uart_log {
    my ($Options) = (@_);
    return e_log->new ({
	name            => $Options->{"log_module_name"},
	tag             => "simulation",
	relativepath  =>1,
	port_map        => {
	    "valid"	=> "~tx_ready",
	    "strobe"	=> "tx_wr_strobe",
	    "data"	=> "tx_data",
	    "log_file"	=> $Options->{log_file},
	},
    });

};

sub make_uart_tx
{
  my ($Options) = (@_);


  my $module = e_module->new ({name  => $Options->{tx_module_name}});
  my $marker = e_default_module_marker->new ($module);

# calculation the text for the code
 my $parity_expr = "";
     $parity_expr = "1'b0"        if ($Options->{parity} =~ /^S0$/i);
     $parity_expr = "1'b1"        if ($Options->{parity} =~ /^S1$/i);
     $parity_expr = "  ^tx_data " if ($Options->{parity} =~ /^E$/i );
     $parity_expr = "~(^tx_data)" if ($Options->{parity} =~ /^O$/i );


# calculating the size of the shift reister
  my $tx_shift_bits =
    ($Options->{stop_bits}             )  +   # stop bits.
    ($Options->{parity} =~ /N/i ? 0 : 1)  +   # parity bit (opt.)
    ($Options->{data_bits}             )  +   # "payload"
    (1                                 )  ;   # start-bit


  if($Options->{use_tx_fifo} == 0)
  {
      e_assign->add (["tx_wr_strobe_onset",  "tx_wr_strobe && begintransfer"]),  # cruben  this should be based on if you need a fifo or not
           e_signal->add(["tx_data", $Options->{data_bits}]);
  }
  else
  {
   	  e_assign->add (["tx_wr_strobe_onset",  "tx_wr_strobe && tx_ready"]),  #cruben
           e_signal->add(["tx_data", $Options->{data_bits}]);
  }

  my $load_val_expr = &concatenate ("\{$Options->{stop_bits} \{1'b1\}\}",
                                    $parity_expr,
                                    "tx_data",
                                    "1'b0",
                                   );

  e_assign->add ({lhs => e_signal->add (["tx_load_val", $tx_shift_bits]),
                  rhs => $load_val_expr,
  });










  e_assign->add (["shift_done", "~(|tx_shift_register_contents)"]);

# added an additional port used when with fifos  cruben
#  e_register->add ({out => e_port->add (["do_load_shifter",1,"out"]),
#                     in  => "(~tx_ready) && shift_done",
#                    })if  $Options->{use_fifo};

  e_register->add ({out => "do_load_shifter",
                     in  => "(~tx_ready) && shift_done",
                    })if  !$Options->{hw_cts};

   e_register->add ({out => "do_load_shifter",
                     in  => "(~tx_ready) && shift_done && ~cts_n",
                    })if  $Options->{hw_cts};









  e_register->add ({
    out         => e_port->add (["tx_ready", 1, "out"]),
    sync_set    => "do_load_shifter",
    set_value   => "1'b1",
    sync_reset  => "tx_wr_strobe_onset",
    async_value => "1'b1",
  });




  e_register->add ({
    out        => "tx_overrun",
    sync_set   => "(~tx_ready && tx_wr_strobe_onset)",
    sync_reset => "status_wr_strobe",
  })if  !$Options->{use_tx_fifo};

  e_register->add ({
    out         => "tx_shift_empty",
    in          => "tx_ready && shift_done",
    async_value => "1'b1",
  });













  e_register->add ({
      out        => e_signal->add({name  => "baud_rate_counter",
                                   width => $Options->{divisor_bits}
                                  }),
      in         => "baud_rate_counter - 1",
      sync_set   => "baud_rate_counter_is_zero || do_load_shifter",
      set_value  => "baud_divisor",
    });

  e_assign->add(["baud_rate_counter_is_zero", "baud_rate_counter == 0"]);


  # need this value for reg module -  no need to regenerate it.
  e_register->add ({out    => "baud_clk_en",
                    in     => "baud_rate_counter_is_zero",
                   });









  e_assign->add(["do_shift", "baud_clk_en  &&
                             (~shift_done) &&
                             (~do_load_shifter)"
   ]);

  e_shift_register->add ({
    serial_out   => "tx_shift_reg_out",
    serial_in    => "1'b0",
    parallel_in  => "tx_load_val",
    parallel_out => "tx_shift_register_contents",
    shift_length => $tx_shift_bits,
    direction    => "LSB-first",
    load         => "do_load_shifter",
    shift_enable => "do_shift",
  });









 #    e_register->add ({
 #   out         => e_port->add (["tx_ready", 1, "out"]),
 #   sync_set    => "do_load_shifter",
 #   sync_reset  => "tx_wr_strobe_onset",
 #   async_value => "1'b1",
 # });

  e_register->add({
    out         => "pre_txd",
    in          => "tx_shift_reg_out",
    enable      => "~shift_done",
    async_value => "1",
  })if  !$Options->{hw_cts};
  
    e_register->add({
    out         => "pre_txd",
    in          => "tx_shift_reg_out",
#    enable      => "~shift_done",
    sync_set      => "shift_done",
    set_value     => "1'b1",
    async_value => "1",
  })if  $Options->{hw_cts};

  e_assign->add(["txd_reset_or_hold", "(!reset_n || (shift_done && cts_n))"]) if  $Options->{hw_cts};;


  e_register->add ({
    out         => "txd",
    in          => "pre_txd & ~do_force_break",
    sync_set    => "txd_reset_or_hold" ,
    set_value   => "1'b1",
    })if  $Options->{hw_cts};

  e_register->add ({
    out         => "txd",
    in          => "pre_txd & ~do_force_break",
    async_value => "1",
  })if  !$Options->{hw_cts};
  return $module;
}
















































sub make_uart_rx
{
  my ($Options) = (@_);






  $Options->{rx_source_module_name} =
    $Options->{rx_module_name} . "_stimulus_source";
  my $stim_module = &make_uart_rxd_source($Options);


  my $module = e_module->new ({name  => $Options->{rx_module_name}});
  my $marker = e_default_module_marker->new ($module);

  e_instance->add ({module => $stim_module});










  e_register->add({
    out    => "sync_rxd",
    in     => "source_rxd",
    delay  => 2,
  });



  e_edge_detector->add({
    in     => "sync_rxd",
    out    => "rxd_falling",
    edge   => "falling",
  });

  e_edge_detector->add({
    in     => "sync_rxd",
    out    => "rxd_edge",
    edge   => "any",
  });






 if($Options->{use_rx_fifo} == 0)
  {
      e_assign->add (["rx_rd_strobe_onset", "rx_rd_strobe && begintransfer"]);
  }
  else
  {
   	  e_assign->add (["rx_rd_strobe_onset", "rx_rd_strobe"]);
      if($Options->{add_error_bits})
      {
          e_signal->add ({comment=>"collection of error bits for the rx fifo",name =>"rx_error_bits", width => 4,export =>1});
          e_signal->add ({name=>"is_parity_error",width=>1,never_export => 1});
          #need to latch these until the rx_rd_strobe happens then they need to be cleared so the bits only
          #show up on one character.
          e_register->add ({comment =>"This is the parity error bit for the fifo",
                          out => "rx_error_bits\[0\]",
                          in =>  "(got_new_char && is_parity_error) ||(!got_new_char && rx_error_bits\[0\])"});    #PE
          e_register->add ({comment =>"This is the framing error bit for the fifo",
                          out => "rx_error_bits\[1\]",
                          in => "(~stop_bit && ~is_break) || (!got_new_char && rx_error_bits\[1\])" });                         #FE
          e_register->add ({comment =>"This is the BREAK bit for the fifo",
                          out => "rx_error_bits\[2\]",
                          in => "(got_new_char && is_break) || (!got_new_char && rx_error_bits\[2\])"});           #break
          e_register->add ({comment =>"This is the Parity Error bit for the fifo",
                          out => "rx_error_bits\[3\]",
                          in => "(got_new_char && rx_char_ready) || (!got_new_char && rx_error_bits\[3\])"});      #Over run
      }
  }
























  e_signal->add (["half_bit_cell_divisor", $Options->{divisor_bits} - 1]);
  e_assign->add ({
    lhs => "half_bit_cell_divisor",
    rhs => 'baud_divisor [baud_divisor.msb : 1]',
  });

  e_mux->add ({
    lhs     => e_signal->add (["baud_load_value", $Options->{divisor_bits}]),
    table   => [rxd_edge => "half_bit_cell_divisor"],
    default => "baud_divisor",
  });

  e_register->add ({
    out       => e_signal->add(["baud_rate_counter",$Options->{divisor_bits}]),
    in        => "baud_rate_counter - 1",
    sync_set  => "baud_rate_counter_is_zero || rxd_edge",
    set_value => "baud_load_value",
  });

  e_assign->add (["baud_rate_counter_is_zero", "baud_rate_counter == 0"]);

  e_register->add ({
    in        => "baud_rate_counter_is_zero",
    out       => "baud_clk_en",
    sync_set  => "rxd_edge",
    set_value => "0",
  });










  e_assign->add (["sample_enable", "baud_clk_en && rx_in_process"]);

  e_register->add ({
    out         => "do_start_rx",
    in          => "0",
    sync_set    => "(~rx_in_process && rxd_falling)",
    set_value   => "1",
    async_value => "0",
  });







  my $rx_shift_bits = 
    (1                                 )  +   # stop bit.
    ($Options->{parity} =~ /N/i ? 0 : 1)  +   # parity bit (opt.)
    ($Options->{data_bits}             )  +   # "payload"
    (1                                 )  ;

  e_shift_register->add ({
    parallel_out => "rxd_shift_reg",
    serial_in    => "sync_rxd",
    serial_out   => "shift_reg_start_bit_n",
    parallel_in  => "\{$rx_shift_bits\{1'b1\}\}",
    shift_length => $rx_shift_bits,
    direction    => "LSB-first",
    load         => "do_start_rx",
    shift_enable => "sample_enable",
  });

  e_assign->add (["rx_in_process", "shift_reg_start_bit_n"]);



  e_signal->add (["raw_data_in", $Options->{data_bits}]);








  my $start_bit_sig = e_signal->add(["unused_start_bit", 1]);
  $start_bit_sig->never_export(1);

  my    @register_segments = ("stop_bit"  );
  push (@register_segments,   "parity_bit") unless $Options->{parity} =~/^N$/i;
  push (@register_segments,   "raw_data_in",
                              "unused_start_bit");

  e_assign->add([&concatenate (@register_segments), "rxd_shift_reg"]);









  e_assign->adds(["is_break",         "~(|rxd_shift_reg)"      ],
                 ["is_framing_error", "~stop_bit && ~is_break" ]);




  e_edge_detector->add ({
    out    => "got_new_char",
    in     => "rx_in_process",
    edge   => "falling",
  });

  e_register->add({
    in     => "raw_data_in",
    out    => e_signal->add(["rx_data", $Options->{data_bits}]),
    enable => "got_new_char",
  });

  e_register->add({
    out        => "framing_error",
    sync_set   => "(got_new_char && is_framing_error)",
    sync_reset => "status_wr_strobe",
  });

  e_register->add({
    out        => "break_detect",
    sync_set   => "(got_new_char && is_break)",
    sync_reset => "status_wr_strobe",
  });

  e_register->add({
    out        => "rx_overrun",
    sync_set   => "(got_new_char && rx_char_ready)",
    sync_reset => "status_wr_strobe",
  });

  e_register->add({
    out        => e_port->add (["rx_char_ready", 1, "out"]),
    sync_set   => "got_new_char",
    sync_reset => "rx_rd_strobe_onset",
    priority   => "reset",
  });



  if ($Options->{parity} =~ /^N$/i) {
    e_assign->add (["parity_error", "0"]);
  } else {
    my $correct_parity_expr = "";
    $correct_parity_expr = "0"               if $Options->{parity} =~ /^S0$/i;
    $correct_parity_expr = "1"               if $Options->{parity} =~ /^S1$/i;
    $correct_parity_expr = " (^raw_data_in)" if $Options->{parity} =~ /^E$/i;
    $correct_parity_expr = "~(^raw_data_in)" if $Options->{parity} =~ /^O$/i;

    e_assign->add (["correct_parity", $correct_parity_expr]);

    e_assign->add ({
      lhs    => "is_parity_error",
      rhs    => "(correct_parity != parity_bit) && ~is_break",
    });

    e_register->add ({
      out        => "parity_error",
      sync_set   => "got_new_char && is_parity_error",
      sync_reset => "status_wr_strobe",
    });
  }

  return $module;
}



























sub make_uart_regs
{


  my ($Options) = (@_);


  my $module = e_module->new ({name  => $Options->{reg_module_name}});
  my $marker = e_default_module_marker->new ($module);





#cruben made 32 bits

  e_register->add({
   comment => "CJR changed to 32 bits wide",
   out => e_signal->add (["readdata",           32]),
   in  => e_signal->add (["selected_read_data", 32]),
  });
my $rx_Use_EAB = $Options->{rx_fifo_LE} ? qq("OFF") : qq("ON");
my $rx_fifo_address_bits = log2($Options->{fifo_size_rx});
my $tx_fifo_address_bits = log2($Options->{fifo_size_tx});

# this section is all about the timestamp
# if it is external the add a timestamp port

if( $Options->{use_timestamp} && $Options->{use_ext_timestamp} && $Options->{use_rx_fifo} )
{
   e_port->adds (
    ["timestamp_in",          $Options->{timestamp_width},                        "in" ]
    );

}
if ( $Options->{use_timestamp} && $Options->{use_rx_fifo}&& !$Options->{use_ext_timestamp})
{
# $module->{comment} .= "This section is for implementation of the timestamp\n";
 e_instance->add
         ({
             module => e_module->new
                 ({
                     name => $Options->{name}."_tsfifo",
                     contents=>
                     [
                     e_blind_instance->new
                      ({
                          tag => "normal",
                          name => 'tsfifo',
                          module => 'scfifo',
                          use_sim_models => 1,
                          in_port_map =>
                          {
                            rdreq   => 'd1_rx_fifo_rd_strobe',   #same as rx fifo
                            sclr	  => 'reset',              #same as rx fifo
                            clock   => 'clk',                    #same as rx fifo
                            wrreq	  => 'd1_rx_rd_strobe',    #same as rx fifo
                            data    => 'ts_in_data',                # this comes from the internal timestamp counter or externally
                          },
                          out_port_map =>
                          {
                              q       => 'timestamp_data',
                          },
                          parameter_map =>
                          {
                              lpm_width               => $Options->{timestamp_width},
                              lpm_numwords            => $Options->{fifo_size_rx},       #same as rx fifo
                              lpm_widthu	      => $rx_fifo_address_bits,
                              lpm_type		      => qq("scfifo"),
                              lpm_showahead	      => qq("ON"),
                              overflow_checking	      => qq("ON"),
                              underflow_checking      => qq("ON"),
                              use_eab		      => $rx_Use_EAB,
                          },
                        }), # wfifo instance
                       ], # contents
                 }), # e_module scfifo_w
         }); # e_instance wfifo

#   my $in_ts_port_map = {
#              rdreq   => 'd1_rx_fifo_rd_strobe',   #same as rx fifo
#              sclr	  => 'reset',              #same as rx fifo
#              clock   => 'clk',                    #same as rx fifo
#              wrreq	  => 'd1_rx_rd_strobe',    #same as rx fifo
#              data    => 'ts_in_data',                # this comes from the internal timestamp counter or externally
#             };

#  my $out_ts_port_map = { q       => 'timestamp_data',
#              };

#  my $ts_parameter_map = {
#              lpm_width               => $Options->{timestamp_width},
#              lpm_numwords            => $Options->{fifo_size_rx},       #same as rx fifo
#              lpm_widthu			  => $rx_fifo_address_bits,
#              lpm_type				  => qq("scfifo"),
#              lpm_showahead			  => qq("ON"),
#              overflow_checking	  => qq("ON"),
#              underflow_checking	  => qq("ON"),
#              use_eab			  => $rx_Use_EAB,
#			  };
#  e_blind_instance->add({
#             tag            => 'normal',
#             use_sim_models => 1,
#             name           => 'timestamp_fifo',
#             module         => 'scfifo',
#             in_port_map    => $in_ts_port_map,
#             out_port_map   => $out_ts_port_map,
#             parameter_map  => $ts_parameter_map,
#          });


  if( $Options->{use_ext_timestamp} )
  {
     e_assign->add ({
     comment => "Data from an external timestamp source to feed the fifo",
     lhs => e_signal->add(["ts_in_data", $Options->{timestamp_width}]),
     rhs => 'timestamp_in',
     });

   } else {
     e_assign->add ({
       comment => "Data from the internal timestamp source to feed the fifo.  Needs to shifted by 3 to get the divide by 8",
     lhs => e_signal->add(["ts_in_data", $Options->{timestamp_width}]),
     rhs => "timestamp_counter\[$Options->{timestamp_width}+2:3\]",
    });

     e_register->add ({
      comment => "This counter counts baudrate/8",
      out        => e_signal->add({name  => "timestamp_counter",
                                   width => $Options->{timestamp_width}+3}),
      in         => "timestamp_counter + 1 ",
      enable     => "baud_clk_en ",

     })if !$Options->{use_ext_timestamp};

     e_signal->add(["timestamp_data", $Options->{timestamp_width}]);
     }

  }
#end timestamp

#irq get connected automatically so I have to change the name so it will not
if ( $Options->{Has_IRQ})
{
  e_register->add({
    comment =>"IRQ to the avalon system",
    out    => "irq",
    in     => "qualified_irq",
  });
}
else
{
  e_register->add({
    comment =>"not connected to the avalon system",
    out    => "irqexport",
    in     => "qualified_irq",
  });
}

#changed the width to 32
  e_port->adds (
    ["address",          4,                        "in" ],
    ["writedata",       32,                        "in" ],
    ["tx_wr_strobe",     1,                        "out"],
    ["status_wr_strobe", 1,                        "out"],
    ["rx_rd_strobe",     1,                        "out"],
    ["baud_divisor",     $Options->{divisor_bits}, "out"],
  );

#  e_assign->adds(
#    ["rx_rd_strb",      "rx_char_ready"],
#    ["tx_wr_strb",      "tx_shift_empty && ~tx_empty"],) if  $Options->{use_fifo};

  e_assign->adds(
    ["status_wr_strobe",  "chipselect && ~write_n && (address == 4'd2)"],
    ["control_wr_strobe", "chipselect && ~write_n && (address == 4'd3)"],);

  e_assign->add(["rx_rd_strobe",      "chipselect && ~read_n  && (address == 4'd0)"])if  !$Options->{use_rx_fifo};
  e_assign->add(["rx_fifo_rd_strobe",      "chipselect && ~read_n  && (address == 4'd0) && begintransfer"])if  $Options->{use_rx_fifo};
  e_assign->add(["rx_rd_strobe",      "rx_char_ready && rx_not_full"])if  $Options->{use_rx_fifo};
  e_assign->add(["tx_wr_strobe",      "chipselect && ~write_n && (address == 4'd1)"])if  !$Options->{use_tx_fifo};
  e_assign->add(["tx_fifo_wr_strobe",      "chipselect && ~write_n && (address == 4'd1) && begintransfer"])if  $Options->{use_tx_fifo};

  e_assign->add(["tx_wr_strobe",      "tx_ready && tx_not_empty"])if  ($Options->{use_tx_fifo} && !$Options->{hw_cts});
  e_assign->add(["tx_wr_strobe",      "tx_ready && tx_not_empty && ~cts_n"])if  ($Options->{use_tx_fifo} && $Options->{hw_cts});

  e_assign->add([ e_signal->add (["reset",1]),"~reset_n",]) if  ($Options->{use_tx_fifo} || $Options->{use_rx_fifo}) ;

  e_assign->add([
     "divisor_wr_strobe", "chipselect && ~write_n  && (address == 4'd4)",
  ]) if !$Options->{fixed_baud};
  e_assign->add([
     "eop_char_wr_strobe","chipselect && ~write_n  && (address == 4'd5)",
  ]) if  $Options->{use_eop_register};
  e_assign->add(["gap_wr_strobe",      "chipselect && ~write_n && (address == 4'd8) && begintransfer"])if  $Options->{use_gap_detection};





  my $tx_data_sig = e_signal->add(["tx_data", $Options->{data_bits}]);
  $tx_data_sig->export(1);
  
  e_register->add ({
    out    => e_signal->add(["gap_reg", 16 ]),
    in     => "writedata\[15 : 0\]",
    enable => "gap_wr_strobe",
    async_value => $Options->{gap_value},
    comment =>"this is the r/w register that contains the gap value",
  })if $Options->{use_gap_detection};

  e_register->add ({
    out    => $tx_data_sig,
    in     => "writedata\[tx_data.msb : 0\]",
    enable => "tx_wr_strobe",
  })if !$Options->{use_tx_fifo};  #fifo will supply if present
  e_register->add ({
    out    => $tx_data_sig,
    in     => "tx_fifo_q",
    enable => "tx_wr_strobe",
    comment =>"tx_data comming from the tx_fifo",
  })if $Options->{use_tx_fifo};  #fifo will supply if present

  e_register->add ({
    out    => e_signal->add(["control_reg", $Options->{num_control_reg_bits}]),
    in     => "writedata\[control_reg.msb:0\]",
    enable => "control_wr_strobe",
  });

  e_register->add ({
    out    => e_signal->add(["eop_char_reg", $Options->{data_bits}]),
    in     => "writedata\[eop_char_reg.msb:0\]",
    enable => "eop_char_wr_strobe",
  }) if $Options->{use_eop_register};


  e_register->add ({
    out        => "tx_overrun",
    sync_set   => "tx_full && tx_fifo_wr_strobe",
    sync_reset => "status_wr_strobe",
  })if  $Options->{use_tx_fifo};


  e_edge_detector->add ({
    tag  => "simulation",
    out  => "do_write_char",
    in   => "tx_ready",
  });

  e_process->add ({
    tag  => "simulation",
    contents => [
      e_if->new ({
      tag  => "simulation",
      condition       => "do_write_char",
      then            => [
        e_sim_write->new ({
          spec_string => '%c',
          expressions => ["tx_data"],
        })
      ],
    }),
  ]});



  e_signal->add (["divisor_constant", $Options->{divisor_bits}]);
  e_assign->add ({
    tag  => $Options->{sim_true_baud} ? "normal" : "synthesis",
    lhs  => "divisor_constant",
    rhs  => $Options->{baud_divisor_constant}
  });
  e_assign->add ({
    tag   => "simulation",
    lhs   => "divisor_constant",
    rhs   => 4,
  }) if !$Options->{sim_true_baud};




  if ($Options->{fixed_baud}) {
    e_assign->add(["baud_divisor", "divisor_constant"]);
  } else {
    e_register->add ({
      in          => "writedata\[baud_divisor.msb:0\]",
      out         => "baud_divisor",
      enable      => "divisor_wr_strobe",
      async_value => "divisor_constant",
    });
  }







  if ($Options->{use_cts_rts} ) {
    e_register->add ({
      in          => "~cts_n",
      out         => "cts_status_bit",
      async_value => 1,
    });

    e_edge_detector->add ({
      in     => "cts_status_bit",
      out    => "cts_edge",
      edge   => "any",
    });

    e_register->add ({
      out         => "dcts_status_bit",
      sync_set    => "cts_edge",
      sync_reset  => "status_wr_strobe",
      async_value => 0,
    });




    e_assign->add (["rts_n", "~rts_control_bit"]);
  } else {
   if ($Options->{hw_cts} ) {
    e_register->add ({
      in          => "~cts_n",
      out         => "cts_status_bit",
      async_value => 1,
    });

    e_edge_detector->add ({
      in     => "cts_status_bit",
      out    => "cts_edge",
      edge   => "any",
    });

    e_assign->adds (["dcts_status_bit", 0]);
    }
    else
    {
    e_assign->adds (["cts_status_bit",  0],
                    ["dcts_status_bit", 0]);
    }

  }







  e_signal->adds({name => "rts_control_bit", never_export => 1},
                 {name => "ie_dcts",         never_export => 1},
                 {name => "ie_eop",          never_export => 1},
                 {name => "ie_gap_detection",          never_export => 1});

  my @control_reg_bits = ();

  push (@control_reg_bits, "ie_gap_detection",
                           "ie_eop" ,
                           "rts_control_bit",
                           "ie_dcts" ,
                           "do_force_break",
                           "ie_any_error",
                           "ie_rx_char_ready",
                           "ie_tx_ready",
                           "ie_tx_shift_empty",
                           "ie_tx_overrun",
                           "ie_rx_overrun",
                           "ie_break_detect",
                           "ie_framing_error",
                           "ie_parity_error",

       );

  e_assign->add([&concatenate(@control_reg_bits), "control_reg"]);




  e_assign->add ({
    lhs    => "any_error",
    rhs    => join (" ||\n",
                             "tx_overrun",
                             "rx_overrun",
                             "parity_error",
                             "framing_error",
                             "break_detect",
                   ),
  });

  my @status_reg_bits = ();
#  if  ($Options->{use_timout})
#{
	  push (@status_reg_bits,
                          "rx_at_threshold");
#}
  push (@status_reg_bits, "gap_timeout",
                          "eop_status_bit",
                          "cts_status_bit",
                          "dcts_status_bit",
                          "1'b0",
                          "any_error");
if  ($Options->{use_rx_fifo})
{
	 push (@status_reg_bits,
                          "rx_not_empty");
}
else
{
  push (@status_reg_bits,
                          "rx_char_ready");
}
if  ($Options->{use_tx_fifo})
{
	  push (@status_reg_bits,
                          "tx_not_full");
}
else
{
  push (@status_reg_bits,
                          "tx_ready");
}
push (@status_reg_bits,
                          "tx_shift_empty",
                          "tx_overrun",
                          "rx_overrun",
                          "break_detect",
                          "framing_error",
                          "parity_error",

       );

# push (@status_reg_bits, "eop_status_bit",
#                          "cts_status_bit",
#                          "dcts_status_bit",
#                          "1'b0",
#                          "any_error",
#                          "rx_not_empty",
#                          "tx_not_full",
#                          "tx_shift_empty",
#                          "tx_overrun",
#                          "rx_overrun",
#                          "break_detect",
#                          "framing_error",
#                          "parity_error",
#       )if  ($Options->{use_rx_fifo} && $Options->{use_tx_fifo});

  e_assign->add ({
    lhs => e_signal->add(["status_reg", $Options->{num_status_reg_bits}]),
    rhs => &concatenate (@status_reg_bits),
  });

# comment


  if ( $Options->{use_tx_fifo} ||$Options->{use_rx_fifo})
  {
    e_register->add({
      in => "rx_not_empty",
      comment =>"Register the rx_not_empty for timing",
    })if  $Options->{use_rx_fifo};
    e_register->add({
      in => "tx_not_full",
      comment =>"Register the tx_not_full for timing",
    })if  $Options->{use_tx_fifo};
    e_register->add({
      in => "rx_rd_strobe",
      comment =>"Register the rx_rd_strobe for timing",
    })if  $Options->{use_rx_fifo};

    e_register->add({
      in => "rx_fifo_rd_strobe",
      comment =>"Register the rx_fifo_strobe for timing",
    })if  $Options->{use_rx_fifo};
 #   e_register->add({
 #     in => "tx_ready",
 #   });
    e_register->add({
      in => "tx_wr_strobe",
      comment =>"Register the tx_wr_strobe for timing",
    })if  $Options->{use_tx_fifo};

#qsys does not use.
#    e_assign->adds
#      ([e_port->new (["dataavailable", 1, "out"]),  "d1_rx_not_empty"] )if  $Options->{use_rx_fifo};
#    e_assign->adds
#      ([e_port->new (["dataavailable", 1, "out"]),  "d1_rx_char_ready"] )if  !$Options->{use_rx_fifo};
#    e_assign->adds
#       ([e_port->new (["readyfordata",  1, "out"]),  "d1_tx_not_full" ] )if  $Options->{use_tx_fifo};
#   e_assign->adds
#       ([e_port->new (["readyfordata",  1, "out"]),  "d1_tx_ready" ] )if  !$Options->{use_tx_fifo};
# Cal 17 dec 08

      e_assign->adds
#       ([e_signal->new ({name => "tx_almost_empty", never_export => 1}),  "tx_used <=$Options->{tx_IRQ_Threshold}" ] )if  $Options->{use_tx_fifo};
       ([e_signal->new ({name => "tx_almost_empty", never_export => 1}), "(tx_used <=$Options->{tx_IRQ_Threshold}) && tx_not_full" ] )if $Options->{use_tx_fifo};
       # e_assign->adds
       #([e_signal->new ({name =>"rx_almost_full", never_export => 1}),  "rx_used > -8+ $Options->{fifo_size_rx}" ] )if  $Options->{use_rx_fifo};

#end
  }
  else
  {
    e_register->add({
      in => "rx_char_ready",
    });
    e_register->add({
      in => "tx_ready",
    });

#    e_assign->adds
#      ([e_port->new (["dataavailable", 1, "out"]),  "d1_rx_char_ready"],
#       [e_port->new (["readyfordata",  1, "out"]),  "d1_tx_ready"     ] );
  }


if ( $Options->{use_tx_fifo})
{
  my $tx_Use_EAB = $Options->{tx_fifo_LE}? qq("OFF") : qq("ON");
  e_instance->add
         ({
             module => e_module->new
                 ({
                     name => $Options->{name}."_txfifo",
                     contents=>
                     [
                     e_blind_instance->new
                      ({
                          tag => "normal",
                          name => 'txfifo',
                          module => 'scfifo',
                          use_sim_models => 1,
                          in_port_map =>
                          {
                             rdreq   => 'd1_tx_wr_strobe',  #needs to be one clock late
                             sclr	  => 'reset',
                             clock   => 'clk',
                             wrreq	  => 'tx_fifo_wr_strobe',
                             data    => "writedata\[$Options->{data_bits}-1:0\]",
                          },
                          out_port_map =>
                          {
                             q       => 'tx_fifo_q',
                             usedw    => 'tx_used',
                             empty => 'tx_empty',
                             full  => 'tx_full',
                          },
                          parameter_map =>
                          {
                             lpm_width               => $Options->{data_bits},
                             lpm_numwords            => $Options->{fifo_size_tx},
			     lpm_widthu		     => $tx_fifo_address_bits,
			     lpm_type		     => qq("scfifo"),
			     lpm_showahead	     => qq("ON"),
			     overflow_checking	  => qq("ON"),
			     underflow_checking	  => qq("ON"),
			     use_eab			  => $tx_Use_EAB,
                          },
                        }), # wfifo instance
                       ], # contents
                 }), # e_module scfifo_w
         }); # e_instance wfifo
#  my $in_tx_port_map = {
#              rdreq   => 'd1_tx_wr_strobe',  #needs to be one clock late
#              sclr	  => 'reset',
#              clock   => 'clk',
#              wrreq	  => 'tx_fifo_wr_strobe',
#              data    => "writedata\[tx_data.msb:0\]",
#  };
#  my $out_tx_port_map = {
#			  q       => 'tx_fifo_q',
#              usedw    => 'tx_used',
#              empty => 'tx_empty',
#              full  => 'tx_full',
#             };
#my $tx_Use_EAB = $Options->{tx_fifo_LE}? qq("OFF") : qq("ON");
#  my $tx_parameter_map = {
#              lpm_width               => $Options->{data_bits},
#              lpm_numwords            => $Options->{fifo_size_tx},
#			  lpm_widthu			  => $tx_fifo_address_bits,
#			  lpm_type				  => qq("scfifo"),
#			  lpm_showahead			  => qq("ON"),
#			  overflow_checking	  => qq("ON"),
#			  underflow_checking	  => qq("ON"),
#			  use_eab			  => $tx_Use_EAB,
#			  };
#
#  e_blind_instance->add({
#             tag            => 'normal',
#             use_sim_models => 1,
#             name           => 'write_fifo',
#             module         => 'scfifo',
#             in_port_map    => $in_tx_port_map,
#             out_port_map   => $out_tx_port_map,
#            parameter_map  => $tx_parameter_map,
#          });
        e_signal->add(["tx_buff_used", $tx_fifo_address_bits+1]);
        e_signal->add(["tx_used", $tx_fifo_address_bits]);
        e_signal->add(["tx_fifo_q", $Options->{data_bits}]);
        e_assign->add(["tx_buff_used", "\{tx_full,tx_used\}"]);
  		  e_assign->add(["tx_not_full", "~tx_full"]);
  		  e_assign->add(["tx_not_empty", "~tx_empty"]);
  }
# e_instance->new({
#             name            => 'write_fifo',
#             module          => 'scfifo',
#             port_map    	 => {
#             	'rdreq'   		 => 'do_load_shifter',
#              	'sclr'	  	 	 => 'reset',
#              	'clock'   	 	 => 'clk',
#              	'wrreq'	  	 	 => 'tx_wr_strobe',
#              	'data'    	 	 => "writedata\[tx_data.msb:0\]",  #"writedata\[tx_data.msb : 0\]"  $Options->{data_bits}
#			  	'q'       	 	 => 'tx_data',
#              	'usedw'    	 	 => 'tx_used',
#              	'empty' 		 => 'tx_empty',
#              	'full'  		 => 'tx_full',
#             }
 #            parameter_map  => $parameter_map,
  #        });
e_assign->add ([e_signal->new ({name => "gap_timeout", never_export => 1}), 0])if  !$Options->{use_gap_detection};
e_assign->add ([e_signal->new ({name => "rx_at_threshold", never_export => 1}), 0 ])if !$Options->{use_rx_fifo};
if ( $Options->{use_rx_fifo})
{
   my $rx_spaces = 10-$Options->{data_bits};
   e_register->add ({out => "gap_detect",
                       in  => "(gap_counter[18:3] == gap_reg || gap_detect) && !d1_rx_rd_strobe",
                    })if  $Options->{use_gap_detection};
      e_assign->add([e_signal->add(["gap_detect", 1 ]),  "1'b0"])if  !$Options->{use_gap_detection};


   # have to account for error bits if enabled.
   my $rx_data_bits =  $Options->{add_error_bits}? 15 : $Options->{data_bits};

      e_assign->add([e_signal->add(["in_rx_data", $rx_data_bits ]),  "rx_data"])if  !$Options->{add_error_bits};
      e_assign->add([e_signal->add(["in_rx_data", $rx_data_bits ]),  &concatenate ("gap_detect","rx_error_bits", "\{$rx_spaces\{1'b0\}\}","rx_data")  ] )if  $Options->{add_error_bits};
      e_instance->add
         ({
             module => e_module->new
                 ({
                     name => $Options->{name}."_rxfifo",
                     contents=>
                     [
                     e_blind_instance->new
                      ({
                          tag => "normal",
                          name => 'rxfifo',
                          module => 'scfifo',
                          use_sim_models => 1,
                          in_port_map =>
                          {
                              rdreq   => 'd1_rx_fifo_rd_strobe',
                              sclr	  => 'reset',
                              clock   => 'clk',
                              wrreq	  => 'd1_rx_rd_strobe',
                              data    => 'in_rx_data',
                            },
                          out_port_map =>
                          {
                              q       => 'rx_data_b',
                              usedw    => 'rx_used',
                              empty => 'rx_empty',
                              full  => 'rx_full',
                          },
                          parameter_map =>
                          {
                              lpm_width               => $rx_data_bits,
                              lpm_numwords            => $Options->{fifo_size_rx},
			      lpm_widthu			  => $rx_fifo_address_bits,
			      lpm_type				  => qq("scfifo"),
			      lpm_showahead			  => qq("ON"),
			      overflow_checking	  => qq("ON"),
			      underflow_checking	  => qq("ON"),
			      use_eab			  => $rx_Use_EAB,
                         },
                        }), # wfifo instance
                       ], # contents
                 }), # e_module scfifo_w
         }); # e_instance wfifo
#  my $in_rx_port_map = {
#              rdreq   => 'd1_rx_fifo_rd_strobe',
#              sclr	  => 'reset',
#              clock   => 'clk',
#              wrreq	  => 'd1_rx_rd_strobe',
#              data    => 'in_rx_data',
#             };
#
#  my $out_rx_port_map = {
#              q       => 'rx_data_b',
#              usedw    => 'rx_used',
#              empty => 'rx_empty',
#              full  => 'rx_full',
#
#             };
#  my $rx_Use_EAB = $Options->{rx_fifo_LE} ? qq("OFF") : qq("ON");
#
#  my $rx_parameter_map = {
#              lpm_width               => $Options->{data_bits},
#               lpm_width               => $rx_data_bits,
#              lpm_numwords            => $Options->{fifo_size_rx},
#			  lpm_widthu			  => $rx_fifo_address_bits,
#			  lpm_type				  => qq("scfifo"),
#			  lpm_showahead			  => qq("ON"),
#			  overflow_checking	  => qq("ON"),
#			  underflow_checking	  => qq("ON"),
#			  use_eab			  => $rx_Use_EAB,
#			  };
#  e_blind_instance->add({
#             tag            => 'normal',
#             use_sim_models => 1,
#             name           => 'read_fifo',
#             module         => 'scfifo',
#             in_port_map    => $in_rx_port_map,
#             out_port_map   => $out_rx_port_map,
#             parameter_map  => $rx_parameter_map,
#          });
  e_signal->add(["rx_buff_used", $rx_fifo_address_bits+1]);
  e_signal->add(["rx_used", $rx_fifo_address_bits]);
  e_register->add ({out => "rx_empty_reg",
                       in  => "rx_empty",
                    })if  $Options->{add_error_bits};

  e_signal->add(["rx_data_b", $rx_data_bits]);

  e_assign->add(["rx_buff_used", "\{rx_full,rx_used\}"]);
  e_assign->add(["rx_not_empty", "~rx_empty"]);
# this is new.  look to see if rx_fifo is to the rigth threashold before sending the interrupt.
  e_assign->adds
#       ([e_signal->new ({name => "rx_at_threshold", never_export => 1}),  "(rx_used >=$Options->{rx_IRQ_Threshold}) || timer_timout" ] )if  $Options->{use_rx_fifo};
       ([e_signal->new ({name => "rx_at_threshold", never_export => 1}), "(rx_used >=$Options->{rx_IRQ_Threshold}) || rx_full || timer_timout" ] )if $Options->{use_rx_fifo};

  e_assign->add(["rx_not_full", "~rx_full"]);


  #gap detection logic.
#  e_assign->adds(["gap_timeout","gap_counter[7:3] >= $Options->{gap_value}" ] ) if  $Options->{use_gap_detection};
  e_register->add ({out => "gap_timeout",
#                     in  => "gap_counter[7:3] >= $Options->{gap_value}",
                       in  => "(gap_counter[18:3] == gap_reg) && !old_gap_detect || (gap_timeout && !status_wr_strobe)",
                    })if  $Options->{use_gap_detection};

   if  ( $Options->{use_gap_detection} ) {
         e_register->add ({out => "old_gap_detect",
                       in  => "gap_counter[18:3] == gap_reg  || (old_gap_detect && !rx_rd_strobe)",
                    })if  $Options->{use_gap_detection};

                    # could use rx_char_ready  it might be better
         e_register->add ({
                       out        => e_signal->add({name  => "gap_counter",
                                   width => 19}),
                       in         => "gap_counter + ((baud_clk_en && !gap_detect) ? 1 : 0) ",
#            enable     => "baud_clk_en && !timer_counter_max",
#            sync_set   => "status_wr_strobe || ( d1_rx_rd_strobe && !gap_timeout)",
             sync_set   => "status_wr_strobe ||  d1_rx_rd_strobe ",
             set_value  => "0",
             comment =>"gap_counter counts the number of 8 bit chars that have elapse since the last char arrived. ",
       });
     }
  # need to add a timmer so we don't orphan any chars in the buffer.
  e_assign->adds(["timer_timout","timer_counter[7:3] >= $Options->{timeout_value}" ] ) if  $Options->{use_timout};
   e_assign->adds(["timer_timout","0" ] ) if  !$Options->{use_timout};

   # three differnt block need this baud_clk_enable; timeout, gap detection, internal timestamp.
   if  ( $Options->{use_timout} || $Options->{use_gap_detection} || ($Options->{use_timestamp} && $Options->{use_ext_timestamp}) ) {
       e_register->add ({
      out        => e_signal->add({name  => "baud_rate_counter",
                                   width => $Options->{divisor_bits}
                                  }),
      in         => "baud_rate_counter - 1",
      sync_set   => "baud_rate_counter_is_zero ",
      set_value  => "baud_divisor",
      comment =>"count down from the uart clock to create a baud clock enable",
    });

    e_assign->add(["baud_rate_counter_is_zero", "baud_rate_counter == 0"]);


    # need this value for reg module -  no need to regenerate it.
    e_register->add ({out    => "baud_clk_en",
                    in     => "baud_rate_counter_is_zero",
                   });
   }

    if  ( $Options->{use_timout}  ) {

     e_register->add ({
      out        => e_signal->add({name  => "timer_counter",
                                   width => 8}),
      in         => "timer_counter + ((baud_clk_en && !timer_timout) ? 1 : 0) ",
#      enable     => "baud_clk_en && !timer_counter_max",
      sync_set   => "rx_empty || d1_rx_rd_strobe ",
      set_value  => "0",
      comment =>"Timer counter counts the number of baudrate/8 chars that have elapsed while a char is in the fifo",
     });
#     e_assign->adds(["timer_counter_max", "timer_counter >= 248"]);

     }
}    # end if use fifo

  if ($Options->{fifo_export_used} && $Options->{use_rx_fifo}) {
      e_port->add(["rxused", ($rx_fifo_address_bits + 1), "out"]);
      e_assign->add(["rxused", "rx_buff_used"]);
  }
  if ($Options->{fifo_export_used} && $Options->{use_tx_fifo}) {
      e_port->add(["txused", ($tx_fifo_address_bits + 1), "out"]);
      e_assign->add(["txused", "tx_buff_used"]);
  }
#  if ( $Options->{use_fifo} && $Options->{hw_cts}){
#      e_port_add(["cts_n", 1, "in"]);
#  }

  if ($Options->{use_eop_register}) {
     e_register->add ({
        out         => "eop_status_bit",

        sync_set    => "(rx_rd_strobe && (eop_char_reg == rx_data)) ||
                        (tx_wr_strobe &&
                          (eop_char_reg == writedata\[tx_data.msb:0\]))",
        sync_reset  => "status_wr_strobe",
        async_value => 0,
     });
     e_assign->add
         ([e_port->new (["endofpacket", 1, "out"]), "eop_status_bit"]);
  } else {

     e_assign->add (["eop_status_bit", "1'b0"]);
  }

  my @read_mux_table = (
    "(address == 4'd1)" => "tx_data",
    "(address == 4'd2)" => "status_reg",
    "(address == 4'd3)" => "control_reg",
  );

  push (@read_mux_table, "(address == 4'd0)" => "rx_data")
    if !$Options->{use_rx_fifo};

  push (@read_mux_table, "(address == 4'd0)" => &concatenate ("rx_empty_reg", "rx_data_b"))
    if( $Options->{use_rx_fifo} && $Options->{add_error_bits});

    push (@read_mux_table, "(address == 4'd0)" => "rx_data_b")
    if( $Options->{use_rx_fifo} && !$Options->{add_error_bits});

  push (@read_mux_table, "(address == 4'd4)" => "baud_divisor")
    if !$Options->{fixed_baud};

  push (@read_mux_table, "(address == 4'd5)" => "eop_char_reg")
    if  $Options->{use_eop_register};

  push (@read_mux_table, "(address == 4'd6)" => "rx_buff_used")
    if $Options->{use_rx_fifo};

  push (@read_mux_table, "(address == 4'd7)" => "tx_buff_used")
    if $Options->{use_tx_fifo};

  push (@read_mux_table, "(address == 4'd8)" => "gap_reg")
    if $Options->{use_gap_detection};

  push (@read_mux_table, "(address == 4'd9)" => "timestamp_data")
    if $Options->{use_timestamp};

  e_mux->add ({
    lhs    => e_signal->add(["selected_read_data", 32]),
    table  => \@read_mux_table,
    type   => "and-or",
  });




  my @irq_terms = ();
  push (@irq_terms, "(ie_dcts           && dcts_status_bit )")
    if $Options->{use_cts_rts};
  push (@irq_terms, "(ie_eop            && eop_status_bit  )")
    if $Options->{use_eop_register};

  push (@irq_terms, "(ie_any_error      && any_error      )",
                    "(ie_tx_shift_empty && tx_shift_empty )",
                    "(ie_tx_overrun     && tx_overrun     )",
                    "(ie_rx_overrun     && rx_overrun     )",
                    "(ie_break_detect   && break_detect   )",
                    "(ie_framing_error  && framing_error  )",
                    "(ie_parity_error   && parity_error   )",
       );
       #cal new option here rx_at_ threshold
   push (@irq_terms,"(ie_rx_char_ready  && rx_at_threshold  )") if   $Options->{use_rx_fifo} ;
   push (@irq_terms,"(ie_rx_char_ready  && rx_char_ready  )") if  !$Options->{use_rx_fifo};
#   push (@irq_terms,"(ie_rx_char_ready  && rx_almost_full  )") if  $Options->{use_rx_fifo};
#   push (@irq_terms,"(ie_tx_ready       && tx_not_full    )") if  $Options->{use_tx_fifo};
   push (@irq_terms,"(ie_tx_ready       && tx_ready       )") if  !$Options->{use_tx_fifo};
   push (@irq_terms,"(ie_tx_ready       && tx_almost_empty     )") if  $Options->{use_tx_fifo};
   push (@irq_terms,"(ie_gap_detection  && gap_timeout )");

#  push (@irq_terms, "(ie_any_error      && any_error      )",
#                    "(ie_tx_shift_empty && tx_shift_empty )",
#                    "(ie_tx_overrun     && tx_overrun     )",
#                    "(ie_rx_overrun     && rx_overrun     )",
#                    "(ie_break_detect   && break_detect   )",
#                    "(ie_framing_error  && framing_error  )",
#                    "(ie_parity_error   && parity_error   )",
#                    "(ie_rx_char_ready  && rx_not_empty  )",
#                    "(ie_tx_ready       && tx_not_full    )",
#       )if  $Options->{use_fifo};

  e_assign->add (["qualified_irq", join (" ||\n", @irq_terms)]);

  e_assign->add(["tx_shift_not_empty", "~tx_shift_empty"])if $Options->{trans_pin} ;
  e_assign->adds
       ([e_port->new (["transmitting",  1, "out"]),  "tx_shift_not_empty" ] )if $Options->{trans_pin} ;
  return $module,
}




















sub make_uart_rxd_source
{
  my ($Options) = (@_);
  my $module = e_module->new ({name  => $Options->{rx_source_module_name}});
  my $marker = e_default_module_marker->new ($module);






  e_port->adds(
    ["rxd",           1,                        "in"],
    ["source_rxd",    1,                       "out"],
    ["rx_char_ready", 1,                        "in"],
    ["clk",           1,                        "in"],
    ["clk_en",        1,                        "in"],
    ["reset_n",       1,                        "in"],
    ["rx_char_ready", 1,                        "in"],
    ["baud_divisor",  $Options->{divisor_bits}, "in"],
  );





  e_assign->add({
   tag  => "synthesis",
   lhs  => "source_rxd",
   rhs  => "rxd",
  });





 if($Options->{generate_sim})
  {


  my @dummies = e_signal->adds (
    ["unused_overrun"],
    ["unused_ready"  ],
    ["unused_empty"  ],
  );

  foreach my $dummy_sig (@dummies) {
    $dummy_sig->never_export   (1);
  }

  if (!$Options->{hw_cts}) {
  e_instance->add ({
    module             => $Options->{"tx_module_name"},
    name               => "stimulus_transmitter",
    tag                => "simulation",
    port_map           => {
      txd                => "source_rxd",
      tx_overrun         => "unused_overrun",
      tx_ready           => "unused_ready",
      tx_shift_empty     => "unused_empty",
      do_force_break     => "1'b0",
      status_wr_strobe   => "1'b0",
      tx_data            => "d1_stim_data",
      begintransfer      => "do_send_stim_data",
      tx_wr_strobe       => "1'b1",
    },
  });
  }
  else
  {
    e_instance->add ({
    module             => $Options->{"tx_module_name"},
    name               => "stimulus_transmitter",
    tag                => "simulation",
    port_map           => {
      txd                => "source_rxd",
      tx_overrun         => "unused_overrun",
      tx_ready           => "unused_ready",
      tx_shift_empty     => "unused_empty",
      do_force_break     => "1'b0",
      status_wr_strobe   => "1'b0",
      tx_data            => "d1_stim_data",
      begintransfer      => "do_send_stim_data",
      tx_wr_strobe       => "1'b1",
      cts_n              => "1'b1",
    },
  });
  }



  e_signal->add ({
    tag             => "simulation",
    name            => "stim_data",
    width           => $Options->{data_bits},
  });


  e_register->add ({
    tag    => "simulation",
    in     => "stim_data",
    out    => e_signal->add(["d1_stim_data", $Options->{data_bits}]),
    enable => "do_send_stim_data",
  });
















  my $size = &max ($Options->{mutex_file_size} + 1, 1024);

  e_drom->add ({
     name	    => $module->name()."_character_source_rom",
     rom_size       => $size,
     dat_name	    => $Options->{char_data_file},
     mutex_name	    => $Options->{char_mutex_file},
     interactive    => $Options->{interactive_in},
     relativepath  =>1,
     allow_missing_mutex_file => 1,
     port_map	    => {"q"         => "stim_data",
			"new_rom"   => "new_rom_pulse",
			"incr_addr" => "do_send_stim_data",
		    }
  });





















  e_edge_detector->add ({
    tag             => "simulation",
    out             => "pickup_pulse",
    in              => "rx_char_ready",
    edge            => "falling",
  });





  e_assign->add ({
    tag             => "simulation",
    lhs             => "do_send_stim_data",
    rhs             => "(pickup_pulse || new_rom_pulse) && safe",
  });
  }
  return $module;
}

1;
