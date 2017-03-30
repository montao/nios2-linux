----------------------------------
-- Microtronix SDRAM controller --
----------------------------------
--        Package Library       --  
----------------------------------
--  Written by Marco Groeneveld --
----------------------------------
-- v1.0 : Initial release
----------------------------------
-- Build : 169/9.7.2010
----------------------------------
LIBRARY ieee;
USE ieee.std_logic_1164.all;

PACKAGE mtx_sdram_package IS

-----
-- Components
-----

-- SDRAM Controller
COMPONENT mtx_sdram_controller IS
	GENERIC
	(

	-- Local SDRAM Port
		lsp_addr_width               : NATURAL := 23;
		lsp_data_width               : NATURAL := 32;
		lsp_data_mask_width          : NATURAL := 4;
		lsp_burst_width              : NATURAL := 14;
		lsp_total_ports              : NATURAL := 2;

	-- SDRAM Settings
		sdram_architecture           : INTEGER := 1;		-- 0 - SDR / 1 - DDR / 2 - Mobile DDR / 3 - DDR2
		sdram_scheduler_parameter    : INTEGER := 0;
		sdram_data_strobe_width      : INTEGER := 2;
		sdram_data_group_width       : INTEGER := 8;
		sdram_dqs_groups             : INTEGER := 1;
		sdram_max_burst_length       : INTEGER := 8192;
		sdram_cas_latency            : INTEGER := 25;
		sdram_dqs_delay              : STRING  := "9";
		sdram_dqs_filter             : INTEGER := 6;
		sdram_devices                : INTEGER := 1;
		sdram_dqs_per_device         : INTEGER := 2;
		sdram_differential_dqs       : INTEGER := 0;
		sdram_clock_pairs            : INTEGER := 1;
		sdram_reduced_drive_strength : INTEGER := 0;
		sdram_command_clock          : INTEGER := 0;
		sdram_odt_value              : INTEGER := 0;
		sdram_disable_ddr_blocks     : INTEGER := 0;

		sdram_linear_addr_width      : INTEGER := 24;
		sdram_addr_width             : INTEGER := 13;
		sdram_data_width             : INTEGER := 16;
		sdram_data_mask_width        : INTEGER := 2;
		sdram_bank_addr_width        : INTEGER := 2;
		sdram_bank_addr_high         : INTEGER := 23;
		sdram_bank_addr_low          : INTEGER := 22;
		sdram_row_addr_width         : INTEGER := 13;
		sdram_row_addr_high          : INTEGER := 21;
		sdram_row_addr_low           : INTEGER := 9;
		sdram_column_addr_width      : INTEGER := 9;
		sdram_column_addr_high       : INTEGER := 8;
		sdram_column_addr_low        : INTEGER := 0;

		sdram_oe_latency             : NATURAL := 1;
		sdram_cmd_latency            : NATURAL := 1; -- 2 - SDR / 1 - DDR 
		sdram_read_latency           : NATURAL := 7;
		sdram_write_data_delay       : NATURAL := 1;

		device_family                : STRING := "CYCLONE II";

	-- Stratix II DLL
		stx2_dll_frequency           : STRING := "5000ps";
		stx2_dll_chain_length        : INTEGER := 16;
		stx2_dll_buffer_mode         : STRING := "high";
		stx2_dll_out_mode            : STRING := "delay_chain3";

	-- SDRAM Timing
		tINIT                        : NATURAL := 27000;
		tRP                          : NATURAL := 3;
		tMRD                         : NATURAL := 2;
		tRFC                         : NATURAL := 12;
		tREFI                        : NATURAL := 2000;
		tRCD                         : NATURAL := 3;
		tWR                          : NATURAL := 3;
		tRAS                         : NATURAL := 7;
		tRC                          : NATURAL := 10
	);

	PORT
	(
		RST                   : IN std_logic;
		SDRAM_CLK             : IN std_logic;
		SDRAM_CMD_CLK         : IN std_logic;

	-- Local SDRAM Port  
		LSP_ADDR              : IN std_logic_vector((lsp_total_ports * lsp_addr_width) - 1 DOWNTO 0);
		LSP_DATA_IN           : IN std_logic_vector((lsp_total_ports * lsp_data_width) - 1 DOWNTO 0);
		LSP_DATA_OUT          : OUT std_logic_vector(lsp_data_width - 1 DOWNTO 0);
		LSP_DATA_MASK         : IN std_logic_vector((lsp_total_ports * lsp_data_mask_width) - 1 DOWNTO 0);
		LSP_READ              : OUT std_logic_vector(lsp_total_ports - 1 DOWNTO 0);
		LSP_WRITE             : OUT std_logic_vector((lsp_total_ports * sdram_devices) - 1 DOWNTO 0);
		LSP_DIR               : IN std_logic_vector(lsp_total_ports - 1 DOWNTO 0);
		LSP_LENGTH            : IN std_logic_vector((lsp_total_ports * lsp_burst_width) - 1 DOWNTO 0);
		LSP_REQUEST           : IN std_logic_vector(lsp_total_ports - 1 DOWNTO 0);
		LSP_GRANDED           : OUT std_logic_vector(lsp_total_ports - 1 DOWNTO 0);
		LSP_PREAMBLE          : OUT std_logic_vector(lsp_total_ports - 1 DOWNTO 0);
		LSP_END               : OUT std_logic_vector(lsp_total_ports - 1 DOWNTO 0);
		LSP_DATA_OUT_CLK      : OUT std_logic_vector(sdram_devices - 1 DOWNTO 0);
		LSP_READY             : OUT std_logic;

	-- Control Interface
		CONTROL_DPD           : IN std_logic;
		CONTROL_POWER_STATE   : OUT std_logic;

	-- SDRAM Interface
		SDRAM_CKE             : OUT std_logic;
		SDRAM_CS              : OUT std_logic;
		SDRAM_WE              : OUT std_logic;
		SDRAM_CAS             : OUT std_logic;
		SDRAM_RAS             : OUT std_logic;
		SDRAM_BA              : OUT std_logic_vector(sdram_bank_addr_width - 1 DOWNTO 0);
		SDRAM_A               : OUT std_logic_vector(sdram_addr_width - 1 DOWNTO 0);
		SDRAM_DQM             : OUT std_logic_vector(sdram_data_mask_width - 1 DOWNTO 0);
		SDRAM_DQ              : INOUT std_logic_vector(sdram_data_width - 1 DOWNTO 0);

	-- SDR
		SDRAM_CAPTURE_CLK     : IN std_logic;

	-- DDR
		SDRAM_WRITE_CLK       : IN std_logic;
		SDRAM_CLK_OUT_P       : OUT std_logic_vector(sdram_clock_pairs - 1 DOWNTO 0);
		SDRAM_CLK_OUT_N       : OUT std_logic_vector(sdram_clock_pairs - 1 DOWNTO 0);
		SDRAM_DQS_P           : INOUT std_logic_vector(sdram_data_strobe_width - 1 DOWNTO 0);
		SDRAM_DQS_N           : INOUT std_logic_vector(sdram_data_strobe_width - 1 DOWNTO 0);

	-- DDR2
		SDRAM_ODT             : OUT std_logic
	);
END COMPONENT mtx_sdram_controller;

-- Port Scheduler
COMPONENT mtx_sdram_controller_scheduler IS
	GENERIC
	(
		TOTAL_PORTS      : NATURAL := 2;
		CUSTOM_PARAMETER : INTEGER := 0
	);

	PORT
	(
		RST          : IN std_logic;
		CLK          : IN std_logic;
		PORT_REQUEST : IN std_logic_vector(TOTAL_PORTS - 1 DOWNTO 0);
		PORT_END     : IN std_logic_vector(TOTAL_PORTS - 1 DOWNTO 0);
		PORT_SELECT  : OUT std_logic_vector(TOTAL_PORTS - 1 DOWNTO 0)
	);
END COMPONENT mtx_sdram_controller_scheduler;

-- Avalon Random
COMPONENT mtx_sdram_avalon_random IS
	GENERIC
	(
	-- Avalon
		avalon_addr_width      : INTEGER := 23;
		avalon_data_width      : INTEGER := 32;
		avalon_data_mask_width : INTEGER := 4;

	-- Local SDRAM Port
		lsp_addr_width         : INTEGER := 23;
		lsp_data_width         : INTEGER := 32;
		lsp_data_mask_width    : INTEGER := 4;
		lsp_burst_width        : INTEGER := 11;
		lsp_write_clocks       : INTEGER := 1;
		lsp_write_data_width   : INTEGER := 32;

	-- Cache
		DEVICE_FAMILY          : STRING := "CYCLONE II";
		CACHE_TOTAL_WORDS      : INTEGER := 16;
		CACHE_ADDR_WIDTH       : INTEGER := 4;
		CACHE_DISABLE          : INTEGER := 0
	);

	PORT
	(
	-- Avalon Slave Interface
		AS_RST          : IN std_logic;
		AS_CLK          : IN std_logic;
		AS_CS           : IN std_logic;
		AS_RD           : IN std_logic;
		AS_WR           : IN std_logic;
		AS_ADDR         : IN std_logic_vector(avalon_addr_width - 1 DOWNTO 0);
		AS_BE           : IN std_logic_vector(avalon_data_mask_width - 1 DOWNTO 0);
		AS_DATA_IN      : IN std_logic_vector(avalon_data_width - 1 DOWNTO 0);
		AS_DATA_OUT     : OUT std_logic_vector(avalon_data_width - 1 DOWNTO 0);
		AS_WAITREQ      : OUT std_logic;

	-- Local SDRAM Port
		LSP_CLK         : IN std_logic;
		LSP_READY       : IN std_logic;
		LSP_ADDR        : OUT std_logic_vector(lsp_addr_width - 1 DOWNTO 0);
		LSP_DIR         : OUT std_logic;
		LSP_READ        : IN std_logic;
		LSP_WRITE       : IN std_logic_vector(lsp_write_clocks - 1 DOWNTO 0);
		LSP_LENGTH      : OUT std_logic_vector(lsp_burst_width - 1 DOWNTO 0);
		LSP_REQUEST     : OUT std_logic;
		LSP_GRANDED     : IN std_logic;
		LSP_PREAMBLE    : IN std_logic;
		LSP_END         : IN std_logic;
		LSP_DATA_IN     : IN std_logic_vector(lsp_data_width - 1 DOWNTO 0);
		LSP_DATA_OUT    : OUT std_logic_vector(lsp_data_width - 1 DOWNTO 0);
		LSP_DATA_MASK   : OUT std_logic_vector(lsp_data_mask_width - 1 DOWNTO 0);
		LSP_DATA_IN_CLK : IN std_logic_vector(lsp_write_clocks - 1 DOWNTO 0)
	);
END COMPONENT mtx_sdram_avalon_random;

-- Avalon Burst
COMPONENT mtx_sdram_avalon_burst IS
	GENERIC
	(
	-- Avalon
		avalon_addr_width      : INTEGER := 23;
		avalon_data_width      : INTEGER := 32;
		avalon_data_mask_width : INTEGER := 4;
		avalon_burst_width     : INTEGER := 4;

	-- Local SDRAM Port
		lsp_addr_width         : INTEGER := 23;
		lsp_data_width         : INTEGER := 32;
		lsp_data_mask_width    : INTEGER := 4;
		lsp_burst_width        : INTEGER := 11;
		lsp_write_clocks       : INTEGER := 1;
		lsp_write_data_width   : INTEGER := 32;

	-- Cache
		DEVICE_FAMILY          : STRING := "CYCLONE II";
		AV_FIFO_TOTAL_WORDS    : INTEGER := 16;
		AV_FIFO_ADDR_WIDTH     : INTEGER := 4;
		LSP_FIFO_TOTAL_WORDS   : INTEGER := 16;
		LSP_FIFO_ADDR_WIDTH    : INTEGER := 4;
		DATA_WIDTH_RATIO       : INTEGER := 1
	);

	PORT
	(

	-- Avalon Slave Interface
		AS_RST          : IN std_logic;
		AS_CLK          : IN std_logic;
		AS_CS           : IN std_logic;
		AS_RD           : IN std_logic;
		AS_WR           : IN std_logic;
		AS_BE           : IN std_logic_vector(avalon_data_mask_width - 1 DOWNTO 0);
		AS_ADDR         : IN std_logic_vector(avalon_addr_width - 1 DOWNTO 0);
		AS_BURSTCOUNT   : IN std_logic_vector(avalon_burst_width - 1 DOWNTO 0);
		AS_DATA_IN      : IN std_logic_vector(avalon_data_width - 1 DOWNTO 0);
		AS_DATA_OUT     : OUT std_logic_vector(avalon_data_width - 1 DOWNTO 0);
		AS_WAITREQ      : OUT std_logic;
		AS_DATAVALID    : OUT std_logic;
		AS_DATAENABLE   : IN std_logic;

	-- Local SDRAM Port
		LSP_CLK         : IN std_logic;
		LSP_READY       : IN std_logic;
		LSP_ADDR        : OUT std_logic_vector(lsp_addr_width - 1 DOWNTO 0);
		LSP_DATA_IN     : IN std_logic_vector(lsp_data_width - 1 DOWNTO 0);
		LSP_DATA_OUT    : OUT std_logic_vector(lsp_data_width - 1 DOWNTO 0);
		LSP_DATA_MASK   : OUT std_logic_vector(lsp_data_mask_width - 1 DOWNTO 0);
		LSP_DIR         : OUT std_logic;
		LSP_READ        : IN std_logic;
		LSP_WRITE       : IN std_logic_vector(lsp_write_clocks - 1 DOWNTO 0);
		LSP_LENGTH      : OUT std_logic_vector(lsp_burst_width - 1 DOWNTO 0);
		LSP_REQUEST     : OUT std_logic;
		LSP_GRANDED     : IN std_logic;
		LSP_PREAMBLE    : IN std_logic;
		LSP_END         : IN std_logic;
		LSP_DATA_IN_CLK : IN std_logic_vector(lsp_write_clocks - 1 DOWNTO 0)
	);
END COMPONENT mtx_sdram_avalon_burst;

-- SDR Bidirectional IO
COMPONENT mtx_sdram_sdr_bidir
	PORT
	(
		RST      : IN std_logic;
		CLK_IN   : IN std_logic;
		CLK_OUT  : IN std_logic;
		DATA_IN  : IN std_logic;
		DATA_OUT : OUT std_logic;
		OE       : IN std_logic;
		PADIO    : INOUT std_logic
	);
END COMPONENT mtx_sdram_sdr_bidir;

-- DDR Output
COMPONENT mtx_sdram_ddr_output IS
	GENERIC
	(
		WIDTH         : NATURAL := 1;
		DEVICE_FAMILY : STRING := "Cyclone II"
	);

	PORT
	(
		RST       : IN std_logic;
		CLK_IN    : IN std_logic;
		ENA       : IN std_logic;
		DATA_IN_L : IN std_logic;
		DATA_IN_H : IN std_logic;
		DATA_OUT  : OUT std_logic
	);
END COMPONENT mtx_sdram_ddr_output;

-- DDR Bidirectional IO
COMPONENT mtx_sdram_ddr_bidir IS
	GENERIC
	(
		MODE                 : NATURAL := 0;
		WIDTH                : NATURAL := 8;
		DEVICE_FAMILY        : STRING := "Cyclone II";
		DQS_DELAY            : STRING := "22";
		DIFFERENTIAL_DQS     : INTEGER := 0;
		DISABLE_DDR_BLOCKS   : INTEGER := 0;
		STX2_DLL_FREQUENCY   : STRING := "5000ps";
		STX2_DLL_BUFFER_MODE : STRING := "high";
		STX2_DLL_OUT_MODE    : STRING := "delay_chain3"
	);

	PORT
	(
		RST             : IN std_logic;
		CLK             : IN std_logic;
		CLK_90          : IN std_logic;
		CLK_DQS_IN      : IN std_logic;
		CLK_DQS_OUT     : OUT std_logic;
		CLK_DQS_ENA     : IN std_logic;

	-- Stratix II DLL Inputs
		DLL_delay_ctrl  : IN std_logic_vector (5 DOWNTO 0);
		DLL_dqs_update  : IN std_logic;
		DLL_offset_ctrl : IN std_logic_vector(5 DOWNTO 0);

		DATA_IN_L       : IN std_logic_vector(WIDTH - 1 DOWNTO 0);
		DATA_IN_H       : IN std_logic_vector(WIDTH - 1 DOWNTO 0);
		DATA_OUT_L      : OUT std_logic_vector(WIDTH - 1 DOWNTO 0);
		DATA_OUT_H      : OUT std_logic_vector(WIDTH - 1 DOWNTO 0);
		OE              : IN std_logic;
		DQ              : INOUT std_logic_vector(WIDTH - 1 DOWNTO 0);
		DQS_P           : INOUT std_logic;
		DQS_N           : INOUT std_logic
	);
END COMPONENT mtx_sdram_ddr_bidir;

COMPONENT  stratixii_dll
	GENERIC
	(
		DELAY_BUFFER_MODE        : STRING := "low";
		DELAY_CHAIN_LENGTH       : NATURAL := 16;
		DELAYCTRLOUT_MODE        : STRING := "normal";
		INPUT_FREQUENCY          : STRING;
		JITTER_REDUCTION         : STRING := "false";
		OFFSETCTRLOUT_MODE       : STRING := "static";
		SIM_LOOP_DELAY_INCREMENT : NATURAL := 100;
		SIM_LOOP_INTRINSIC_DELAY : NATURAL := 1000;
		SIM_VALID_LOCK           : NATURAL := 1;
		SIM_VALID_LOCKCOUNT      : NATURAL := 90;
		STATIC_DELAY_CTRL        : NATURAL := 0;
		STATIC_OFFSET            : STRING;
		USE_UPNDNIN              : STRING := "false";
		USE_UPNDNINCLKENA        : STRING := "false";
		lpm_type                 : STRING := "stratixii_dll"
	);

	PORT
	(
		addnsub       : IN STD_LOGIC := '1';
		aload         : IN STD_LOGIC := '0';
		clk           : IN STD_LOGIC;
		delayctrlout  : OUT STD_LOGIC_VECTOR(5 DOWNTO 0);
		dqsupdate     : OUT STD_LOGIC;
		offset        : IN STD_LOGIC_VECTOR(5 DOWNTO 0) := (OTHERS => '0');
		offsetctrlout : OUT STD_LOGIC_VECTOR(5 DOWNTO 0);
		upndnin       : IN STD_LOGIC := '0';
		upndninclkena : IN STD_LOGIC := '1';
		upndnout      : OUT STD_LOGIC
	);
END COMPONENT;

-----
-- SDRAM commands
-----
TYPE sdram_cmd_type IS RECORD
	cs  : std_logic;
	ras : std_logic;
	cas : std_logic;
	we  : std_logic;
END RECORD;

-- SDRAM Commands
--                                                        CSn, RASn, CASn, Wen
CONSTANT sdram_cmd_deselect           : sdram_cmd_type := ('1', '1', '1', '1');
CONSTANT sdram_cmd_nop                : sdram_cmd_type := ('0', '1', '1', '1');
CONSTANT sdram_cmd_active             : sdram_cmd_type := ('0', '0', '1', '1');
CONSTANT sdram_cmd_read               : sdram_cmd_type := ('0', '1', '0', '1');
CONSTANT sdram_cmd_write              : sdram_cmd_type := ('0', '1', '0', '0');
CONSTANT sdram_cmd_burst_stop         : sdram_cmd_type := ('0', '1', '1', '0');
CONSTANT sdram_cmd_precharge          : sdram_cmd_type := ('0', '0', '1', '0');
CONSTANT sdram_cmd_refresh            : sdram_cmd_type := ('0', '0', '0', '1');
CONSTANT sdram_cmd_load_mode_register : sdram_cmd_type := ('0', '0', '0', '0');
CONSTANT sdram_cmd_deep_power_down    : sdram_cmd_type := ('0', '1', '1', '0');

-- Simulation
CONSTANT Ttrace       : TIME := 400 ps; -- Trace delay
CONSTANT Tcell        : TIME := 300 ps; -- LCELL Delay
CONSTANT Tpin2reg     : TIME := 0.9 ns; -- Delay from FPGA pin to register
CONSTANT Tmux2pin     : TIME := 1.7 ns; -- Delay from FPGA pin to register
CONSTANT Tfastreg2pin : TIME := 1.2 ns; -- Output delay from Fast Output Register to FPGA pin
CONSTANT Tpin2fastreg : TIME := 0.1 ns; -- Input delay from FPGA pin to Fast Input Register
CONSTANT Tdqs2reg     : TIME :=   1 ns; -- Input delay from DQS pin to Register

END PACKAGE mtx_sdram_package;
