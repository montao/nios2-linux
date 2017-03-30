# +----------------------------------------------------------------------------+
# | Copyright (C)2001-2008 Altera Corporation                                  |
# |  Any megafunction design, and related net list (encrypted or decrypted),   |
# |  support information, device programming or simulation file, and any other |
# |  associated documentation or information provided by Altera or a partner   |
# |  under Altera's Megafunction Partnership Program may be used only to       |
# |  program PLD devices (but not masked PLD devices) from Altera.  Any other  |
# |  use of such megafunction design, net list, support information, device    |
# |  programming or simulation file, or any other related documentation or     |
# |  information is prohibited for any other purpose, including, but not       |
# |  limited to modification, reverse engineering, de-compiling, or use with   |
# |  any other silicon devices, unless such use is explicitly licensed under   |
# |  a separate agreement with Altera or a megafunction partner.  Title to     |
# |  the intellectual property, including patents, copyrights, trademarks,     |
# |  trade secrets, or maskworks, embodied in any such megafunction design,    |
# |  net list, support information, device programming or simulation file, or  |
# |  any other related documentation or information provided by Altera or a    |
# |  megafunction partner, remains with Altera, the megafunction partner, or   |
# |  their respective licensors.  No other licenses, including any licenses    |
# |  needed under any third party's intellectual property, are provided herein.|
# |  Copying or modifying any file, or portion thereof, to which this notice   |
# |  is attached violates this copyright.                                      |
# +----------------------------------------------------------------------------+



# +----------------------------------------------------------------------------+
# | file: UP_Argument_Parser.pm                                                |
# |                                                                            |
# | This SOPC Builder Generator program is provided by                         |
# | Altera's University Program.                                               |
# |                                                                            |
# | Its purpose is to construct the HDL files for                              |
# | a particular instance of a particular University                           |
# | Program SOPC Builder peripheral.                                           |
# |                                                                            |
# | version: 1.0                                                               |
# |                                                                            |
# +----------------------------------------------------------------------------+

package UP_Argument_Parser;
use Exporter;

@ISA = Exporter;
@EXPORT = qw(
	parse_arguments
	print_verbose
	get_hdl_file_list
	get_ip_core_target_name
	get_parameter_value
	get_section_value
	get_system_directory
);

#use strict;

my $generator_args = {
			hdl_langauge => "",
			system_directory => "",
			target_name => "",

			files => "",
			parameters => "",
			sections => "",

			verbose => 0,
};

my $hdl_file = {
		module_info => {
			name => 0,
			description => "",
		},
    };

sub parse_arguments
{
	if (0) {
		# Clear Dump File
		open (FILE, ">c:/dump.txt") or die "Unable to open file";
		close (FILE);

		$generator_args->{verbose} = 1;

	}

	&print_verbose("Running Parser");

	&print_verbose("Arguments:");
	foreach my $argument (@ARGV) {
		&print_verbose("\t$argument");
		$argument =~ /=/;
		if ($` eq "lang") {
			$generator_args->{hdl_langauge} = $';
			&print_verbose("\tlang = $'");
		} elsif ($` eq "dir") {
			$generator_args->{system_directory} = $';
			&print_verbose("\tsysdir = $'");
		} elsif ($` eq "name") {
			$generator_args->{target_name} = $';
			&print_verbose("\tname = $'");
		} elsif ($` eq "files") {
			@{$generator_args->{files}} = split(/;/, $');
			&print_verbose("\tFiles = ");
			foreach my $file (@{$generator_args->{files}}) {
				&print_verbose("\t\t$file");
			}
		} elsif ($` eq "params") {
			my @params = split(/;/, $');
			foreach my $param (@params) {
				$param =~ /:/;
				$generator_args->{parameters}->{$`} = $';
				&print_verbose("\t\t$` = $'");
			}
		} elsif ($` eq "sections") {
			my @sections = split(/;/, $');
			foreach my $section (@sections) {
				$section =~ /:/;
				$generator_args->{sections}->{$`} = $';
				&print_verbose("\t\t$` = $'");
			}
		}
	}


	return 1;
}

sub print_verbose
{
	my ($info) = (@_);

	if($generator_args->{verbose} eq 1){
		open (FILE, ">>c:/dump.txt") or die "Unable to open file";
		print FILE $info."\n";
		close (FILE);
	}
}

sub get_hdl_file_list
{
	return @{$generator_args->{files}};
}

sub get_ip_core_target_name
{
	return $generator_args->{target_name};
}

sub get_parameter_value
{
	my ($param) = (@_);
	if (defined $generator_args->{parameters}->{$param}) {
		return 1, $generator_args->{parameters}->{$param};
	}
	return 0, "Parameter not found";
}

sub get_section_value
{
	my ($section) = (@_);
	if (defined $generator_args->{sections}->{$section}) {
		return 1, $generator_args->{sections}->{$section};
	}
	return 0, "Parameter not found";
}

sub get_system_directory
{
	return $generator_args->{system_directory};
}

# End with success

return 1;

# end of file

