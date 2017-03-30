#!/bin/bash

# -----------------------------------------------------------------------------
# The user of this script should alter the variables below to accomodate their
# particular build settings.
#
# EXPECTED_QUARTUS_VERSION
#   this is the first few characters of the Quartus version string that you
#   expect Quartus to report when the script checks the current version.
#
# QUARTUS_PROJECT_NAME
#   this is the name that you wish to have the Quartus project named when it is
#   created.
#
# INITIAL_PROJECT_ASSIGNMENTS
#   this is the file name of the TCL script that contains Quartus project
#   assignments that can be applied to the project immediately after it is
#   created.  This TCL script will be sourced by the build script. This script
#   should be located in the build scripts "src" directory.
#
# PIN_ASSIGNMENTS
#   this is the file name of the TCL script that contains pin assignments for
#   the Quartus project.  This TCL script will be sourced by the build script.
#   This script should be located in the build scripts "src" directory.
#
# POST_SOPC_PROJECT_ASSIGNMENTS
#   this is the file name of the TCL script that contains Quartus project
#   assignments that must be applied after the SOPC Builder system has been
#   generated.  This TCL script will be sourced by the build script.  This
#   script should be located in the build scripts "src" directory.
#
# SOPC_SYSTEM_NAME
#   this is the name that you wish to have the SOPC System named when it is
#   named.
#
# SOPC_SYSTEM_BUILD_SCRIPT
#   this is the file name of the TCL script that contains the SOPC system
#   definition.  This TCL script will be executed by SOPC Builder.  This script
#   should be located in the build scripts "src" directory.
# -----------------------------------------------------------------------------

EXPECTED_QUARTUS_VERSION="Version 10.1 Build 197"

QUARTUS_PROJECT_NAME="nios2_linux_3c120_125mhz"

INITIAL_PROJECT_ASSIGNMENTS="initial_assignments.tcl"

PIN_ASSIGNMENTS="pin_assignments.tcl"

POST_SOPC_PROJECT_ASSIGNMENTS="post_sopc_assignments.tcl"

SOPC_SYSTEM_NAME="nios2_linux_3c120_125mhz_sys_sopc"

SOPC_SYSTEM_BUILD_SCRIPT="unused_sopc_system_build_script"

# -----------------------------------------------------------------------------
# These paths are relative from the location of this file.  The default values
# should be acceptable for most uses unless you wish to alter the physical
# layout of the build scripts or Quartus project in the file system.
#
# REL_PATH_TO_PROJECT_DIR ------ relative path to Quartus project directory
#
# REL_PATH_TO_BUILD_UTILS_DIR -- relative path to build scripts utils directory
#
# REL_PATH_TO_BUILD_SRC_DIR ---- relative path to build scripts src directory
# -----------------------------------------------------------------------------

REL_PATH_TO_PROJECT_DIR=".."

REL_PATH_TO_BUILD_UTILS_DIR="utils"

REL_PATH_TO_BUILD_SRC_DIR="src"

# +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#   The script below here should not require editing.  If you wish to alter the
#   flow of the project build, add or remove functionality, then editing the
#   following lines may be required.
# +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

# -----------------------------------------------------------------------------
# load in the build utility scripts and configure the environment
# -----------------------------------------------------------------------------

. ${REL_PATH_TO_BUILD_UTILS_DIR:?environment variable is required...}/build_utils.shinc

# -----------------------------------------------------------------------------
# Check to make sure we're running under the expected version of Quartus, else
# exit the script.  Change the value of EXPECTED_QUARTUS_VERSION to change the
# version of Quartus that is accepted.
# -----------------------------------------------------------------------------

check_quartus_version               \
    "${EXPECTED_QUARTUS_VERSION}"   \
        || {
			echo ""
            echo "Error in check_quartus_version..."
			echo ""
            exit 1
        }

# -----------------------------------------------------------------------------
# Create the Quartus project.  Change the value of QUARTUS_PROJECT_NAME to
# change the name of the project that gets created.
# -----------------------------------------------------------------------------

create_new_project              \
    "${QUARTUS_PROJECT_NAME}"   \
        || {
			echo ""
            echo "Error in create_new_project..."
			echo ""
            exit 1
        }

# -----------------------------------------------------------------------------
# Make some initial project assignments, these assignments are pretty global
# settings, some of which should be set immediately and others which just need
# to be set at some point before compilation.  The INITIAL_PROJECT_ASSIGNMENTS
# variable should point to a TCL script in the build_scripts/src directory.
# -----------------------------------------------------------------------------

add_project_assignments                 \
    "${QUARTUS_PROJECT_NAME}"           \
    "${INITIAL_PROJECT_ASSIGNMENTS}"    \
        || {
			echo ""
            echo "Error in add_project_assignments during initial_project_assignments..."
			echo ""
            exit 1
        }

# -----------------------------------------------------------------------------
# Make the pin assignments, these assignments cover all aspects of pin
# assignments from location to I/O standards, etc.  The PIN_ASSIGNMENTS
# variable should point to a TCL script in the build_scripts/src directory.
# -----------------------------------------------------------------------------

add_project_assignments         \
    "${QUARTUS_PROJECT_NAME}"   \
    "${PIN_ASSIGNMENTS}"        \
        || {
			echo ""
            echo "Error in add_project_assignments during pin_assignments..."
			echo ""
            exit 1
        }

# -----------------------------------------------------------------------------
# Copy and generate the SOPC system.  Change the value of SOPC_SYSTEM_NAME to
# change the name of the system that gets created.  The a file named after this
# system name ${SOPC_SYSTEM_NAME}.sopc must reside in the build_scripts/src
# directory.
# -----------------------------------------------------------------------------

copy_and_generate_sopc_system      \
    "${SOPC_SYSTEM_NAME}"           \
        || {
			echo ""
            echo "Error in copy_and_generate_sopc_system..."
			echo ""
            exit 1
        }

# -----------------------------------------------------------------------------
# Make some additional project assignments, these assignments are primarily
# values that assign things that did not exist prior to executing SOPC Builder.
# The POST_SOPC_PROJECT_ASSIGNMENTS variable should point to a TCL script in
# the build_scripts/src directory.
# -----------------------------------------------------------------------------

add_project_assignments                 \
    "${QUARTUS_PROJECT_NAME}"           \
    "${POST_SOPC_PROJECT_ASSIGNMENTS}"  \
        || {
			echo ""
            echo "Error in add_project_assignments during post_sopc_project_assignments..."
			echo ""
            exit 1
        }

# -----------------------------------------------------------------------------
# Compile the Quartus project.
# -----------------------------------------------------------------------------

compile_project                 \
    "${QUARTUS_PROJECT_NAME}"   \
        || {
			echo ""
            echo "Error in compile_project..."
			echo ""
            exit 1
        }

# -----------------------------------------------------------------------------
# Scan the map, fit and sta reports for messages about critical warnings...
# -----------------------------------------------------------------------------

check_critical_warning "${QUARTUS_PROJECT_NAME}.map.rpt"
check_critical_warning "${QUARTUS_PROJECT_NAME}.fit.rpt"
check_critical_warning "${QUARTUS_PROJECT_NAME}.sta.rpt"

# -----------------------------------------------------------------------------
# Scan the sta report for timing not met warnings...
# -----------------------------------------------------------------------------

check_timing_not_met                    \
    "${QUARTUS_PROJECT_NAME}.sta.rpt"   \
        || {
			echo ""
            echo "Error in check_timing_not_met..."
			echo ""
            exit 1
        }

# -----------------------------------------------------------------------------
# No errors have stopped us yet so we return no error...
# -----------------------------------------------------------------------------

echo ""
echo "Project build script completed successfully..."
echo ""

exit 0
