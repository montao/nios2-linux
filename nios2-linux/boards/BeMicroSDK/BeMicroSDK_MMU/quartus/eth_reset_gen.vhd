library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

entity eth_reset_gen is 
	port (
		clk : in std_logic;
		reset_n : in std_logic;

		reset_n_out : out std_logic
		
	);
end entity eth_reset_gen;


architecture rtl of eth_reset_gen is
	signal count : integer range 0 to 65535;
	signal reset_int : std_logic;
begin

	process(reset_n, clk)begin
		if(reset_n = '0')then
			count <= 0;
		elsif(clk'event and clk = '1')then
			if(count = 65535)then
				count <= count;
			else
				count <= count + 1;
			end if;
		end if;
	end process;
	
	process(reset_n, clk)begin
		if(reset_n = '0')then
		elsif(clk'event and clk = '1')then
			if(count < 1024)then
				reset_int <= '1';
			elsif(count < 65530)then
				reset_int <= '0';
			else
				reset_int <= '1';			
			end if;
		end if;
	end process;
	
	reset_n_out <= reset_int;
      
end rtl;
