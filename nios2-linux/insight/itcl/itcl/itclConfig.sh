# itclConfig.sh --
# 
# This shell script (for sh) is generated automatically by Itcl's
# configure script.  It will create shell variables for most of
# the configuration options discovered by the configure script.
# This script is intended to be included by the configure scripts
# for Itcl extensions so that they don't have to figure this all
# out for themselves.  This file does not duplicate information
# already provided by tclConfig.sh, so you may need to use that
# file in addition to this one.
#
# The information in this file is specific to a single platform.

# Itcl's version number.
ITCL_VERSION='3.2'
ITCL_MAJOR_VERSION='3'
ITCL_MINOR_VERSION='2'
ITCL_RELEASE_LEVEL='.1'

# The name of the Itcl library (may be either a .a file or a shared library):
ITCL_LIB_FILE=libitcl3.2.a

# String to pass to linker to pick up the Itcl library from its
# build directory.
ITCL_BUILD_LIB_SPEC='-L/home/dwesterg/data/nios2-linux/nios2-linux/insight/itcl/itcl -litcl3.2${TCL_DBGX}'

# String to pass to linker to pick up the Itcl library from its
# installed directory.
ITCL_LIB_SPEC='-L/usr/local/lib -litcl3.2${TCL_DBGX}'

# The name of the Itcl stub library (a .a file):
ITCL_STUB_LIB_FILE=libitclstub3.2.a

# String to pass to linker to pick up the Itcl stub library from its
# build directory.
ITCL_BUILD_STUB_LIB_SPEC='-L/home/dwesterg/data/nios2-linux/nios2-linux/insight/itcl/itcl -litclstub3.2${TCL_DBGX}'

# String to pass to linker to pick up the Itcl stub library from its
# installed directory.
ITCL_STUB_LIB_SPEC='-L/usr/local/lib -litclstub3.2${TCL_DBGX}'

# Location of the top-level source directories from which [incr Tcl]
# was built.  This is the directory that contains generic, unix, etc.
# If [incr Tcl] was compiled in a different place than the directory
# containing the source files, this points to the location of the sources,
# not the location where [incr Tcl] was compiled.
ITCL_SRC_DIR='/home/dwesterg/data/nios2-linux/nios2-linux/insight/itcl/itcl'
