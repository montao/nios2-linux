---------------------------------
--       SDRAM DDR Output      --
---------------------------------
-- Written by Marco Groeneveld --
---------------------------------
-- History
-- v1.0 - Initial release / 6-11-2005
-- v1.1 - Changed from std_logic_vector to std_logic
-- v1.2 - Added Stratix II support
-- v1.3 - Added Stratix III support
-- v1.4 - Added Arria GX support
-- v1.5 - Added synchronous reset

-----

-----
-- Library
-----
LIBRARY ieee;
USE ieee.std_logic_1164.all;
LIBRARY altera_mf;
USE altera_mf.all;

LIBRARY mtx_sdram;
USE mtx_sdram.mtx_sdram_package.all;

-----
-- Entity
-----
ENTITY mtx_sdram_ddr_output IS
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
END ENTITY mtx_sdram_ddr_output;

-----
-- Architecture
-----
ARCHITECTURE behaviour OF mtx_sdram_ddr_output IS

-- Component
-- Stratix II I/O
	COMPONENT altddio_out
	GENERIC
	(
		extend_oe_disable      : STRING;
		intended_device_family : STRING;
		lpm_type               : STRING;
		oe_reg                 : STRING;
		width                  : NATURAL
	);
	PORT
	(
		dataout  : OUT STD_LOGIC_VECTOR (0 DOWNTO 0);
		outclock : IN STD_LOGIC ;
		datain_h : IN STD_LOGIC_VECTOR (0 DOWNTO 0);
		aclr     : IN STD_LOGIC ;
		datain_l : IN STD_LOGIC_VECTOR (0 DOWNTO 0)
	);
	END COMPONENT;

-- Signals
SIGNAL one              : std_logic;
SIGNAL sdram_reset_line : std_logic_vector(1 DOWNTO 0);
SIGNAL sdram_reset      : std_logic;
SIGNAL reg_low          : std_logic;
SIGNAL reg_high         : std_logic;
SIGNAL mux              : std_logic;
SIGNAL ena_delayed      : std_logic;
SIGNAL data_low         : std_logic;
SIGNAL data_high        : std_logic;

-- Preserve registers and mux
ATTRIBUTE preserve                        : boolean;
ATTRIBUTE preserve OF reg_low             : SIGNAL IS true;
ATTRIBUTE preserve OF reg_high            : SIGNAL IS true;
ATTRIBUTE preserve OF sdram_reset_line    : SIGNAL IS true;
ATTRIBUTE keep                            : boolean;
ATTRIBUTE keep OF mux                     : SIGNAL IS true;
ATTRIBUTE altera_attribute                : string;
ATTRIBUTE altera_attribute OF sdram_reset : SIGNAL IS "-name GLOBAL_SIGNAL OFF";

BEGIN

-- Misc
	one <= '1';

-- Synchronous Reset
-- SDRAM Clock Domain
	PROCESS(RST, CLK_IN,
	  sdram_reset_line)
	BEGIN
		IF RST = '1' THEN
			sdram_reset_line <= (others => '0');
			sdram_reset      <= '1';
		ELSIF (CLK_IN'event AND CLK_IN = '1') THEN
			sdram_reset_line <= sdram_reset_line(sdram_reset_line'high - 1 DOWNTO 0) & '1';
			sdram_reset      <= NOT sdram_reset_line(sdram_reset_line'high);
		END IF;
	END PROCESS;

-- To emulate the same behaviour as the dq pins,
-- the enable signal must be delayed for one clock.
	PROCESS(sdram_reset, CLK_IN, ENA)
	BEGIN
		IF sdram_reset = '1' THEN
			ena_delayed <= '0';

		ELSIF (CLK_IN'event AND CLK_IN = '1') THEN
			ena_delayed <= ENA;
		END IF;
	END PROCESS;

	data_low  <= DATA_IN_L WHEN ena_delayed = '1' AND ENA = '1' ELSE '0';
	data_high <= DATA_IN_H WHEN ena_delayed = '1' AND ENA = '1' ELSE '0';


gen_cycloneii : IF device_family = "Cyclone II" GENERATE

	-- Input Registers
		PROCESS(sdram_reset, CLK_IN, data_low, data_high)
		BEGIN
			IF sdram_reset = '1' THEN
				reg_low  <= '0';
				reg_high <= '0';

			ELSIF (CLK_IN'event AND CLK_IN = '1') THEN

				reg_low  <= data_low;
				reg_high <= data_high;

			END IF;
		END PROCESS;

	-- Ouput Mux
		mux <= reg_low AFTER Tcell WHEN CLK_IN = '0' ELSE reg_high AFTER Tcell;

	-- Output the DDR signal. For simulation purposes the delay from the register to the pin is added.
	DATA_OUT <= TRANSPORT mux AFTER Tmux2pin;
END GENERATE;

gen_stratixii : IF device_family = "Stratix II" OR DEVICE_FAMILY = "Arria GX" OR DEVICE_FAMILY = "Arria II GX" GENERATE
	sii : altddio_out
		GENERIC MAP
		(
			extend_oe_disable      => "UNUSED",
			intended_device_family => DEVICE_FAMILY,
			lpm_type               => "altddio_out",
			oe_reg                 => "UNUSED",
			width                  => 1
		)
		PORT MAP
		(
			outclock    => CLK_IN,
			datain_h(0) => data_high,
			aclr        => sdram_reset,
			datain_l(0) => data_low,
			dataout(0)  => DATA_OUT
		);
END GENERATE;

gen_cycloneiii : IF device_family = "Cyclone III" GENERATE
	cii : altddio_out
		GENERIC MAP
		(
			extend_oe_disable      => "UNUSED",
			intended_device_family => DEVICE_FAMILY,
			lpm_type               => "altddio_out",
			oe_reg                 => "UNUSED",
			width                  => 1
		)
		PORT MAP
		(
			outclock    => CLK_IN,
			datain_h(0) => data_high,
			aclr        => sdram_reset,
			datain_l(0) => data_low,
			dataout(0)  => DATA_OUT
		);
END GENERATE;

gen_cycloneiv : IF device_family = "Cyclone IV E" OR device_family = "Cyclone IV GX" GENERATE
	civ : altddio_out
		GENERIC MAP
		(
			extend_oe_disable      => "UNUSED",
			intended_device_family => DEVICE_FAMILY,
			lpm_type               => "altddio_out",
			oe_reg                 => "UNUSED",
			width                  => 1
		)
		PORT MAP
		(
			outclock    => CLK_IN,
			datain_h(0) => data_high,
			aclr        => sdram_reset,
			datain_l(0) => data_low,
			dataout(0)  => DATA_OUT
		);
END GENERATE;

gen_stratixiii : IF device_family = "Stratix III" GENERATE
	siii : altddio_out
		GENERIC MAP
		(
			extend_oe_disable      => "UNUSED",
			intended_device_family => DEVICE_FAMILY,
			lpm_type               => "altddio_out",
			oe_reg                 => "UNUSED",
			width                  => 1
		)
		PORT MAP
		(
			outclock    => CLK_IN,
			datain_h(0) => data_high,
			aclr        => sdram_reset,
			datain_l(0) => data_low,
			dataout(0)  => DATA_OUT
		);
END GENERATE;

END ARCHITECTURE behaviour;
