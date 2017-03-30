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
# | file: UP_HDL_Writer.pm                                                     |
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

package UP_HDL_Writer;
use Exporter;


@ISA = Exporter;
@EXPORT = qw(
	write_hdl_file
);


use strict;
use UP_Argument_Parser;

sub write_hdl_file
{
	my ($module_name, @hdl_file) = (@_);

	my $line;
	my $filename = get_system_directory().$module_name.".v";

	print_verbose ("Writing Module File: ".$module_name.", with fullpath name: ".$filename);

	open (FILE, ">$filename") or return "Unable to open the file: ".$filename." to create the module: ".$module_name;
	foreach $line (@hdl_file)
	{
		print FILE $line;
	}
	close (FILE);


	return 1;
}


# End with success

return 1;

# end of file

