#
#   This script performs little error checking on it's input arguments, it
#   assumes that it will be called with the proper number of arguments in the
#   proper order and that all arguments are valid.
#
#   Script:
#       
#       add_project_assignments.tcl
#   
#   Input Arguments:
#   
#       PROJECT_NAME        : the name of the project
#       ASSIGNMENTS_FILE    : the name the assignments file
#
#   Output Results:
#   
#       this script checks if the project argument passed in already exists and
#       if it does, it opens the project, sources the assignments file and
#       returns and exit status of success, otherwise returns an exit status
#       of error.
#
#   Exit Status:
#       
#       success : 0
#       error   : != 0
#

set PROJECT_NAME        [ lindex ${argv} 0 ]
set ASSIGNMENTS_FILE    [ lindex ${argv} 1 ]

if { [ project_exists ${PROJECT_NAME} ] } {

    project_open -current_revision ${PROJECT_NAME}
    source ${ASSIGNMENTS_FILE}
    project_close
    qexit -success

} else {

    qexit -error
    
}
