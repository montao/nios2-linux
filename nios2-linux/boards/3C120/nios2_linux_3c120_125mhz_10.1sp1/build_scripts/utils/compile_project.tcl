#
#   This script performs little error checking on it's input arguments, it
#   assumes that it will be called with the proper number of arguments in the
#   proper order and that all arguments are valid.
#
#   Script:
#       
#       compile_project.tcl
#   
#   Input Arguments:
#   
#       PROJECT_NAME    : the name of the project to compile
#
#   Output Results:
#   
#       this script checks if the project argument passed in already exists and
#       if it does, it calls the quartus compile flow and returns success or
#       error based on the compile flow results, otherwise it returns an exit
#       status of error.
#
#   Exit Status:
#       
#       success : 0
#       error   : != 0
#

load_package report
load_package flow

set PROJECT_NAME [ lindex ${argv} 0 ]

if { [ project_exists ${PROJECT_NAME} ] } {

    project_open ${PROJECT_NAME}

    if { [ catch { execute_flow -compile } result ] } {
        
        post_message -type error ""
        post_message -type error "Project compilation failed..."
        post_message -type error ""

        project_close
        qexit -error
        
    }

    project_close
    qexit -success

} else {

    post_message -type error ""
    post_message -type error "The project ${PROJECT_NAME} does not appear to exist..."
    post_message -type error ""
    
    qexit -error
    
}
