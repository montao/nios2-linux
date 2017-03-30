---------------------------------------
--            Microtronix            --
-- SDRAM Memory controller Scheduler --
---------------------------------------
--    Written by Marco Groeneveld    --
---------------------------------------

-- History
-- v1.0 - Initial Release

-----
-- Library
-----
LIBRARY ieee;
USE ieee.std_logic_1164.all;

LIBRARY mtx_sdram;
USE mtx_sdram.mtx_sdram_package.all;

------
-- Entity
------
ENTITY mtx_sdram_controller_scheduler IS
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
END ENTITY mtx_sdram_controller_scheduler;

-----
-- Architecture
-----
ARCHITECTURE behaviour OF mtx_sdram_controller_scheduler IS

-- Signals
SIGNAL zero_total_ports      : std_logic_vector(TOTAL_PORTS - 1 DOWNTO 0);
SIGNAL sdram_request_counter : INTEGER RANGE 0 TO TOTAL_PORTS;
SIGNAL sdram_select          : std_logic_vector(TOTAL_PORTS - 1 DOWNTO 0);
SIGNAL sdram_next_selected   : std_logic;

BEGIN

	zero_total_ports <= (others => '0');

	PROCESS(RST, CLK,
	  PORT_REQUEST, PORT_END, sdram_select)
		VARIABLE increment_port : STD_LOGIC;
	BEGIN
		IF RST = '1' THEN
			sdram_request_counter <= 0;
			sdram_select          <= (others => '0');
			sdram_next_selected   <= '0';

		ELSIF (CLK'event AND CLK = '1') THEN

			FOR i IN 0 TO TOTAL_PORTS - 1 LOOP
				IF PORT_END(i) = '1' THEN
					sdram_select(i) <= '0';
				END IF;
			END LOOP;

			-- Only increment when there is no request pending.
			IF sdram_select = zero_total_ports THEN
				IF sdram_request_counter = TOTAL_PORTS - 1 THEN
					sdram_request_counter <= 0;
				ELSE
					sdram_request_counter <= sdram_request_counter + 1;
				END IF;

				FOR i IN 0 TO TOTAL_PORTS - 1 LOOP
					IF (PORT_REQUEST(i) = '1' AND sdram_request_counter = i) THEN
						sdram_next_selected <= '0';
						sdram_select(i) <= '1';
					END IF;
				END LOOP;
			ELSIF sdram_next_selected = '0' THEN
				increment_port := '1';

				FOR i IN 0 TO TOTAL_PORTS - 1 LOOP
					IF (PORT_REQUEST(i) = '1' AND sdram_request_counter = i AND sdram_select(i) = '0') THEN
						sdram_next_selected <= '1';
						increment_port := '0';
					END IF;
				END LOOP;

				IF increment_port = '1' THEN
					IF sdram_request_counter = TOTAL_PORTS - 1 THEN
						sdram_request_counter <= 0;
					ELSE
						sdram_request_counter <= sdram_request_counter + 1;
					END IF;
				END IF;
			END IF;

		END IF;
	END PROCESS;

-- Map output signal
	PORT_SELECT <= sdram_select;

END ARCHITECTURE behaviour;
