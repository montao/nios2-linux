# +-----------------------------------
# | 
# | graphics_lcd "graphics_lcd" v1.0
# |  2008.11.27.00:36:34
# | 
# | 
# +-----------------------------------


# +-----------------------------------
# | module graphics_lcd
# | 
set_module_property DESCRIPTION ""
set_module_property NAME graphics_lcd
set_module_property VERSION 1.0
set_module_property GROUP ""
set_module_property AUTHOR ""
set_module_property DISPLAY_NAME graphics_lcd
set_module_property INSTANTIATE_IN_SYSTEM_MODULE false
set_module_property EDITABLE false
# | 
# +-----------------------------------

# +-----------------------------------
# | files
# | 
# | 
# +-----------------------------------

# +-----------------------------------
# | parameters
# | 
# | 
# +-----------------------------------

# +-----------------------------------
# | connection point clock
# | 
add_interface clock clock end
set_interface_property clock ptfSchematicName ""

add_interface_port clock clk clk Input 1
# | 
# +-----------------------------------

# +-----------------------------------
# | connection point s0
# | 
add_interface s0 avalon_tristate end
set_interface_property s0 activeCSThroughReadLatency false
set_interface_property s0 addressSpan 2
set_interface_property s0 bridgesToMaster ""
set_interface_property s0 holdTime 40
set_interface_property s0 isMemoryDevice false
set_interface_property s0 isNonVolatileStorage false
set_interface_property s0 maximumPendingReadTransactions 0
set_interface_property s0 minimumUninterruptedRunLength 1
set_interface_property s0 printableDevice false
set_interface_property s0 readLatency 0
set_interface_property s0 readWaitStates 500
set_interface_property s0 readWaitTime 500
set_interface_property s0 setupTime 500
set_interface_property s0 timingUnits Nanoseconds
set_interface_property s0 writeWaitStates 500
set_interface_property s0 writeWaitTime 500

set_interface_property s0 ASSOCIATED_CLOCK clock

add_interface_port s0 ats_s0_address address Input 1
add_interface_port s0 ats_s0_read_n read_n Input 1
add_interface_port s0 ats_s0_data data Bidir 8
add_interface_port s0 ats_s0_write_n write_n Input 1
add_interface_port s0 ats_s0_chipselect_n chipselect_n Input 1
add_interface_port s0 ats_s0_byteenable byteenable Input 1
# | 
# +-----------------------------------
