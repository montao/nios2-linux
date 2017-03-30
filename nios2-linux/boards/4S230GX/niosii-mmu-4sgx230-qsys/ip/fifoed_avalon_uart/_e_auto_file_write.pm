#Copyright (C)2001-2008 Altera Corporation
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


=head1 NAME

_e_auto_file_write - description of the module goes here ...

=head1 SYNOPSIS

The _e_auto_file_write class implements ... detailed description of functionality

=head1 METHODS

=over 4

=cut

package _e_auto_file_write;

use europa_utils;
use e_expression;

use e_process;
@ISA = qw (e_process);

use strict;

my @outputs = qw ();
my @inputs  = qw (valid strobe data);
my @output_expressions = map {e_expression->new ($_)} @outputs;
map {$_->direction ("output")} @output_expressions;
my @input_expressions  = map {e_expression->new ($_)} @inputs;

my %fields = (
	      contents => [ @output_expressions, @input_expressions ],
	      tag => "simulation",
	      log_file => undef,

              writememb => 0,


              showascii => 0,



              relativepath => 0,
              );

my %pointers = ();

&package_setup_fields_and_pointers
    (__PACKAGE__,
     \%fields, 
     \%pointers,
     );



=item I<new()>

Object constructor

=cut

sub new
{
   my $this = shift;
   $this = $this->SUPER::new(@_);

   my @output_expressions = map {e_expression->new ($_)} @outputs;
   map {$_->direction ("output")} @output_expressions;
   my @input_expressions  = map {e_expression->new ($_)} @inputs;

   map {$_->parent($this);} 
   (@output_expressions, @input_expressions);

   $this->reset('');
   return $this;
}



=item I<file_names()>

method description goes here...
...remember: there must be a newline around each POD tag (e.g. =item, =cut etc)!

=cut

sub file_names
{
    my $this = shift;
    my $module_name = $this->parent_module->name;
    my ($log_name)  = (@_);		# pass in file_name
    my $log_filepath;

    if (! defined ($log_name)) {
	$log_name = $module_name.".txt";
	# print "_e_auto_file_write: file_names: undefined \$log_name ".
	# "set to $log_name\n";
    }

    $log_filepath =
        join ("/",
              $this->parent_module->_project()->{_sopc_modelsim_dir},
              $log_name);
    
    open (L_FILE,">> $log_filepath") || &ribbit ("Cannot create $log_filepath: $!\n");
    close L_FILE;
        
    if ($this->relativepath) {
        return $log_name;
    } else {
        return $log_filepath;
    }
}



=item I<to_verilog()>

method description goes here...
...remember: there must be a newline around each POD tag (e.g. =item, =cut etc)!

=cut

sub to_verilog
{
   my $this  = shift;


   my $log_name = $this->file_names($this->log_file);


   my $vs = "   reg [31:0] text_handle; // for \$fopen";
   
   my $write = $this->writememb ?
       "\$fwrite (text_handle, \"\%b\\n\", data);" :
       "// Send \\n (linefeed) instead of \\r (^M, Carriage Return)...\n".
"         \$fwrite (text_handle, \"\%s\", ((data == 8'hd) ? 8'ha : data));";

   if ($this->writememb && $this->showascii) {
       $write .= qq [
          // echo raw binary strings to file as ascii to screen
         \$write(\"\%s\", ((data == 8'hd) ? 8'ha : data));
                     ];
   }

   $vs .= <<"__END_OF_SOURCE__";

   initial text_handle = \$fopen (\"$log_name\");

   always @(posedge clk) begin
      if (valid && strobe) begin
	 $write
	 // non-standard; poorly documented; required to get real data stream.
	 \$fflush (text_handle);
      end
   end // clk

__END_OF_SOURCE__

   return ($vs);
}



=item I<to_vhdl()>

method description goes here...
...remember: there must be a newline around each POD tag (e.g. =item, =cut etc)!

=cut

sub to_vhdl
{
   my $this  = shift;
   my $class = ref($this) or &ribbit ("this ($this) not understood");

   my $libraries_hash = $this->parent_module->vhdl_libraries;
   $libraries_hash->{std}{textio} = "all";

   my $indent = shift;

   my $name = $this->name();
   my $log_name = $this->file_names($this->log_file);

   my $quoted_string = qq [
  file text_handle : TEXT ;
                           ];

   my $translate_on = $this->project()->_translate_on();
   my $translate_off = $this->project()->_translate_off();



   my $showascii = ((1 == $this->writememb) && (1 == $this->showascii));
   if ( (0 == $this->writememb) || ($showascii) ) {
       $quoted_string .= qq [
  -- $translate_off
  -- purpose: convert 8 bit signal data to 8 bit string
  FUNCTION bin_to_char(vec_to_convert : STD_LOGIC_VECTOR (7 downto 0))
    RETURN CHARACTER IS
    VARIABLE result: CHARACTER;
  BEGIN
    CASE vec_to_convert IS      -- cover basic ascii printable characters...
      when X"0a" => result := lf; -- \\n, linefeed
      when X"0d" => result := nul; -- \\r, Ctrl-M
      when X"09" => result := ht; -- \\t, Ctrl-I, TAB
      when X"20" => result := ' ' ;
      when X"21" => result := '!' ;
      when X"22" => result := '"' ;
      when X"23" => result := '#' ;
      when X"24" => result := '\$' ;
      when X"25" => result := '%' ;
      when X"26" => result := '&' ;
      when X"27" => result := ''' ; -- sync ' char for hilighting txt editors
      when X"28" => result := '(' ;
      when X"29" => result := ')' ;
      when X"2a" => result := '*' ;
      when X"2b" => result := '+' ;
      when X"2c" => result := ',' ;
      when X"2d" => result := '-' ;
      when X"2e" => result := '.' ;
      when X"2f" => result := '/' ;
      when X"30" => result := '0' ;
      when X"31" => result := '1' ;
      when X"32" => result := '2' ;
      when X"33" => result := '3' ;
      when X"34" => result := '4' ;
      when X"35" => result := '5' ;
      when X"36" => result := '6' ;
      when X"37" => result := '7' ;
      when X"38" => result := '8' ;
      when X"39" => result := '9' ;
      when X"3a" => result := ':' ;
      when X"3b" => result := ';' ;
      when X"3c" => result := '<' ;
      when X"3d" => result := '=' ;
      when X"3e" => result := '>' ;
      when X"3f" => result := '?' ;
      when X"40" => result := '\@' ;
      when X"41" => result := 'A' ;
      when X"42" => result := 'B' ;
      when X"43" => result := 'C' ;
      when X"44" => result := 'D' ;
      when X"45" => result := 'E' ;
      when X"46" => result := 'F' ;
      when X"47" => result := 'G' ;
      when X"48" => result := 'H' ;
      when X"49" => result := 'I' ;
      when X"4a" => result := 'J' ;
      when X"4b" => result := 'K' ;
      when X"4c" => result := 'L' ;
      when X"4d" => result := 'M' ;
      when X"4e" => result := 'N' ;
      when X"4f" => result := 'O' ;
      when X"50" => result := 'P' ;
      when X"51" => result := 'Q' ;
      when X"52" => result := 'R' ;
      when X"53" => result := 'S' ;
      when X"54" => result := 'T' ;
      when X"55" => result := 'U' ;
      when X"56" => result := 'V' ;
      when X"57" => result := 'W' ;
      when X"58" => result := 'X' ;
      when X"59" => result := 'Y' ;
      when X"5a" => result := 'Z' ;
      when X"5b" => result := '[' ;
      when X"5c" => result := '\\' ;
      when X"5d" => result := ']' ;
      when X"5e" => result := '^' ;
      when X"5f" => result := '_' ;
      when X"60" => result := '`' ;
      when X"61" => result := 'a' ;
      when X"62" => result := 'b' ;
      when X"63" => result := 'c' ;
      when X"64" => result := 'd' ;
      when X"65" => result := 'e' ;
      when X"66" => result := 'f' ;
      when X"67" => result := 'g' ;
      when X"68" => result := 'h' ;
      when X"69" => result := 'i' ;
      when X"6a" => result := 'j' ;
      when X"6b" => result := 'k' ;
      when X"6c" => result := 'l' ;
      when X"6d" => result := 'm' ;
      when X"6e" => result := 'n' ;
      when X"6f" => result := 'o' ;
      when X"70" => result := 'p' ;
      when X"71" => result := 'q' ;
      when X"72" => result := 'r' ;
      when X"73" => result := 's' ;
      when X"74" => result := 't' ;
      when X"75" => result := 'u' ;
      when X"76" => result := 'v' ;
      when X"77" => result := 'w' ;
      when X"78" => result := 'x' ;
      when X"79" => result := 'y' ;
      when X"7a" => result := 'z' ;
      when X"7b" => result := '{' ;
      when X"7c" => result := '|' ;
      when X"7d" => result := '}' ;
      when X"7e" => result := '~' ;
      when X"7f" => result := '_' ;
      WHEN others =>
        ASSERT False REPORT "data contains a non-printable character" SEVERITY Warning;
        result := nul;
    END case;
    RETURN result;
  end bin_to_char;
  -- $translate_on
                             ];
   }

   $this->parent_module->vhdl_add_string($quoted_string);

   my $vs = $this->string_to_vhdl_comment($indent,$this->comment());
   my $incremental_indent = $this->indent();

   my $clock = $this->clock()->to_vhdl();
   my $reset_expr = $this->reset();
   my $r_p = $reset_expr->parent();

   my $reset = $this->reset()->to_vhdl();

   my $tempy;

   if($clock =~ /(["'])(\d+)(['"])/)
   {
     my $pm = $this->parent_module();
     my $new_name = $pm->get_exclusive_name("clock_input");
     my $new_signal =  e_signal->new([$new_name, $this->clock()->width(),0,1]);
     my $new_assignment = e_assign->new({
					 lhs => $new_signal,
					 rhs => $this->clock,
					 tag => $this->tag(),
					});

     $new_assignment->update($this);
     $clock = $new_name;

     push(@{$this->_vhdl_fixes()},$new_assignment->to_vhdl());
   }



   if($reset =~ /(["'])(\d+)(['"])/)
   {
     my $pm = $this->parent_module();
     my $new_name = $pm->get_exclusive_name("reset_input");
     my $new_signal =  e_signal->new([$new_name, $this->reset()->width(),0,1]);
     my $new_assignment = e_assign->new({
					 lhs => $new_signal,
					 rhs => $this->reset,
					 tag => $this->tag(),
					});

     $new_assignment->update($this);
     $reset = $new_name;
     push(@{$this->_vhdl_fixes()},$new_assignment->to_vhdl());
   }

   my @condition_list;
   push (@condition_list, $this->clock()->
         _get_all_signal_names_in_expression());

   push (@condition_list, $this->reset()->
         _get_all_signal_names_in_expression());




   my $clock_level = $this->clock_level();
   my $reset_level = $this->reset_level();
   my $contents_vs = qq[

  -- purpose: simulate verilog initial function to open file in write mode
  -- type   : combinational
  -- inputs : initial
  -- outputs: <none>
  process is
    variable initial : boolean := true; -- not initialized yet
    variable status : file_open_status; -- status for fopen
  begin  -- process
    if initial = true then
      file_open (status, text_handle, \"$log_name\", WRITE_MODE);
      initial := false;                 -- done!
    end if;
    wait;                               -- wait forever
  end process;

  process (clk)
    variable data_string : LINE;        -- for line buffer to file
    variable status : file_open_status; -- status for fopen
                        ];
   if ($showascii) {
       $contents_vs .= qq[
    variable echo_string : LINE;        -- for line buffer to screen (stdout)
                          ];
   }

   $contents_vs .= qq[
  begin  -- process clk
    if clk'event and clk = '1' then -- sync ' chars for hilighting txt editors
      if (valid and strobe) = '1' then
                        ];

   if ($this->writememb) {
       $contents_vs .= qq [
        write (data_string,To_bitvector(data)); -- every char flushes line
        writeline (text_handle,data_string);
        file_close (text_handle);     -- flush buffer
        file_open (status, text_handle, \"$log_name\", APPEND_MODE);
                           ];
   } else {
       $contents_vs .= qq [
        write (data_string,bin_to_char(data));
        if data = X"0a" or data = X"0d" then -- \\n or \\r will flush line
          writeline (text_handle,data_string);
          file_close (text_handle);     -- flush buffer
          file_open (status, text_handle, \"$log_name\", APPEND_MODE);
        end if;
                           ];
   }

   if ($showascii) {
       $contents_vs .= qq[
        -- save up characters into a line to send to the screen
        write (echo_string,bin_to_char(data));
        if data = X"0a" or data = X"0d" then -- \\n or \\r will flush line
          writeline (output,echo_string);
        end if;
                          ];
   }

   $contents_vs .= qq [
      end if;
    end if;
  end process;
                       ];



   return ($vs.$contents_vs);
}



=item I<vhdl_dump_variables()>

method description goes here...
...remember: there must be a newline around each POD tag (e.g. =item, =cut etc)!

=cut

sub vhdl_dump_variables
  {
    my $this = shift;
    my $indent = shift;
    my $vs;

   my @variables = @{$this->_vhdl_variables()};

   foreach my $var (@variables)
     {
       $vs .= "$indent";
       $vs .= "VARIABLE $$var[0] : $$var[1]";
       $vs .= " := $$var[2]"
	 if ($$var[2] ne "");
       $vs .= ";\n";
     }
    return ($vs);
}



=item I<vhdl_dump_files()>

method description goes here...
...remember: there must be a newline around each POD tag (e.g. =item, =cut etc)!

=cut

sub vhdl_dump_files
  {
    my $this = shift;
    my $indent = shift;
    my $vs;

   my @files = @{$this->_vhdl_files()};

   foreach my $var (@files)
     {
       $vs .= "$indent";
       $vs .= "FILE $$var[0] : $$var[1]";
       $vs .= " \"".$$var[2]."\""
	 if ($$var[2] ne "");
       $vs .= ";\n";
     }


    return ($vs);
}



=item I<vhdl_add_file()>

method description goes here...
...remember: there must be a newline around each POD tag (e.g. =item, =cut etc)!

=cut

sub vhdl_add_file
{
  my $this = shift;
  push (@{$this->_vhdl_files()},[@_]);
}



=item I<vhdl_add_variable()>

method description goes here...
...remember: there must be a newline around each POD tag (e.g. =item, =cut etc)!

=cut

sub vhdl_add_variable
{
  my $this = shift;

  push (@{$this->_vhdl_variables()},[@_]);
}



=item I<vhdl_dump_fixes()>

method description goes here...
...remember: there must be a newline around each POD tag (e.g. =item, =cut etc)!

=cut

sub vhdl_dump_fixes
{
  my $this=shift;
  my $vs;
  my @fixes = @{$this->_vhdl_fixes()};

  $vs = join("\n", @fixes);

  return $vs;
}

1;


=back

=cut

=head1 EXAMPLE

Here is a usage example ...

=head1 AUTHOR

Santa Cruz Technology Center

=head1 BUGS AND LIMITATIONS

list them here ...

=head1 SEE ALSO

The inherited class e_process

=begin html

<A HREF="e_process.html">e_process</A> webpage

=end html

=head1 COPYRIGHT

Copyright (C)2001-2005 Altera Corporation, All rights reserved.

=cut

1;
