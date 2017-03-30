#
#   This script performs little error checking on it's input arguments, it
#   assumes that it will be called with the proper number of arguments in the
#   proper order and that all arguments are valid.
#
#   Script:
#       
#       check_quartus_version.tcl
#   
#   Input Arguments:
#   
#       VERSION_STRING : the begining of the expected Quartus II version string
#
#   Output Results:
#   
#       script returns an exit status of success if the VERSION_STRING argument
#       matches the begining of the current quartus version string, otherwise
#       it returns an exit status of error
#
#   Exit Status:
#       
#       success : 0
#       error   : != 0
#

if { ${argc} != 1 } {

    post_message -type error "This script expects only one argument..."
    qexit -error

}

set VERSION_STRING [ lindex ${argv} 0 ]

set current_version_string ${::quartus(version)}

set expected_string_length [ string length ${VERSION_STRING} ]

if { ${expected_string_length} < 15 } {

    post_message -type error "The version string passed into this script seems too short..."
    qexit -error

}

post_message -type info ""
post_message -type info "Expected version: ${VERSION_STRING}"
post_message -type info " Current version: ${current_version_string}"
post_message -type info ""

if {[ string compare -length ${expected_string_length} ${VERSION_STRING} ${current_version_string} ]} {

    qexit -error
    
} else {
    
    qexit -success

}
