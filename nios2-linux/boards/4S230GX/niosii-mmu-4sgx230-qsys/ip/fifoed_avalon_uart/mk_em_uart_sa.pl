
#BEGIN
#{
#  unshift @INC, "$ENV{QUARTUS_ROOTDIR}/sopc_builder/bin";
#  unshift @INC, "$ENV{QUARTUS_ROOTDIR}/sopc_builder/bin/europa";
#}

use europa_all;
use strict;
use mk_em_uart;

#use Getopt::Long;
use Getopt::Long;


sub pretty_print($$)
{
  my $param = shift;
  my $optional = shift;

  my $type_spec = "<$param->{type}>";
  for ($param->{type})
  {
    /^s$/ and do {$type_spec = "<string>"; last;};
    /^i$/ and do {$type_spec = "<integer>"; last;};
    /^f$/ and do {$type_spec = "<float>"; last;};
  }
  my $l_delimit = "";
  my $r_delimit = "";
  my $default_value = "";
  if ($optional)
  {
    $l_delimit = '[';
    $r_delimit = ']';
    $default_value = " ($param->{value})";
  }

  my $str =
    "  $l_delimit--$param->{name}=$type_spec$r_delimit " .
    " - $param->{desc}$default_value\n";

  return $str;
}

sub print_usage_and_fail($$)
{
  my ($req, $opt) = @_;

  my $base = `basename $0`;
  1 while chomp $base;

  print STDERR "Usage:\n\n$base\n",
    (map {pretty_print($_, 0)} @$req),
    (map {pretty_print($_, 1)} @$opt),
    "\n";
  exit 1;
}

my $start_time = time();
#print "Hello I was here" ;
# Parameter specifications.
# Most fields are self-explanatory.  The "dest" field
# takes one of three values:
#   1) "module": the parameter is passed directly to e_module->new().
#   2) "project": the parameter is passed to e_project->new().
#   3) "wsa": the parameter is passed to the component generator subroutine.
#     --_system_directory=$outdir \
#      --language=$language \
#      --name=$output_name \
#      --clock_freq=$clock_freq \
#      --use_tx_fifo=$use_tx_fifo \
#      --use_rx_fifo=$use_rx_fifo \
#   X   --baud=$baud \
#    X  --data_bits=$data_bits \
#   X   --fixed_baud=$fixed_baud \
#  X    --parity=$parity \
#   X   --stop_bits=$stop_bits \
#  X    --use_cts_rts=$use_cts_rts \
#   X   --use_eop_register=$use_eop_register \
#   X   --sim_true_baud=$sim_true_baud \
#   X   --sim_char_stream=$sim_char_stream \
#  X    --fifo_export_used=$fifo_export_used \
#  x    --Has_IRQ=$Has_IRQ \
#      --hw_cts=$hw_cts \
#  X    --trans_pin=$trans_pin \
#  X    --fifo_size_tx=$fifo_size_tx \
#  X    --fifo_size_rx=$fifo_size_rx \
#
my @required_parameters = (
{
    name => 'trans_pin',
    type => 'i',
    value => undef,
    dest => 'wsa',
    desc => "",
  },
{
    name => 'fifo_size_tx',
    type => 'i',
    value => undef,
    dest => 'wsa',
    desc => "",
  },
{
    name => 'hw_cts',
    type => 'i',
    value => undef,
    dest => 'wsa',
    desc => "",
  },
{
    name => 'fifo_size_rx',
    type => 'i',
    value => undef,
    dest => 'wsa',
    desc => "",
  },
{
    name => 'Has_IRQ',
    type => 'i',
    value => undef,
    dest => 'wsa',
    desc => "",
  },
{
    name => 'fifo_export_used',
    type => 'i',
    value => undef,
    dest => 'wsa',
    desc => "",
  },


 {
     name => 'sim_true_baud',
     type => 'i',
     value => undef,
     dest => 'wsa',
     desc => "",
   },
   {
     name => 'use_eop_register',
     type => 'i',
     value => undef,
     dest => 'wsa',
     desc => "",
   },
   {
     name => 'use_cts_rts',
     type => 'i',
     value => undef,
     dest => 'wsa',
     desc => "",
   },
   {
     name => 'stop_bits',
     type => 'i',
     value => undef,
     dest => 'wsa',
     desc => "",
   },
  {
    name => 'parity',
    type => 's',
    value => undef,
    dest => 'wsa',
    desc => "",
  },
 {
    name => 'fixed_baud',
    type => 'i',
    value => undef,
    dest => 'wsa',
    desc => "",
  },
{
    name => 'data_bits',
    type => 'i',
    value => undef,
    dest => 'wsa',
    desc => "",
  },
{
    name => 'baud',
    type => 'i',
    value => undef,
    dest => 'wsa',
    desc => "",
  },
   {
     name => 'name',
     type => 's',
     value => "fifoed_avalon_uart_1",
     dest => 'module',
     desc => "component module name",
   },
   {
     name => 'clock_freq',
     type => 'i',
     value => undef,
     dest => 'wsa',
     desc => "",
   },
   {
     name => 'use_tx_fifo',
     type => 'i',
     value => undef,
     dest => 'wsa',
     desc => "",
   },
   {
     name => 'use_rx_fifo',
     type => 'i',
     value => undef,
     dest => 'wsa',
     desc => "",
   },
   {
     name => 'use_timout',
     type => 'i',
     value => undef,
     dest => 'wsa',
     desc => "",
   },
   {
     name => 'timeout_value',
     type => 'i',
     value => undef,
     dest => 'wsa',
     desc => "",
   },
   {
     name => 'tx_IRQ_Threshold',
     type => 'i',
     value => undef,
     dest => 'wsa',
     desc => "",
   },
   {
     name => 'rx_IRQ_Threshold',
     type => 'i',
     value => undef,
     dest => 'wsa',
     desc => "",
   },
   {
     name => 'rx_fifo_LE',
     type => 'i',
     value => undef,
     dest => 'wsa',
     desc => "",
   },
     {
     name => 'tx_fifo_LE',
     type => 'i',
     value => undef,
     dest => 'wsa',
     desc => "",
   },
   {
     name => 'use_gap_detection',
     type => 'i',
     value => undef,
     dest => 'wsa',
     desc => "",
   },
   {
     name => 'gap_value',
     type => 'i',
     value => undef,
     dest => 'wsa',
     desc => "",
   },
   {
     name => 'use_timestamp',
     type => 'i',
     value => undef,
     dest => 'wsa',
     desc => "",
   },
   {
     name => 'use_ext_timestamp',
     type => 'i',
     value => undef,
     dest => 'wsa',
     desc => "",
   },
   {
     name => 'timestamp_width',
     type => 'i',
     value => undef,
     dest => 'wsa',
     desc => "",
   },
   {
     name => 'add_error_bits',
     type => 'i',
     value => undef,
     dest => 'wsa',
     desc => "",
   },
   {
     name => '_system_directory',
     type => 's',
     value => undef,
     dest => 'project',
     desc => "HDL file output directory",
   },
      {
     name => 'generate_sim',
     type => 'i',
     value => undef,
     dest => 'wsa',
     desc => "create simulation models",
   },
 );

# Optional paramters, with defaults.
my @optional_parameters = (
  {
    name => 'language',
    type => 's',
    value => 'verilog',
    dest => 'project',
    desc => "HDL output language",
  },
  # this is just a file now.
  {
     name => 'sim_char_file',
     type => 's',
     value => "",
     dest => 'wsa',
     desc => "",
   },
      {
     name => 'sim_directory',
     type => 's',
     value => "",
     dest => 'wsa',
     desc => "Simualtion Directory",
   },
);

my @params =
  sort {$a->{name} cmp $b->{name}}
    (@required_parameters, @optional_parameters);
#print "Hello I was here a" ;

Getopt::Long::GetOptions(
#GetOptions(
  map {("$_->{name}=$_->{type}" => \$_->{value})} @params
);

for my $req (@required_parameters)
{
  if (!defined $req->{value})
  {
    print STDERR "$0: missing required parameter '--$req->{name}'\n";
    print_usage_and_fail(\@required_parameters, \@optional_parameters);
  }
}

#print "Hello I was here b" ;
# Unpack the parameters and route them to their various destinations.
my $WSA = {
  map {$_->{dest} eq 'wsa' ? ($_->{name} => $_->{value}) : ()}  @params
};
#print "Hello I was here c" ;
my $top = e_module->new(
{
  do_ptf => 0, # This avoids a warning about old-style ptf format.
  (map {$_->{dest} eq 'module' ? ($_->{name} => $_->{value}) : ()} @params),
});
#print "Hello I was here d" ;
my $project = e_project->new({
  top => $top,
  map {$_->{dest} eq 'project' ? ($_->{name} => $_->{value}) : ()} @params
});
#print "Hello I was here e" ;
 my $parameters_message =
   join('', (map {"   --$_->{name}=$_->{value}\n"} @params));

 print "\nGenerating FIFOED uart with these parameters:\n\n",
   $parameters_message,
   "\n";
# print "\nGenerating FIFOED uart with WSA:\n\n",
#   $WSA,
#   "\n";
#print "Hello I was here f" ;
#
# # It's pretty handy to have a list of the parameters used for generation, right
# # in the generated HDL file.
 $top->comment("Generation parameters:\n" . $parameters_message);

# print "Output file: ", $project->hdl_output_filename(), "\n";
#print "Hello I was here g" ;
mk_em_uart::make($project, $WSA);
#print "Hello I was here h" ;
 my $run_time = time() - $start_time;

 print(
   "Generation time: about $run_time second%s\n\n",
   ($run_time == 1) ? "" : "s"
 );

