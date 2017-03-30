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
# | file: UP_IP_Core_Generator.pl                                              |
# |                                                                            |
# | This SOPC Builder Generator program is provided by                         |
# | Altera's University Program.                                               |
# |                                                                            |
# | Its purpose is to construct the HDL files for a particular instance of a   |
# | particular University Program SOPC Builder peripheral.                     |
# |                                                                            |
# | version: 1.0                                                               |
# |                                                                            |
# +----------------------------------------------------------------------------+

use strict;
use UP_Argument_Parser;
use UP_HDL_Parser;
use UP_HDL_Writer;
#use UP_System_Info;
#use UP_PTF_Parser;
#use UP_HDL_Parser;
#use UP_HDL_Writer;
#use UP_Extras;

sub main
{
	my $error;
	my $module_name;
	my @hdl_file;

#	print "Running the A/V Config Generator!!\n";

	($error) = parse_arguments();
	&ribbit ($error) if ($error != 1);

#	($error) = initialize_ptf_parser();
#	&ribbit ($error) if ($error != 1);

	my @hdl_files = get_hdl_file_list();

	for my $i (0 .. $#hdl_files)
	{
		print_verbose("$i is ".@hdl_files[$i]);
		($error, $module_name, @hdl_file) = parse_hdl_file(@hdl_files[$i], $i);
		&ribbit ($error) if ($error != 1);
		($error) = write_hdl_file($module_name, @hdl_file);
		&ribbit ($error) if ($error != 1);
	}
	
#	($error) = copy_additional_files();
#	&ribbit ($error) if ($error != 1);

    exit (0);
}

$| = 1;  # always polite to flush.
main()

# end of file
