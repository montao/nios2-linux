#
#   This script performs little error checking on it's input arguments, it
#   assumes that it will be called with the proper number of arguments in the
#   proper order and that all arguments are valid.
#
#   Script:
#       
#       create_new_project.tcl
#   
#   Input Arguments:
#   
#       PROJECT_NAME    : the name to give the project
#
#   Output Results:
#   
#       this script checks if the project argument passed in already exists and
#       if it does, it returns an exit status of error, otherwise it creates
#       the project and returns an exit status of success.
#
#   Exit Status:
#       
#       success : 0
#       error   : != 0
#

set PROJECT_NAME [ lindex ${argv} 0 ]

if { [ project_exists ${PROJECT_NAME} ] } {

    qexit -error

} else {

    project_new ${PROJECT_NAME}
    project_close
    qexit -success
    
}
