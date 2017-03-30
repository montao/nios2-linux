#
#	This file provides the TCL procedure checkQuartusVersion which takes one
#	input argument that is the begining of the version string that is expected
#	from Quartus.  The script compares this input string against the actual
#	Quartus version string, and if it matches then nothing is done, but if it
#	does not match, then critical warning messages are printed to notify the
#	user of the discrpancy.
#
post_message -type info "Reading file: \'checkQuartusVersion.sdc\'"

proc checkQuartusVersion versionString {

	set currentQuartusVersion ${::quartus(version)}
	set testedQuartusVersion "${versionString}"

	if { [ string compare -length [ string length "${testedQuartusVersion}" ] "${currentQuartusVersion}" "${testedQuartusVersion}" ] } {

		post_message -type critical_warning "This script was tested with a different version of Quartus II."
		post_message -type critical_warning "You may wish to verify that it is still valid under this version of Quartus II."
		post_message -type critical_warning "Tested Version: ${testedQuartusVersion}"
		post_message -type critical_warning "Current Version: ${currentQuartusVersion}"

	}

}
