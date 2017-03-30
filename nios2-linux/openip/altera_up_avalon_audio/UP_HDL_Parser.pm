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
# | file: UP_HDL_Parser.pl                                                     |
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

package UP_HDL_Parser;
use Exporter;


@ISA = Exporter;
@EXPORT = qw(
	parse_hdl_file
);

use strict;
use UP_Argument_Parser;

sub parse_hdl_file
{
	my ($filename, $is_top_level_module) = (@_);

	my $error;
	my $module_name;
	my @hdl_file;

	$module_name = $filename;
	$module_name =~ s/\.v//;

	($error, @hdl_file) = load_hdl_file($filename);
	return $error if ($error != 1);

	if ($is_top_level_module == 0) {
		($error, @hdl_file) = modify_the_parameters(@hdl_file);
		return $error if ($error != 1);

		($error, @hdl_file) = parse_preprocessor_directives($module_name, @hdl_file);
		return $error if ($error != 1);

		$module_name = get_ip_core_target_name();
		($error, @hdl_file) = change_module_name($module_name, @hdl_file);
		return $error if ($error != 1);
	}

	return 1, $module_name, @hdl_file;
}

sub load_hdl_file
{
	my ($info) = (@_);

	my $filename = "hdl/".$info;

	print_verbose ("Loading Module File: ".$info.", with fullpath name: ".$filename);

	my $text;

	my $line_count;
	my @hdl_file;

	open (FILE, $filename) or return "Unable to find the module: ".$info;
	while(<FILE>)
	{
		$line_count = push @hdl_file, $_;
	}
	close (FILE);

	print_verbose ("Module File ".$info." was loaded and contains ".$line_count." lines of code.");

	return (1, @hdl_file);
}

sub modify_the_parameters
{
	my (@hdl_file) = (@_);
	my @new_hdl_file;
	my $error;

	my $line_count = $#hdl_file + 1;

	for (my $i = 0; $i < $line_count; $i++)
	{
		my ($line) = $hdl_file[$i];
		if ($line =~ /^\s*parameter\s*/)
		{	 
			my $the_start = $&;
			my $the_rest = $';

			my $comment = ";\n";
			if ($the_rest =~ /;\s*\/\//)
			{
				$comment = $&.$';
				$the_rest = $`;
			}

			$the_rest =~ /\s*=\s*/;
			my $the_param		= $`;
			my $the_equal_sign	= $&;

			my $new_value;
			($error, $new_value) = get_parameter_value($the_param);
			if ($error != 1)
			{
				push @new_hdl_file, $line;
			}
			else
			{
#				$new_value =~ s/h/'h/;
#				$new_value =~ s/b/'b/;
#				$new_value =~ s/d/'d/;
				push @new_hdl_file, ($the_start.$the_param.$the_equal_sign.$new_value.$comment);
			}
		}
		else
		{
			push @new_hdl_file, $line;
		}
	}

	return 1, @new_hdl_file;
}

# Does not handle nested IF directives
sub parse_preprocessor_directives
{
	my ($moduleName, @hdl_file) = (@_);

	print_verbose ("Parsing preprocessor directives for the module: ".$moduleName);

	my $line;
	my $line_number = 0;
	my @new_hdl_file;

	my $in_directive = 0;
	my $valid_directive = 0;
	my $allow_valid_directive = 1;
	my $has_valid_directive = 0;

	my $nested_ifs_count = 0;
	my @nested_ifs_data;

	foreach $line (@hdl_file)
	{
		if (($line =~ /^`.+/) && (!($line =~ /^`timescale.+/)))
		{
			if ($line =~ /^`IF\s+/)
			{
				chop (my $section_name = $');
				if ($in_directive == 1)
				{
					my $nest_ifs_info = {
						in_directive			=> $in_directive,
						valid_directive			=> $valid_directive,
						allow_valid_directive	=> $allow_valid_directive,
						has_valid_directive		=> $has_valid_directive,
					};
					$nested_ifs_count++;
					push @nested_ifs_data, $nest_ifs_info;
					$allow_valid_directive	= $valid_directive;
					$valid_directive		= 0;
					$has_valid_directive	= 0;
					# return "Preprocessor directive parse error: found nested `IF directives at line number: ".$line_number;
				}
#				else
#				{
					$in_directive = 1;
					if ($section_name eq "1")
					{
#						$valid_directive = 1;
						$valid_directive = $allow_valid_directive;
					}
					elsif ($section_name eq "0")
					{
						$valid_directive = 0;
					}
					else
					{
						my ($error, $value) = get_section_value($section_name);
						if (($error == 1) && ($value == 1))
						{
#							$valid_directive = 1;
							$valid_directive = $allow_valid_directive;
						}
						else
						{
							$valid_directive = 0;
						}
					}
					$has_valid_directive = $valid_directive;
#				}
			}
			elsif ($line =~ /^`ELSIF\s+/)
			{
				chop (my $section_name = $');
				if ($in_directive == 0)
				{
					return "Preprocessor directive parse error: found `ELSIF before `IF at line number: ".$line_number;
				}
				elsif ($has_valid_directive == 1)
				{
					$valid_directive = 0;
				}
				else
				{
					if ($section_name eq "1")
					{
#						$valid_directive = 1;
						$valid_directive = $allow_valid_directive;
					}
					elsif ($section_name eq "0")
					{
						$valid_directive = 0;
					}
					else
					{
						my ($error, $value) = get_section_value($section_name);
						if (($error == 1) && ($value == 1))
						{
#							$valid_directive = 1;
							$valid_directive = $allow_valid_directive;
						}
						else
						{
							$valid_directive = 0;
						}
					}
					$has_valid_directive = $valid_directive;
				}
			}
			elsif ($line =~ /^`ELSE/)
			{
				if ($in_directive == 0)
				{
					return "Preprocessor directive parse error: found `ELSE before `IF at line number: ".$line_number;
				}
				elsif ($has_valid_directive == 1)
				{
					$valid_directive = 0;
				}
				else
				{
#					$valid_directive = 1;
					$valid_directive = $allow_valid_directive;
				}
				$has_valid_directive = 1;
			}
			elsif ($line =~ /^`ENDIF/)
			{
				if ($in_directive == 0)
				{
					return "Preprocessor directive parse error: found `ENDIF before `IF at line number: ".$line_number;
				}
				elsif ($nested_ifs_count > 0)
				{
					my $nest_ifs_info = {
						in_directive			=> 0,
						valid_directive			=> 0,
						allow_valid_directive	=> 1,
						has_valid_directive		=> 0,
					};
					$nest_ifs_info = pop @nested_ifs_data;
					$nested_ifs_count--;

					$in_directive			= $nest_ifs_info->{in_directive};
					$valid_directive		= $nest_ifs_info->{valid_directive};
					$allow_valid_directive	= $nest_ifs_info->{allow_valid_directive};
					$has_valid_directive	= $nest_ifs_info->{has_valid_directive};
				}
				else
				{
					$in_directive			= 0;
					$valid_directive		= 0;
					$allow_valid_directive	= 1;
					$has_valid_directive	= 0;
				}
			}
			else
			{
				return "Preprocessor directive parse error: found unknown directive ".$&." at line number: ".$line_number;
			}
		}
		elsif (($in_directive == 0) || ($valid_directive == 1))
		{
			push @new_hdl_file, $line;
		}

		$line_number++;
	}

	return (1, @new_hdl_file);
}

sub change_module_name
{
	my $new_module_name;
	my @hdl_file;
	($new_module_name, @hdl_file) = (@_);

	my $line;
	my @new_hdl_file;
	foreach $line (@hdl_file)
	{
		if ($line =~ /^module/)
		{
			push @new_hdl_file, "module ".$new_module_name." (\n";		
		}
		else
		{
			push @new_hdl_file, $line;
		}
	}

	return 1, @new_hdl_file;
}

# End with success

return 1;

# end of file

