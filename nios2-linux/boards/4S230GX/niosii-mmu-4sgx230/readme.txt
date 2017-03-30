readme - Applications Processor MMU Design

Overview:
=========

This design is provided to showing how Nios II can be configured as an applications processor:
a processor with a memory management unit so it can run a host class operating system .


Contents of the System:
=======================

 - Nios II/f Core with MMU
 - JTAG Debug Module (Level 1)
 - Tightly-coupled memories (On Chip Memory)
 - DDR SDRAM Controller (128MB)
 - CFI Flash Memory Interface (64MB)
 - Descriptor memories (On Chip Memory)
 - JTAG UART
 - UART (RS-232)
 - One Timers
 - Ethernet Interface
 - LED PIO
 - Push Button PIO
 - System ID Peripheral
 - PLLs 

 
Further Notes:
==============

- The top level of this design is the HDL file generated around the SOPC Builder design. This wrapper performs the following functions:

  1.) Renaming the DDR-associated pins from SOPC Builder to match the timing assignments produced by the DDR megafunction.
  2.) If you modify and regenerate the SOPC Builder design, the port list of the SOPC Builder instance may change. 
      You must manually edit the HDL wrapper file to rectify any discrepancies.

- This Quartus II project contains assignments that match the port names produced by SOPC Builder. 
  If you add or modify SOPC Builder components, the pin assignments may no longer be valid. 
  To view the Assignment Editor in the Quartus II software, in the Assignments menu, click "Assignment Editor".

- This design contains the DDR memory and Triple Speed Ethernet components.  Any design containing these cores must be re-generated 
  in SOPC Builder before re-compiling it in Quartus if the installation path to the Altera toolchain has changed since it was 
  last generated.  This is because these cores make use of RTL libraries that are referenced using absolute paths.  The re-generation 
  process will update these absolute paths.
  Attempting to recompile in Quartus II without regenerating will result in an error of the following form during Quartus II Analysis and 
  Synthesis:
  Error: Node instance "ddr_control" instantiates undefined entity "auk_ddr_controller"

- The current version of the Nios II EDS hardware design example uses an HDL 
  file as the top level of the design hierarchy.  If you would like to use a 
  schematic-based top level instead (BDF), follow the steps listed below.  
    1) In the Quartus II software, open the top-level HDL file (.v or .vhd) for
       the design.
    2) Create a symbol for the HDL file by clicking 
       File -> Create/Update -> Create Symbol Files for Current File
    3) Create a new BDF file by clicking 
       File -> New -> Block Diagram/Schematic File.
    4) Instantiate the symbol in the BDF by double-clicking in the empty space
       of the BDF file and selecting "Project -> <symbol filename>"
    5) Instantiate pins in the BDF by double-clicking empty space, then 
       typing "input", "output", or "bidir".
    6) Rename the pins and connect them to the appropriate ports on the symbol.
    7) Save the BDF as a unique filename.
    8) Set the BDF as your top level entity by clicking:
       Project -> Set as Top-Level Entity
    9) Recompile the Quartus II project.

- This example design should be used with any of the Altera's Partner Development Board Daughter Cards
  (http://www.altera.com/products/devkits/kit-daughter_boards.jsp) listed below:
  
  1) 10/100 Ethernet PHY Daughter Card with National Semiconductor PHY (http://www.morethanip.com/boards_10_100_dp83848.htm)
  2) 10/100/1000 Ethernet PHY Daughter Board with Marvell PHY (http://www.morethanip.com/boards_10_100_1000_88E1111.htm)
  3) 10/100/1000 Ethernet PHY Daughter Card with National Semiconductor PHY (http://www.morethanip.com/boards_10_100_1000_dp83865.htm)

- For more information, please refer to http://www.altera.com/support/examples/nios2/exm-nios2.html

