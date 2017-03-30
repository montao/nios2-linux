---------------------------------
--  Microtronix SDR SDRAM I/O  --
---------------------------------
-- Written by Marco Groeneveld --
---------------------------------
-- History
-- v1.0 - Initial release / 26 April 2006
-----

-----
-- Library
-----
LIBRARY ieee;
USE ieee.std_logic_1164.all;
LIBRARY mtx_sdram;
USE mtx_sdram.mtx_sdram_package.all;

-----
-- Entity
-----
ENTITY mtx_sdram_sdr_bidir IS
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
END ENTITY mtx_sdram_sdr_bidir;

-----
-- Architecture
-----
ARCHITECTURE behaviour OF mtx_sdram_sdr_bidir IS

SIGNAL data_from_padio_reg : std_logic;
SIGNAL data_to_padio_reg   : std_logic;
SIGNAL oe_reg              : std_logic;
SIGNAL padio_data_out      : std_logic;
SIGNAL padio_data_in       : std_logic;

-- Preserve registers and mux
ATTRIBUTE preserve                        : boolean;
ATTRIBUTE preserve OF data_from_padio_reg : signal is true;
ATTRIBUTE preserve OF data_to_padio_reg   : signal is true;
ATTRIBUTE preserve OF oe_reg              : signal is true;

BEGIN

-- Output Enable Register
	PROCESS(RST, CLK_OUT, OE)
	BEGIN
		IF RST = '1' THEN
			oe_reg <= '0';

		ELSIF (CLK_OUT'event AND CLK_OUT = '0') THEN
			oe_reg <= OE;
		END IF;
	END PROCESS;

-- Input Register
	PROCESS(RST, CLK_IN, padio_data_in)
	BEGIN
		IF RST = '1' THEN
			data_from_padio_reg <= '0';

		ELSIF (CLK_IN'event AND CLK_IN = '1') THEN
			data_from_padio_reg <= padio_data_in;
		END IF;
	END PROCESS;
	padio_data_in <= TRANSPORT PADIO AFTER Tpin2fastreg;

	DATA_OUT      <= data_from_padio_reg;

-- Output Register
	PROCESS(RST, CLK_OUT, DATA_IN)
	BEGIN
		IF RST = '1' THEN
			data_to_padio_reg <= '0';

		ELSIF (CLK_OUT'event AND CLK_OUT = '0') THEN
			data_to_padio_reg <= DATA_IN;
		END IF;
	END PROCESS;

-- Tri-State Buffer
	padio_data_out <= data_to_padio_reg WHEN oe_reg = '1' ELSE 'Z';

-- Output the signal. For simulation purposes the delay from the fast out register to the pin is added.
	PADIO <= TRANSPORT padio_data_out AFTER Tfastreg2pin;

END ARCHITECTURE behaviour;
