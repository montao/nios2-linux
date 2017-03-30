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

_e_auto_file_read - description of the module goes here ...

=head1 SYNOPSIS

The _e_auto_file_read class implements ... detailed description of functionality

=head1 METHODS

=over 4

=cut

package _e_auto_file_read;

use europa_utils;
use e_expression;

use e_process;
@ISA = qw (e_process);

use strict;

my @outputs = qw (address pre mutex mem_array);
my @inputs  = qw (incr_addr address mutex mem_array);

my %fields = (

              dat_name => undef, 

              mutex_name => undef,

	      interactive=> 0,
	      # default to non-interactive unless forced.
              tag => "simulation",

              readmemb => 0,


              addrbits => 11,



              relativepath => 0,




              allow_missing_mutex_file => 0,
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
   my $this = $this->SUPER::new(@_);

   my @output_expressions = map {e_expression->new ($_)} @outputs;
   map {$_->direction ("output")} @output_expressions;
   my @input_expressions  = map {e_expression->new ($_)} @inputs;

   map {$_->parent($this);} 
   (@output_expressions, @input_expressions);
   return $this;
}



=item I<file_names()>

method description goes here...
...remember: there must be a newline around each POD tag (e.g. =item, =cut etc)!

=cut

sub file_names
{
    my $this = shift;
    my ($mutex_name, $stream_name) = (@_); # pass in filenames
    my $module_name = $this->parent_module->name;
    my $mutex_filepath;
    my $stream_filepath;


    if (! defined ($mutex_name)) {
        $mutex_name  = $module_name."_mutex.dat";
    }
    if (! defined ($stream_name)) {
        $stream_name = $module_name."_stream.dat";


    }
    
    ($mutex_filepath, $stream_filepath) = map { 
        join ("/",$this->parent_module->_project()->{_sopc_modelsim_dir},$_)
    } ($mutex_name, $stream_name);

    open (M_FILE,">> $mutex_filepath") ||
        &ribbit ("Cannot create $mutex_filepath: $!\n");
    close M_FILE;

    open (S_FILE,">> $stream_filepath") ||
        &ribbit ("Cannot create $stream_filepath $!\n");
    close S_FILE;


    if ($this->relativepath) {
        return ($mutex_name, $stream_name);
    } else {
        return ($mutex_filepath, $stream_filepath);
    }
}



=item I<to_verilog()>

method description goes here...
...remember: there must be a newline around each POD tag (e.g. =item, =cut etc)!

=cut

sub to_verilog
{
   my $this  = shift;


   my ($mutex_name, $stream_name) =
       $this->file_names($this->mutex_name, $this->dat_name);

   my $interactive = $this->interactive;

   my $readmem = ($this->readmemb) ? "\$readmemb" : "\$readmemh";
   
   my $allow_missing_mutex_file = $this->allow_missing_mutex_file;
   my $if_for_missing_mutex    = ($allow_missing_mutex_file)? 'if (mutex_handle) begin' : '';
   my $endif_for_missing_mutex = ($allow_missing_mutex_file)? 'end'                     : '';

   my $vs;
   if ($this->readmemb) {
       $vs = qq[
   assign     num_bytes = mutex[1];
                ];
   }

   $vs .= <<"__END_OF_SOURCE__";
   reg        safe_delay;
   reg [31:0] poll_count;
   reg [31:0] mutex_handle;
   wire       interactive = 1'b$interactive ; // '
   assign     safe = (address < mutex[1]);

   initial poll_count = POLL_RATE;

   always @(posedge clk or negedge reset_n) begin
      if (reset_n !== 1) begin
         safe_delay <= 0;
      end else begin
         safe_delay <= safe;
      end
   end // safe_delay

   always @(posedge clk or negedge reset_n) begin
      if (reset_n !== 1) begin  // dont worry about null _stream.dat file
         address <= 0;
         mem_array[0] <= 0;
         mutex[0] <= 0;
         mutex[1] <= 0;
         pre <= 0;
      end else begin            // deal with the non-reset case
         pre <= 0;
         if (incr_addr && safe) address <= address + 1;
         if (mutex[0] && !safe && safe_delay) begin
            // and blast the mutex after falling edge of safe if interactive
            if (interactive) begin
               mutex_handle = \$fopen (\"$mutex_name\");
               \$fdisplay (mutex_handle, "0");
               \$fclose (mutex_handle);
               // \$display (\$stime, "\\t%m:\\n\\t\\tMutex cleared!");
            end else begin
               // sleep until next reset, do not bash mutex.
               wait (!reset_n);
            end
         end // OK to bash mutex.
         if (poll_count < POLL_RATE) begin // wait
            poll_count = poll_count + 1;
         end else begin         // do the interesting stuff.
            poll_count = 0;
            $if_for_missing_mutex
            \$readmemh (\"$mutex_name\", mutex);
            $endif_for_missing_mutex
            if (mutex[0] && !safe) begin
            // read stream into mem_array after current characters are gone!
               // save mutex[0] value to compare to address (generates 'safe')
               mutex[1] <= mutex[0];
               // \$display (\$stime, "\\t%m:\\n\\t\\tMutex hit: Trying to read %d bytes...", mutex[0]);
               $readmem(\"$stream_name\", mem_array);
               // bash address and send pulse outside to send the char:
               address <= 0;
               pre <= -1;
            end // else mutex miss...
         end // poll_count
      end // reset
   end // posedge clk

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
   my ($mutex_name, $stream_name) =
       $this->file_names($this->mutex_name, $this->dat_name);

   my $interactive;
   if ($this->interactive) {
       $interactive = "TRUE";
   } else {
       $interactive = "FALSE";
   }

   my $readmem = ($this->readmemb) ? "readmemb" : "readmemh";
   
   my $allow_missing_mutex_file = $this->allow_missing_mutex_file;
   my $if_for_missing_mutex    = ($allow_missing_mutex_file)? 'IF (status=OPEN_OK) THEN' : '';
   my $endif_for_missing_mutex = ($allow_missing_mutex_file)? 'END IF;'                  : '';

   my $translate_on = $this->project()->_translate_on();
   my $translate_off = $this->project()->_translate_off();

   my $quoted_string = qq [
          signal safe_wire : STD_LOGIC; -- deal with bogus VHDL type casting
          signal safe_delay : STD_LOGIC; 
          FILE mutex_handle : TEXT ;  -- open this for read and write manually.
          -- stream can be opened simply for read...
          FILE stream_handle : TEXT open READ_MODE is \"$stream_name\";

-- $translate_off
-- convert functions deadlifted from e_rom.pm

FUNCTION convert_string_to_number(string_to_convert : STRING;
                                  final_char_index : NATURAL := 0)
  RETURN NATURAL IS
  VARIABLE result: NATURAL := 0;
  VARIABLE current_index : NATURAL := 1;
  VARIABLE the_char : CHARACTER;
  
BEGIN
  IF final_char_index = 0 THEN
    result := 0;
  ELSE
    WHILE current_index <= final_char_index LOOP
      the_char := string_to_convert(current_index);
      IF    '0' <= the_char AND the_char <= '9' THEN
        result := result * 16 + character'pos(the_char) - character'pos('0');
      ELSIF 'A' <= the_char AND the_char <= 'F' THEN
        result := result * 16 + character'pos(the_char) - character'pos('A') + 10;
      ELSIF 'a' <= the_char AND the_char <= 'f' THEN
        result := result * 16 + character'pos(the_char) - character'pos('a') + 10;
      ELSE
        report "convert_string_to_number: Ack, a formatting error!";
      END IF;
      current_index := current_index + 1;
    END LOOP;
  END IF; 
  RETURN result;
END convert_string_to_number;


FUNCTION convert_string_to_std_logic(value : STRING; num_chars : INTEGER; mem_width_chars : INTEGER)
  RETURN STD_LOGIC_VECTOR is                       
  VARIABLE num_bits: integer := mem_width_chars * 4;
  VARIABLE result: std_logic_vector(num_bits-1 downto 0);
  VARIABLE curr_char : integer;
  VARIABLE min_width : integer := mem_width_chars;
  VARIABLE num_nibbles : integer := 0;
  
BEGIN
  result := (others => '0');
  num_nibbles := mem_width_chars;
  IF (mem_width_chars > num_chars) THEN
    num_nibbles := num_chars;
  END IF;
  
  FOR I IN 1 TO num_nibbles LOOP
    curr_char := num_nibbles - (I-1);
    
    CASE value(I) IS
      WHEN '0' =>  result((4*curr_char)-1  DOWNTO 4*(curr_char-1)) := "0000";
      WHEN '1' =>  result((4*curr_char)-1  DOWNTO 4*(curr_char-1)) := "0001";
      WHEN '2' =>  result((4*curr_char)-1  DOWNTO 4*(curr_char-1)) := "0010";
      WHEN '3' =>  result((4*curr_char)-1  DOWNTO 4*(curr_char-1)) := "0011";
      WHEN '4' =>  result((4*curr_char)-1  DOWNTO 4*(curr_char-1)) := "0100";
      WHEN '5' =>  result((4*curr_char)-1  DOWNTO 4*(curr_char-1)) := "0101";
      WHEN '6' =>  result((4*curr_char)-1  DOWNTO 4*(curr_char-1)) := "0110";
      WHEN '7' =>  result((4*curr_char)-1  DOWNTO 4*(curr_char-1)) := "0111";
      WHEN '8' =>  result((4*curr_char)-1  DOWNTO 4*(curr_char-1)) := "1000";
      WHEN '9' =>  result((4*curr_char)-1  DOWNTO 4*(curr_char-1)) := "1001";
      WHEN 'A' | 'a' => result((4*curr_char)-1  DOWNTO 4*(curr_char-1)) := "1010";
      WHEN 'B' | 'b' => result((4*curr_char)-1  DOWNTO 4*(curr_char-1)) := "1011";
      WHEN 'C' | 'c' => result((4*curr_char)-1  DOWNTO 4*(curr_char-1)) := "1100";
      WHEN 'D' | 'd' => result((4*curr_char)-1  DOWNTO 4*(curr_char-1)) := "1101";
      WHEN 'E' | 'e' => result((4*curr_char)-1  DOWNTO 4*(curr_char-1)) := "1110";
      WHEN 'F' | 'f' => result((4*curr_char)-1  DOWNTO 4*(curr_char-1)) := "1111";
      WHEN ' ' => EXIT;
      WHEN HT  => exit;
      WHEN others =>
        ASSERT False
          REPORT "function From_Hex: string """ & value & """ contains non-hex character"
          severity Error;
        EXIT;
    END case;
  END loop;
  RETURN result;
END convert_string_to_std_logic;

-- purpose: open mutex/discard \@address/convert value to std_logic_vector
function get_mutex_val (file_name : string)
  return STD_LOGIC_VECTOR is
  VARIABLE result : STD_LOGIC_VECTOR (31 downto 0) := X"00000000";
  FILE handle : TEXT ;
  VARIABLE status : file_open_status; -- status for fopen
  VARIABLE data_line : LINE;
  VARIABLE the_character_from_data_line : CHARACTER;
  VARIABLE converted_number : NATURAL := 0;
  VARIABLE found_string_array : STRING(1 TO 128);
  VARIABLE string_index : NATURAL := 0;
  VARIABLE line_length : NATURAL := 0; 
 
begin  -- get_mutex_val

  file_open (status, handle, file_name, READ_MODE);
  $if_for_missing_mutex

  WHILE NOT(endfile(handle)) LOOP
    readline(handle, data_line);
    line_length := data_line'LENGTH; -- match ' for emacs font-lock
    
    WHILE line_length > 0 LOOP
      read(data_line, the_character_from_data_line);
      -- check for the \@ character indicating a new address wad
      -- if found, ignore the line!  This is just protection
      IF '\@' = the_character_from_data_line THEN
        exit;                           -- bail out of this line
      end if;
      -- process the hex address, character by character ...
      IF NOT(' ' = the_character_from_data_line) THEN
        string_index := string_index + 1;
        found_string_array(string_index) := the_character_from_data_line;
      END IF;
      line_length := line_length - 1; 
    end loop;                           -- read characters

  end loop;                             -- read lines
  $endif_for_missing_mutex
  file_close (handle);
  
  if string_index /= 0 then
    result := convert_string_to_std_logic(found_string_array, string_index, 8);
  end if;

  return result;
  
end get_mutex_val;

-- purpose: emulate verilogs readmemh function (mostly)
-- in verilog you say: \$readmemh ("file", array);
-- in VHDL, we say: array <= readmemh("file"); -- which makes more sense.
function readmemh (file_name : string)
  return mem_type is
  VARIABLE result : mem_type;
  FILE handle : TEXT ;
  VARIABLE status : file_open_status; -- status for fopen
  VARIABLE data_line : LINE;
  VARIABLE b_address : BOOLEAN := FALSE; -- distinguish between addrs and data
  VARIABLE the_character_from_data_line : CHARACTER;
  VARIABLE converted_number : NATURAL := 0;
  VARIABLE found_string_array : STRING(1 TO 128);
  VARIABLE string_index : NATURAL := 0;
  VARIABLE line_length : NATURAL := 0; 
  VARIABLE load_address : NATURAL := 0;
  VARIABLE mem_index : NATURAL := 0;
begin  -- readmemh

  file_open (status, handle, file_name, READ_MODE);

  WHILE NOT(endfile(handle)) LOOP
    readline(handle, data_line);
    line_length := data_line'LENGTH; -- match ' for emacs font-lock
    b_address := false;

    WHILE line_length > 0 LOOP
      read(data_line, the_character_from_data_line);
      -- check for the \@ character indicating a new address wad
      -- if found, ignore the line!  This is just protection
      IF '\@' = the_character_from_data_line and not b_address then -- is addr
        b_address := true;
      end if;
      -- process the hex address, character by character ...
      IF NOT((' ' = the_character_from_data_line) or
	     ('\@' = the_character_from_data_line) or
             (lf = the_character_from_data_line) or
	     (cr = the_character_from_data_line)) THEN
        string_index := string_index + 1;
        found_string_array(string_index) := the_character_from_data_line;
      END IF;
      line_length := line_length - 1;
    end loop;                           -- read characters

    if b_address then
      mem_index := convert_string_to_number(found_string_array, string_index);
      b_address := FALSE;
    else
      result(mem_index) := convert_string_to_std_logic(found_string_array, string_index, 2);
    end if;

    string_index := 0;

  end loop;                             -- read lines

  file_close (handle);
  
  return result;
  
end readmemh;
                           ];


   if ($this->readmemb) {
       $quoted_string .= qq [

-- purpose: emulate verilogs readmemb function (mostly)
-- in verilog you say: \$readmemb ("file", array);
-- in VHDL, we say: array <= readmemb("file"); -- which makes more sense.
function readmemb (file_name : string)
  return mem_type is
  VARIABLE result : mem_type;
  FILE handle : TEXT ;
  VARIABLE status : file_open_status; -- status for fopen
  VARIABLE data_line : LINE;
  VARIABLE the_character_from_data_line : BIT_VECTOR(7 DOWNTO 0); -- '0' & '1's
  VARIABLE line_length : NATURAL := 0; 
  VARIABLE mem_index : NATURAL := 0;
begin  -- readmemb

  file_open (status, handle, file_name, READ_MODE);

  WHILE NOT(endfile(handle)) LOOP
    readline(handle, data_line);
    line_length := data_line'LENGTH; -- match ' for emacs font-lock

    WHILE line_length > 7 LOOP
      read(data_line, the_character_from_data_line);
      -- No \@ characters allowed in binary/bit_vector mode
      result(mem_index) := To_stdlogicvector(the_character_from_data_line);
      mem_index := mem_index + 1;
      line_length := line_length - 8;
    end loop;                           -- read characters

  end loop;                             -- read lines

  file_close (handle);
  
  return result;
  
end readmemb;
                          ]; # close quoted string
   } # if (readmemb)

   $quoted_string .= qq [
-- $translate_on
                      ]; # close quoted_string

   $this->parent_module->vhdl_add_string($quoted_string);

   my $vs = $this->string_to_vhdl_comment($indent,$this->comment());
   my $incremental_indent = $this->indent();

   my $clock = $this->clock()->to_vhdl();
   my $reset_expr = $this->reset();
   my $r_p = $reset_expr->parent();

   my $reset = $this->reset()->to_vhdl();

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

   my $reset_addr_string = "0" x $this->addrbits;
   my $incr__addr_string = "0" x ($this->addrbits - 1) . "1";

   my $contents_vs;
   if ($this->readmemb) {
       $contents_vs = qq[
    num_bytes <= mutex(1);
                         ];
   }
   $contents_vs .= qq [
    safe <= safe_wire;
    safe_wire <= to_std_logic( address < mutex(1) );

    process (clk, reset_n)
    begin
      if reset_n = '0' then
        safe_delay <= '0';
      elsif clk'event and clk = '1' then -- balance ' for emacs quoting
        safe_delay <= safe_wire;
      end if;
    end process;

    process (clk, reset_n)
      variable poll_count : integer := POLL_RATE; -- STD_LOGIC_VECTOR (31:0);
      variable status : file_open_status; -- status for fopen
      variable mutex_string : LINE;  -- temp space for read/write data
      variable stream_string : LINE;  -- temp space for read data
      variable init_done : BOOLEAN; -- only used if non-interactive
      variable interactive : BOOLEAN := $interactive;
    begin
      if reset_n /= '1' then
        address <= "$reset_addr_string";
        mem_array(0) <= X"00";
        mutex(0) <= X"00000000";
        mutex(1) <= X"00000000";
        pre <= '0';
        init_done := FALSE;
      elsif clk'event and clk = '1' then -- balance ' for emacs quoting
        pre <= '0';
        if incr_addr = '1' and safe_wire = '1' then
          address <= address + "$incr__addr_string";
        end if;
        -- blast mutex via textio after falling edge of safe
        if mutex(0) /= X"00000000" and safe_wire = '0' and safe_delay = '1' then
	  if interactive then           -- bash mutex
            file_open (status, mutex_handle, \"$mutex_name\", WRITE_MODE);
            write (mutex_string, string'("0")); -- balance ' for emacs quoting
            writeline (mutex_handle, mutex_string);
            file_close (mutex_handle);
            mutex(0) <= X"00000000";
	  else -- non-nteractive does not bash mutex: it stops poll counter
	    init_done := TRUE;
          end if;
        end if;
        if poll_count < POLL_RATE then  -- wait
          if not init_done then         -- stop counting if init_done is TRUE
            poll_count := poll_count + 1;
          end if;
        else                            -- do the real work
          poll_count := 0;
          -- get mutex via textio ...
          mutex(0) <= get_mutex_val (\"$mutex_name\");
          if mutex(0) /= X"00000000" and safe_wire = '0' then
            -- read stream into array after previous stream is complete
            mutex (1) <= mutex (0); -- save mutex value for address compare
            -- get mem_array via textio ...
            mem_array <= $readmem(\"$stream_name\");
            -- prep address and pre-pulse to alert world to new contents
            address <= "$reset_addr_string";
            pre <= '1';
          end if; -- poll_count
        end if;   -- clock
      end if;     -- reset
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
