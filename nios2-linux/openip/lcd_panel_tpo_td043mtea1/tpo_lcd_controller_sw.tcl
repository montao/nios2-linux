#
# tpo_lcd_controller_sw.tcl
#

# Create a new driver
create_driver tpo_lcd_controller_driver

# Associate it with some hardware known as "tpo_lcd_controller"
set_sw_property hw_class_name tpo_lcd_controller

# The version of this driver is "7.2"
set_sw_property version 7.2

# This driver may be incompatible with versions of hardware less
# than specified below. Updates to hardware and device drivers
# rendering the driver incompatible with older versions of
# hardware are noted with this property assignment.
#
# Multiple-Version compatibility was introduced in version 7.1;
# prior versions are therefore excluded.
set_sw_property min_compatible_hw_version 7.2

# Initialize the driver in alt_sys_init()
set_sw_property auto_initialize true

# Location in generated BSP that above sources will be copied into
set_sw_property bsp_subdirectory drivers

# This driver supports the HAL & uC/OS-II (OS) types
add_sw_property supported_bsp_type HAL
add_sw_property supported_bsp_type UCOSII

#
# Source file listings...
#

# C/C++ source files
add_sw_property c_source HAL/src/tpo_lcd_controller.c

# Include files
add_sw_property include_source HAL/inc/tpo_lcd_controller.h

# End of file
