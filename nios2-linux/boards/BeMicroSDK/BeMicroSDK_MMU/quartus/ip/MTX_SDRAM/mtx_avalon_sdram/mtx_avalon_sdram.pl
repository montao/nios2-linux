###
# Microtronix SDRAM SOPC Builder Perl Script v2.0
# Build : 169/9.7.2010
###
#use strict;

my $MODE_DISABLED = 0;
my $MODE_BURST = 1;
my $MODE_RANDOM = 2;

my $ARCH_SDR = 0;
my $ARCH_DDR = 1;
my $ARCH_DDR2 = 2;
my $ARCH_MDDR = 3;

my $filename = shift(@ARGV);
my $directory = shift(@ARGV);
my $top_level_module_name = shift(@ARGV);
my $quartus_sh = shift(@ARGV);
my $device = shift(@ARGV);
my $project_name = shift(@ARGV);

my @ports = ( 'a' .. 'p' );

my %port_mode;
my %port_buffer_size;
my %port_max_burst;
my %port_data_width;
my %port_disable_cache;

my $burst_ports = 0;
my $random_ports = 0;
my $lsp_total_ports = 0;
foreach my $port (@ports) {
	$port_mode{$port} = shift(@ARGV);
	$port_buffer_size{$port} = shift(@ARGV);
	$port_max_burst{$port} = shift(@ARGV);
	$port_data_width{$port} = shift(@ARGV);
	$port_disable_cache{$port} = shift(@ARGV);

	if ( $port_mode{$port} != $MODE_DISABLED ) {
		$lsp_total_ports++;
		if ( $port_mode{$port} == $MODE_RANDOM ) {
			$random_ports++;
		} else {
			$burst_ports++;
		}
	}
}

my $mem_architecture = shift(@ARGV);
my $mem_scheduler_parameter = shift(@ARGV);
my $mem_data_width = shift(@ARGV);
my $mem_bank_bits = shift(@ARGV);
my $mem_row_bits = shift(@ARGV);
my $mem_column_bits = shift(@ARGV);
my $mem_prefix = shift(@ARGV);
my $mem_devices = shift(@ARGV);
my $mem_dqs = shift(@ARGV);
my $mem_diff_dqs = shift(@ARGV);
my $mem_clocks = shift(@ARGV);
my $mem_reduced_drive_strength = shift(@ARGV);
my $mem_phase_shift = shift(@ARGV);
my $mem_deep_power_down = shift(@ARGV);
my $mem_odt = shift(@ARGV);
my $mem_disable_ddr_blocks = shift(@ARGV);
my $mem_manual_scripts = shift(@ARGV);

my $timing_frequency = shift(@ARGV);
my $timing_tinit = shift(@ARGV);
my $timing_trp = shift(@ARGV);
my $timing_tmrd = shift(@ARGV);
my $timing_trfc = shift(@ARGV);
my $timing_trefi = shift(@ARGV);
my $timing_trcd = shift(@ARGV);
my $timing_twr = shift(@ARGV);
my $timing_tras = shift(@ARGV);
my $timing_trc = shift(@ARGV);
my $timing_tac = shift(@ARGV);
my $timing_toh = shift(@ARGV);
my $timing_pcb_delay = shift(@ARGV);

my $sdc_enable = shift(@ARGV);
my $sdc_tck = shift(@ARGV);
my $sdc_tds = shift(@ARGV);
my $sdc_tdh = shift(@ARGV);
my $sdc_tis = shift(@ARGV);
my $sdc_tih = shift(@ARGV);
my $sdc_tdss = shift(@ARGV);
my $sdc_tdsh = shift(@ARGV);
my $sdc_tdqsq = shift(@ARGV);
my $sdc_tqhs = shift(@ARGV);
my $sdc_tdqsck = shift(@ARGV);
my $sdc_tdqss = shift(@ARGV);
my $sdc_input = shift(@ARGV);


my $mem_data_mask_width;
my $mem_data_strobe_width = $mem_devices * $mem_dqs;
my $mem_data_group_width;
my $mem_cas_latency;
my $mem_dqs_delay;
my $mem_dqs_filter;
my $mem_linear_addr_width;
my $mem_bank_addr_high;
my $mem_bank_addr_low;
my $mem_row_addr_high;
my $mem_row_addr_low;
my $mem_column_addr_high;
my $mem_column_addr_low;
my $mem_oe_latency;
my $mem_cmd_latency;
my $mem_read_latency;
my $mem_write_data_delay;

my $lsp_addr_width;
my $lsp_data_width;
my $lsp_data_mask_width;
my $lsp_write_data_width;
my $lsp_burst_width;
my $lsp_burst_length;

my $tINIT;
my $tRP;
my $tMRD;
my $tRFC;
my $tREFI;
my $tRCD;
my $tWR;
my $tRAS;
my $tRC;

my $dq_to_core_delay;
my $dqs_to_core_delay;
my $stx2_dll_frequency;
my $stx2_dll_chain_length;
my $stx2_dll_buffer_mode;
my $stx2_dll_out_mode;
my $stx3_d1_delay;
my $stx3_d2_delay;
my $stx3_d3_delay;
my $stx3_d4_delay;

my $device_family;

my $num_words;
my $num_bits;


###
# Subroutines
###

sub add_sdram {
	if ($mem_phase_shift eq 'true')
	{
		$top_port .= qq
		{
		CMD_CLK     : IN std_logic;

		};
	}

	if ($mem_architecture == $ARCH_MDDR && $mem_deep_power_down eq 'true')
	{
		$top_port .= qq
		{
	-- Control Interface
		DPD         : IN std_logic;
		POWER_STATE : OUT std_logic;

		};
	}

	$top_port .= qq
	{
	-- SDRAM Interface
		CKE         : OUT std_logic;
		CS          : OUT std_logic;
		WE          : OUT std_logic;
		CAS         : OUT std_logic;
		RAS         : OUT std_logic;
	};

	if ($mem_bank_bits == 1)
	{
		$top_port .= qq
		{
		BA          : OUT std_logic;
		};
	} else {
		$top_port .= qq
		{
		BA          : OUT std_logic_vector($mem_bank_bits - 1 DOWNTO 0);
		};
	}

	$top_port .= qq
	{
		A           : OUT std_logic_vector($mem_row_bits - 1 DOWNTO 0);
	};

	if ($mem_data_mask_width == 1)
	{
		$top_port .= qq
		{
		DQM         : OUT std_logic;
		};
	} else {
		$top_port .= qq
		{
		DQM         : OUT std_logic_vector($mem_data_mask_width - 1 DOWNTO 0);
		};
	}


	$top_port .= qq
	{
		DQ          : INOUT std_logic_vector($mem_data_width - 1 DOWNTO 0);
	};

	if ($mem_architecture == $ARCH_SDR)
	{
		$top_port .= qq
		{
		CAPTURE_CLK		: IN std_logic
	);
		};
	}

	if ($mem_architecture == $ARCH_DDR | $mem_architecture == $ARCH_MDDR)
	{
		$top_port .= qq
		{
		WRITE_CLK   : IN std_logic;
		};

 		if ($mem_data_strobe_width == 1)
		{
			$top_port .= qq
			{
		DQS         : INOUT std_logic;
			};
		} else {
			$top_port .= qq
			{
		DQS         : INOUT std_logic_vector($mem_data_strobe_width - 1 DOWNTO 0);
			};
		}

		if ($mem_clocks == 1)
		{
			$top_port .= qq
			{
		CLK_OUT_P   : OUT std_logic;
		CLK_OUT_N   : OUT std_logic
	);
			};
		} else {
			$top_port .= qq
			{
		CLK_OUT_P   : OUT std_logic_vector($mem_clocks - 1 DOWNTO 0);
		CLK_OUT_N   : OUT std_logic_vector($mem_clocks - 1 DOWNTO 0)
	);
			};
		}
	}

	if ($mem_architecture == $ARCH_DDR2)
	{
		$top_port .= qq
		{
		WRITE_CLK   : IN std_logic;
		};

		if ($mem_data_strobe_width == 1)
		{
			if (($mem_diff_dqs eq 'true') & ($device_family eq 'Stratix III'))
			{
				$top_port .= qq
				{
		DQS_P       : INOUT std_logic;
		DQS_N       : INOUT std_logic;
				};
			} else {
				$top_port .= qq
				{
		DQS         : INOUT std_logic;
				};
			}
		} else {
			if (($mem_diff_dqs eq 'true') & ($device_family eq 'Stratix III'))
			{
				$top_port .= qq
				{
		DQS_P       : INOUT std_logic_vector($mem_data_strobe_width - 1 DOWNTO 0);
		DQS_N       : INOUT std_logic_vector($mem_data_strobe_width - 1 DOWNTO 0);
				};
			} else {
				$top_port .= qq
				{
		DQS         : INOUT std_logic_vector($mem_data_strobe_width - 1 DOWNTO 0);
				};
			}
		}

		$top_port .= qq
		{
		ODT         : OUT std_logic;
		};

		if ($mem_clocks == 1)
		{
			$top_port .= qq
			{
		CLK_OUT_P   : OUT std_logic;
		CLK_OUT_N   : OUT std_logic
	);
			};
		} else {
			$top_port .= qq
			{
		CLK_OUT_P   : OUT std_logic_vector($mem_clocks - 1 DOWNTO 0);
		CLK_OUT_N   : OUT std_logic_vector($mem_clocks - 1 DOWNTO 0)
	);
			};
		}
	}

	my $diff_dqs;
	my $reduced_drive_strength;
	my $phase_shift;
	my $disable_ddr_blocks;

	if ( $mem_diff_dqs eq 'true' )
	{
		$diff_dqs = 1;
	}
	else
	{
		$diff_dqs = 0;
	}

	if ( $mem_reduced_drive_strength eq 'true' )
	{
		$reduced_drive_strength = 1;
	}
	else
	{
		$reduced_drive_strength = 0;
	}

	if ( $mem_phase_shift eq 'true' )
	{
		$phase_shift = 1;
	}
	else
	{
		$phase_shift = 0;
	}

	if ( ($mem_disable_ddr_blocks eq 'true') & (($device_family eq 'Stratix III') | ($device_family eq 'Stratix II')))
	{
		$disable_ddr_blocks = 1;
	}
	else
	{
		$disable_ddr_blocks = 0;
	}

	# Component
	$top_component .= qq
	{
sdram : mtx_sdram_controller
	GENERIC MAP
	(

	-- Local SDRAM Port
		lsp_addr_width               => $lsp_addr_width,
		lsp_data_width               => $lsp_data_width,
		lsp_data_mask_width          => $lsp_data_mask_width,
		lsp_burst_width              => $lsp_burst_width,
		lsp_total_ports              => $lsp_total_ports,

	-- SDRAM Settings
		sdram_architecture           => $mem_architecture,
		sdram_scheduler_parameter    => $mem_scheduler_parameter,
		sdram_data_strobe_width      => $mem_data_strobe_width,
		sdram_data_group_width       => $mem_data_group_width,
		sdram_max_burst_length       => $lsp_burst_length,
		sdram_cas_latency            => $mem_cas_latency,
		sdram_dqs_delay              => "$mem_dqs_delay",
		sdram_dqs_filter             => $mem_dqs_filter,
		sdram_devices                => $mem_devices,
		sdram_clock_pairs            => $mem_clocks,
		sdram_dqs_per_device         => $mem_dqs,
		sdram_differential_dqs       => $diff_dqs,
		sdram_reduced_drive_strength => $reduced_drive_strength,
		sdram_command_clock          => $phase_shift,
		sdram_odt_value              => $mem_odt,
		sdram_disable_ddr_blocks     => $disable_ddr_blocks,

		sdram_linear_addr_width      => $mem_linear_addr_width,
		sdram_addr_width             => $mem_row_bits,
		sdram_data_width             => $mem_data_width,
		sdram_data_mask_width        => $mem_data_mask_width,
		sdram_bank_addr_width        => $mem_bank_bits,
		sdram_bank_addr_high         => $mem_bank_addr_high,
		sdram_bank_addr_low          => $mem_bank_addr_low,
		sdram_row_addr_width         => $mem_row_bits,
		sdram_row_addr_high          => $mem_row_addr_high,
		sdram_row_addr_low           => $mem_row_addr_low,
		sdram_column_addr_width      => $mem_column_bits,
		sdram_column_addr_high       => $mem_column_addr_high,
		sdram_column_addr_low        => $mem_column_addr_low,

		sdram_oe_latency             => $mem_oe_latency,
		sdram_cmd_latency            => $mem_cmd_latency,
		sdram_read_latency           => $mem_read_latency,
		sdram_write_data_delay       => $mem_write_data_delay,

		device_family                => "$device_family",
		stx2_dll_frequency           => "$stx2_dll_frequency",
		stx2_dll_chain_length        => $stx2_dll_chain_length,
		stx2_dll_buffer_mode         => "$stx2_dll_buffer_mode",
		stx2_dll_out_mode            => "$stx2_dll_out_mode",

	-- SDRAM Timing
		tINIT                        => $tINIT,
		tRP                          => $tRP,
		tMRD                         => $tMRD,
		tRFC                         => $tRFC,
		tREFI                        => $tREFI,
		tRCD                         => $tRCD,
		tWR                          => $tWR,
		tRAS                         => $tRAS,
		tRC                          => $tRC
	)

	PORT MAP
	(
		RST                 => AS_PORT_A_RST,
		SDRAM_CLK           => CLK,
	};

	if ($mem_phase_shift eq 'true')
	{
		$top_component .= qq
		{
		SDRAM_CMD_CLK       => CMD_CLK,
		};
	} else {
		$top_component .= qq
		{
		SDRAM_CMD_CLK       => zero,
		};
	}

	$top_component .= qq
	{
	-- Local SDRAM Port (LSP)
		LSP_ADDR            => LSP_ADDR,
		LSP_DIR             => LSP_DIR,
		LSP_READ            => LSP_READ,
		LSP_WRITE           => LSP_WRITE,
		LSP_LENGTH          => LSP_LENGTH,
		LSP_REQUEST         => LSP_REQUEST,
		LSP_GRANDED         => LSP_GRANDED,
		LSP_PREAMBLE        => LSP_PREAMBLE,
		LSP_END             => LSP_END,
		LSP_DATA_IN         => LSP_DATA_IN,
		LSP_DATA_OUT        => LSP_DATA_OUT,
		LSP_DATA_MASK       => LSP_DATA_MASK,
		LSP_DATA_OUT_CLK    => LSP_DATA_OUT_CLK,
		LSP_READY           => LSP_READY,

	};

	if ($mem_architecture == $ARCH_MDDR && $mem_deep_power_down eq 'true')
	{
		$top_component .= qq
		{
	-- CONTROL
		CONTROL_DPD         => DPD,
		CONTROL_POWER_STATE => POWER_STATE,

		};
	} else {
		$top_component .= qq
		{
		-- CONTROL
		CONTROL_DPD         => zero,
		CONTROL_POWER_STATE => open,

		};
	}

	$top_component .= qq
	{
	-- SDRAM Interface
		SDRAM_CKE           => CKE,
		SDRAM_CS            => CS,
		SDRAM_WE            => WE,
		SDRAM_CAS           => CAS,
		SDRAM_RAS           => RAS,
	};

	if ($mem_bank_bits == 1)
	{
		$top_component .= qq
		{
		SDRAM_BA(0)         => BA,
		};
	} else {
		$top_component .= qq
		{
		SDRAM_BA            => BA,
		};
	}

	$top_component .= qq
	{
		SDRAM_A             => A,
	};

	if ($mem_data_mask_width == 1)
	{
		$top_component .= qq
		{
		SDRAM_DQM(0)        => DQM,
		};
	} else {
		$top_component .= qq
		{
		SDRAM_DQM           => DQM,
		};
	}

	$top_component .= qq
	{
		SDRAM_DQ            => DQ,
	};

	if ($mem_architecture == $ARCH_SDR)
	{
		$top_component .= qq
		{
	-- SDR
		SDRAM_CAPTURE_CLK   => CAPTURE_CLK, 

	-- DDR
		SDRAM_WRITE_CLK     => zero, 
		SDRAM_CLK_OUT_P     => open,
		SDRAM_CLK_OUT_N     => open,
		SDRAM_DQS_P         => open,
		SDRAM_DQS_N         => open,
		SDRAM_ODT           => open
	);
		};
	}

	if ($mem_architecture == $ARCH_DDR | $mem_architecture == $ARCH_MDDR)
	{
		$top_component .= qq
		{
	-- SDR
		SDRAM_CAPTURE_CLK   => zero,

	-- DDR
		SDRAM_WRITE_CLK     => WRITE_CLK,
		};

		if ($mem_data_strobe_width == 1)
		{
			$top_component .= qq
			{
		SDRAM_DQS_P(0)      => DQS,
			};
		} else {
			$top_component .= qq
			{
		SDRAM_DQS_P         => DQS,
			};
		}

		$top_component .= qq
		{
		SDRAM_DQS_N         => open,
		SDRAM_ODT           => open,
		};


		if ($mem_clocks == 1)
		{
			$top_component .= qq
			{
		SDRAM_CLK_OUT_P(0)  => CLK_OUT_P,
		SDRAM_CLK_OUT_N(0)  => CLK_OUT_N
	);
			};
		} else {
			$top_component .= qq
			{
		SDRAM_CLK_OUT_P     => CLK_OUT_P,
		SDRAM_CLK_OUT_N     => CLK_OUT_N
	);
			};
		}
	}

	if ($mem_architecture == $ARCH_DDR2)
	{
		$top_component .= qq
		{
	-- SDR
		SDRAM_CAPTURE_CLK   => zero,

	-- DDR
		SDRAM_WRITE_CLK     => WRITE_CLK,
		};

		if ($mem_data_strobe_width == 1)
		{
			if ($mem_diff_dqs eq 'true' & $device_family eq 'Stratix III')
			{
				$top_component .= qq
				{
		SDRAM_DQS_P(0)      => DQS_P,
		SDRAM_DQS_N(0)      => DQS_N,
				};
			} else {
				$top_component .= qq
				{
		SDRAM_DQS_P(0)      => DQS,
		SDRAM_DQS_N         => open,
				};
			}
		} else {
			if ($mem_diff_dqs eq 'true' & $device_family eq 'Stratix III')
			{
				$top_component .= qq
				{
		SDRAM_DQS_P         => DQS_P,
		SDRAM_DQS_N         => DQS_N,
				};
			} else {
				$top_component .= qq
				{
		SDRAM_DQS_P         => DQS,
		SDRAM_DQS_N         => open,
				};
			}
		}

		$top_component .= qq
		{
		SDRAM_ODT           => ODT,
		};


		if ($mem_clocks == 1)
		{
			$top_component .= qq
			{
		SDRAM_CLK_OUT_P(0)  => CLK_OUT_P,
		SDRAM_CLK_OUT_N(0)  => CLK_OUT_N
	);
			};
		} else {
			$top_component .= qq
			{
		SDRAM_CLK_OUT_P     => CLK_OUT_P,
		SDRAM_CLK_OUT_N     => CLK_OUT_N
	);
			};
		}
	}

	$top_signals .= qq
	{
SIGNAL LSP_ADDR         : std_logic_vector(($lsp_total_ports * $lsp_addr_width) - 1 DOWNTO 0);
SIGNAL LSP_DATA_IN      : std_logic_vector(($lsp_total_ports * $lsp_data_width) - 1 DOWNTO 0);
SIGNAL LSP_DATA_OUT     : std_logic_vector($lsp_data_width - 1 DOWNTO 0);
SIGNAL LSP_DATA_MASK    : std_logic_vector(($lsp_total_ports * $lsp_data_mask_width) - 1 DOWNTO 0);
SIGNAL LSP_READ         : std_logic_vector($lsp_total_ports - 1 DOWNTO 0);
SIGNAL LSP_WRITE        : std_logic_vector(($lsp_total_ports * $mem_devices) - 1 DOWNTO 0);
SIGNAL LSP_DIR          : std_logic_vector($lsp_total_ports - 1 DOWNTO 0);
SIGNAL LSP_LENGTH       : std_logic_vector(($lsp_total_ports * $lsp_burst_width) - 1 DOWNTO 0);
SIGNAL LSP_REQUEST      : std_logic_vector($lsp_total_ports - 1 DOWNTO 0);
SIGNAL LSP_GRANDED      : std_logic_vector($lsp_total_ports - 1 DOWNTO 0);
SIGNAL LSP_PREAMBLE     : std_logic_vector($lsp_total_ports - 1 DOWNTO 0);
SIGNAL LSP_END          : std_logic_vector($lsp_total_ports - 1 DOWNTO 0);
SIGNAL LSP_DATA_OUT_CLK : std_logic_vector($mem_devices - 1 DOWNTO 0);
SIGNAL LSP_READY        : std_logic;
	};

}

sub add_random_port {
	my ($port_letter, $port_index, $avalon_addr_width, $avalon_data_width, $avalon_data_mask_width, $fifo_total_words, $fifo_addr_width, $cache_disable) = @_;
	my $port_identifier = '_'.$port_letter.'_';

	# Port
	$top_port .= qq
	{
	-- Port $port_letter (random)
		AS_PORT_XX_RST      : IN std_logic;
		AS_PORT_XX_CLK      : IN std_logic;
		AS_PORT_XX_CS       : IN std_logic;
		AS_PORT_XX_RD       : IN std_logic;
		AS_PORT_XX_WR       : IN std_logic;
		AS_PORT_XX_ADDR     : IN std_logic_vector($avalon_addr_width - 1 DOWNTO 0);
		AS_PORT_XX_BE       : IN std_logic_vector($avalon_data_mask_width - 1 DOWNTO 0);
		AS_PORT_XX_DATA_IN  : IN std_logic_vector($avalon_data_width - 1 DOWNTO 0);
		AS_PORT_XX_DATA_OUT : OUT std_logic_vector($avalon_data_width - 1 DOWNTO 0);
		AS_PORT_XX_WAITREQ  : OUT std_logic;
	};

	$top_port =~ s/_XX_/$port_identifier/g;

	# Component
	$top_component .= qq
	{
port_$port_letter : mtx_sdram_avalon_random
	GENERIC MAP
	(

	-- Avalon
		avalon_addr_width      => $avalon_addr_width,
		avalon_data_width      => $avalon_data_width,
		avalon_data_mask_width => $avalon_data_mask_width,

	-- Local SDRAM Port
		lsp_addr_width         => $lsp_addr_width,
		lsp_data_width         => $lsp_data_width,
		lsp_data_mask_width    => $lsp_data_mask_width,
		lsp_burst_width        => $lsp_burst_width,
		lsp_write_clocks       => $mem_devices,
		lsp_write_data_width   => $lsp_write_data_width,

	-- buffer
		DEVICE_FAMILY          => "$device_family",
		CACHE_TOTAL_WORDS      => $fifo_total_words,
		CACHE_ADDR_WIDTH       => $fifo_addr_width,
		CACHE_DISABLE          => $cache_disable
	)

	PORT MAP
	(
	-- Avalon Slave Interface
		AS_RST      => AS_PORT_XX_RST,
		AS_CLK      => AS_PORT_XX_CLK,
		AS_CS       => AS_PORT_XX_CS,
		AS_RD       => AS_PORT_XX_RD,
		AS_WR       => AS_PORT_XX_WR,
		AS_ADDR     => AS_PORT_XX_ADDR,
		AS_BE       => AS_PORT_XX_BE,
		AS_DATA_IN  => AS_PORT_XX_DATA_IN,
		AS_DATA_OUT => AS_PORT_XX_DATA_OUT,
		AS_WAITREQ  => AS_PORT_XX_WAITREQ,

	-- Local SDRAM Port
		LSP_CLK         => CLK,
		LSP_READY       => LSP_READY,
		LSP_ADDR        => LSP_XX_ADDR,
		LSP_DIR         => LSP_XX_DIR,
		LSP_LENGTH      => LSP_XX_LENGTH,
		LSP_READ        => LSP_XX_READ,
		LSP_WRITE       => LSP_XX_WRITE,
		LSP_REQUEST     => LSP_XX_REQUEST,
		LSP_GRANDED     => LSP_XX_GRANDED,
		LSP_PREAMBLE    => LSP_XX_PREAMBLE,
		LSP_END         => LSP_XX_END,
		LSP_DATA_IN     => LSP_XX_DATA_IN,
		LSP_DATA_OUT    => LSP_XX_DATA_OUT,
		LSP_DATA_MASK   => LSP_XX_DATA_MASK,
		LSP_DATA_IN_CLK => LSP_XX_DATA_IN_CLK
	);
	};

	$top_component =~ s/_XX_/$port_identifier/g;
	# Signals
	$top_signals .= qq
	{
SIGNAL LSP_XX_ADDR        : std_logic_vector($lsp_addr_width - 1 DOWNTO 0);
SIGNAL LSP_XX_DATA_IN     : std_logic_vector($lsp_data_width - 1 DOWNTO 0);
SIGNAL LSP_XX_DATA_OUT    : std_logic_vector($lsp_data_width - 1 DOWNTO 0);
SIGNAL LSP_XX_DATA_MASK   : std_logic_vector($lsp_data_mask_width - 1 DOWNTO 0);
SIGNAL LSP_XX_WRITE       : std_logic_vector($mem_devices - 1 DOWNTO 0);
SIGNAL LSP_XX_READ        : std_logic;
SIGNAL LSP_XX_DIR         : std_logic;
SIGNAL LSP_XX_LENGTH      : std_logic_vector($lsp_burst_width - 1 DOWNTO 0);
SIGNAL LSP_XX_REQUEST     : std_logic;
SIGNAL LSP_XX_GRANDED     : std_logic;
SIGNAL LSP_XX_PREAMBLE    : std_logic;
SIGNAL LSP_XX_END         : std_logic;
SIGNAL LSP_XX_DATA_IN_CLK : std_logic_vector($mem_devices - 1 DOWNTO 0);
	};

	$top_signals =~ s/_XX_/$port_identifier/g;

	$lsp_write_index_low = $port_index * $mem_devices;
	$lsp_write_index_high = (($port_index + 1) * $mem_devices) - 1;

	# Signal Map
	$top_signals_map .= qq
	{
	LSP_XX_DATA_IN     <= LSP_DATA_OUT;
	LSP_XX_READ        <= LSP_READ($port_index);
	LSP_XX_WRITE       <= LSP_WRITE($lsp_write_index_high DOWNTO $lsp_write_index_low);
	LSP_XX_END         <= LSP_END($port_index);
	LSP_XX_PREAMBLE    <= LSP_PREAMBLE($port_index);
	LSP_XX_GRANDED     <= LSP_GRANDED($port_index);
	LSP_XX_DATA_IN_CLK <= LSP_DATA_OUT_CLK;
	LSP_REQUEST($port_index)    <= LSP_XX_REQUEST;
	LSP_DIR($port_index)        <= LSP_XX_DIR;
	};

	$top_signals_map =~ s/_XX_/$port_identifier/g;


	$top_lsp_addr .= 'LSP_'.$port_letter.'_ADDR';
	$top_lsp_data_in .= 'LSP_'.$port_letter.'_DATA_OUT';
	$top_lsp_data_mask .= 'LSP_'.$port_letter.'_DATA_MASK';
	$top_lsp_length .= 'LSP_'.$port_letter.'_LENGTH';

	if ($port_letter ne 'A')
	{
		$top_lsp_addr .= ' & ';
		$top_lsp_data_in .= ' & ';
		$top_lsp_data_mask .= ' & ';
		$top_lsp_length .= ' & ';
	}
}

sub add_burst_port {
	my ($port_letter, $port_index, $avalon_addr_width, $avalon_data_width, $avalon_data_mask_width, $fifo_total_words, $fifo_addr_width, $avalon_burst_width, $burst_dataenable, $data_width_ratio) = @_;
	my $port_identifier = '_'.$port_letter.'_';
	my $dataenable_driver;

	my $lsp_fifo_total_words;
	my $lsp_fifo_addr_width;

	if ($data_width_ratio == 2)
	{
		$lsp_fifo_total_words = $fifo_total_words * 2;
		$lsp_fifo_addr_width = $fifo_addr_width + 1;
	}
	else
	{
		$lsp_fifo_total_words = $fifo_total_words;
		$lsp_fifo_addr_width = $fifo_addr_width;
	}

	# Port
	$top_port .= qq
	{
	-- Port $port_letter (burst)
		AS_PORT_XX_RST        : IN std_logic;
		AS_PORT_XX_CLK        : IN std_logic;
		AS_PORT_XX_CS         : IN std_logic;
		AS_PORT_XX_RD         : IN std_logic;
		AS_PORT_XX_WR         : IN std_logic;
		AS_PORT_XX_ADDR       : IN std_logic_vector($avalon_addr_width - 1 DOWNTO 0);
		AS_PORT_XX_BURSTCOUNT : IN std_logic_vector($avalon_burst_width - 1 DOWNTO 0);
		AS_PORT_XX_BE         : IN std_logic_vector($avalon_data_mask_width - 1 DOWNTO 0);
		AS_PORT_XX_DATA_IN    : IN std_logic_vector($avalon_data_width - 1 DOWNTO 0);
		AS_PORT_XX_DATA_OUT   : OUT std_logic_vector($avalon_data_width - 1 DOWNTO 0);
		AS_PORT_XX_WAITREQ    : OUT std_logic;
		AS_PORT_XX_DATAVALID  : OUT std_logic;
	};

	if ($burst_dataenable == 1)
	{
		$dataenable_driver = 'AS_PORT_XX_DATAENABLE';
		$top_port .= qq
		{
		AS_PORT_XX_DATAENABLE : IN std_logic;
		};
	} else {
		$dataenable_driver = 'one';
	}


	$top_port =~ s/_XX_/$port_identifier/g;


	# Component
	$top_component .= qq
	{
port_$port_letter : mtx_sdram_avalon_burst
	GENERIC MAP
	(
	-- Avalon
		avalon_addr_width      => $avalon_addr_width,
		avalon_data_width      => $avalon_data_width,
		avalon_data_mask_width => $avalon_data_mask_width,
		avalon_burst_width     => $avalon_burst_width,

	-- Local SDRAM Port
		lsp_addr_width         => $lsp_addr_width,
		lsp_data_width         => $lsp_data_width,
		lsp_data_mask_width    => $lsp_data_mask_width,
		lsp_burst_width        => $lsp_burst_width,
		lsp_write_clocks       => $mem_devices,
		lsp_write_data_width   => $lsp_write_data_width,

	-- buffer
		DEVICE_FAMILY          => "$device_family",
		AV_FIFO_TOTAL_WORDS    => $fifo_total_words,
		AV_FIFO_ADDR_WIDTH     => $fifo_addr_width,
		LSP_FIFO_TOTAL_WORDS   => $lsp_fifo_total_words,
		LSP_FIFO_ADDR_WIDTH    => $lsp_fifo_addr_width,
		DATA_WIDTH_RATIO       => $data_width_ratio
	)

	PORT MAP
	(
	-- Avalon Slave Interface
		AS_RST        => AS_PORT_XX_RST,
		AS_CLK        => AS_PORT_XX_CLK,
		AS_CS         => AS_PORT_XX_CS,
		AS_RD         => AS_PORT_XX_RD,
		AS_WR         => AS_PORT_XX_WR,
		AS_ADDR       => AS_PORT_XX_ADDR,
		AS_BURSTCOUNT => AS_PORT_XX_BURSTCOUNT,
		AS_BE         => AS_PORT_XX_BE,
		AS_DATA_IN    => AS_PORT_XX_DATA_IN,
		AS_DATA_OUT   => AS_PORT_XX_DATA_OUT,
		AS_WAITREQ    => AS_PORT_XX_WAITREQ,
		AS_DATAVALID  => AS_PORT_XX_DATAVALID,
		AS_DATAENABLE => $dataenable_driver,

	-- Local SDRAM Port
		LSP_CLK         => CLK,
		LSP_READY       => LSP_READY,
		LSP_ADDR        => LSP_XX_ADDR,
		LSP_DIR         => LSP_XX_DIR,
		LSP_LENGTH      => LSP_XX_LENGTH,
		LSP_READ        => LSP_XX_READ,
		LSP_WRITE       => LSP_XX_WRITE,
		LSP_REQUEST     => LSP_XX_REQUEST,
		LSP_GRANDED     => LSP_XX_GRANDED,
		LSP_PREAMBLE    => LSP_XX_PREAMBLE,
		LSP_END         => LSP_XX_END,
		LSP_DATA_IN     => LSP_XX_DATA_IN,
		LSP_DATA_OUT    => LSP_XX_DATA_OUT,
		LSP_DATA_MASK   => LSP_XX_DATA_MASK,
		LSP_DATA_IN_CLK => LSP_XX_DATA_IN_CLK
	);
	};

	$top_component =~ s/_XX_/$port_identifier/g;


	# Signals
	$top_signals .= qq
	{
SIGNAL LSP_XX_ADDR        : std_logic_vector($lsp_addr_width - 1 DOWNTO 0);
SIGNAL LSP_XX_DATA_IN     : std_logic_vector($lsp_data_width - 1 DOWNTO 0);
SIGNAL LSP_XX_DATA_OUT    : std_logic_vector($lsp_data_width - 1 DOWNTO 0);
SIGNAL LSP_XX_DATA_MASK   : std_logic_vector($lsp_data_mask_width - 1 DOWNTO 0);
SIGNAL LSP_XX_WRITE       : std_logic_vector($mem_devices - 1 DOWNTO 0);
SIGNAL LSP_XX_READ        : std_logic;
SIGNAL LSP_XX_DIR         : std_logic;
SIGNAL LSP_XX_LENGTH      : std_logic_vector($lsp_burst_width - 1 DOWNTO 0);
SIGNAL LSP_XX_REQUEST     : std_logic;
SIGNAL LSP_XX_GRANDED     : std_logic;
SIGNAL LSP_XX_PREAMBLE    : std_logic;
SIGNAL LSP_XX_END         : std_logic;
SIGNAL LSP_XX_DATA_IN_CLK : std_logic_vector($mem_devices - 1 DOWNTO 0);
	};

	$top_signals =~ s/_XX_/$port_identifier/g;

	$lsp_write_index_low = $port_index * $mem_devices;
	$lsp_write_index_high = (($port_index + 1) * $mem_devices) - 1;

	# Signal Map
	$top_signals_map .= qq
	{
	LSP_XX_DATA_IN     <= LSP_DATA_OUT;
	LSP_XX_READ        <= LSP_READ($port_index);
	LSP_XX_WRITE       <= LSP_WRITE($lsp_write_index_high DOWNTO $lsp_write_index_low);
	LSP_XX_END         <= LSP_END($port_index);
	LSP_XX_PREAMBLE    <= LSP_PREAMBLE($port_index);
	LSP_XX_GRANDED     <= LSP_GRANDED($port_index);
	LSP_XX_DATA_IN_CLK <= LSP_DATA_OUT_CLK;
	LSP_REQUEST($port_index)    <= LSP_XX_REQUEST;
	LSP_DIR($port_index)        <= LSP_XX_DIR;
	};

	$top_signals_map =~ s/_XX_/$port_identifier/g;


	$top_lsp_addr .= 'LSP_'.$port_letter.'_ADDR';
	$top_lsp_data_in .= 'LSP_'.$port_letter.'_DATA_OUT';
	$top_lsp_data_mask .= 'LSP_'.$port_letter.'_DATA_MASK';
	$top_lsp_length .= 'LSP_'.$port_letter.'_LENGTH';

	if ($port_letter ne 'A')
	{
		$top_lsp_addr .= ' & ';
		$top_lsp_data_in .= ' & ';
		$top_lsp_data_mask .= ' & ';
		$top_lsp_length .= ' & ';
	}
}

$mem_data_mask_width = $mem_data_width / 8;
$mem_data_strobe_width = $mem_devices * $mem_dqs;

###
# Get Device Family
###

my %translate_device_name = (
	'CYCLONE' => 'Cyclone',
	'CYCLONEII' => 'Cyclone II',
	'CYCLONEIII' => 'Cyclone III',
	'TARPON' => 'Cyclone III LS',
	'CYCLONEIVE' => 'Cyclone IV E',
	'STINGRAY' => 'Cyclone IV GX',
	'STRATIX' => 'Stratix',
	'STRATIXGX' => 'Stratix',
	'STRATIXII' => 'Stratix II',
	'STRATIXIIGX' => 'Stratix II',
	'HARDCOPYII' => 'Stratix II',
	'STRATIXIII' => 'Stratix III',
	'HARDCOPYIII' => 'Stratix III',
	'STRATIXIIGXLITE' => 'Arria GX',
	'ARRIAII' => 'Arria II GX',
);

$device_family = $translate_device_name{$device};

###
# LSP bus widths
###
$num_words = 2**$mem_row_bits * 2**$mem_column_bits * 2**$mem_bank_bits;
$num_bits = $num_words * $mem_data_width;
$mem_linear_addr_width = int((log($num_words)/log(2)) + 0.9999999999);

$lsp_addr_width = $mem_linear_addr_width;

if ($mem_architecture == $ARCH_SDR)
{
	$lsp_data_width = $mem_data_width;
	$lsp_data_mask_width = $mem_data_mask_width;
}
else
{
	$lsp_data_width = $mem_data_width * 2;
	$lsp_data_mask_width = $mem_data_mask_width * 2;
}
$lsp_write_data_width = $lsp_data_width / $mem_devices;

###
# Find the maximum burst width
###

$lsp_burst_length = 0;

foreach my $port ( @ports ) {
	if ( $port_mode{$port} ne 0 )
	{
		if ( $port_buffer_size{$port} > $lsp_burst_length ) 
		{
			$lsp_burst_length = $port_buffer_size{$port};
		}
	}
}
$lsp_burst_width =  int((log($lsp_burst_length)/log(2)) + 0.9999999999) + 1;

###
# Define CAS latency
###

$mem_dqs_filter = 6;

if ($mem_architecture == $ARCH_SDR)
{
	$mem_cas_latency = 3;
}
elsif ($mem_architecture == $ARCH_DDR)
{
	if ($timing_frequency >= 200)
	{
		$mem_cas_latency = 3;
	}
	else
	{
		$mem_cas_latency = 25;
		$mem_dqs_filter = 5;
	}
}
elsif ($mem_architecture == $ARCH_MDDR)
{
	$mem_cas_latency = 3;
}
else
{
	if ($timing_frequency >= 300)
	{
		$mem_cas_latency = 5;
		$mem_dqs_filter = 7;
	}
	else
	{
		$mem_cas_latency = 4;
	}
}

###
# Find clock period
###
my $tCK = (1000.0 / $timing_frequency);

###
# Calculate Memory Timings Parameters
# All the counters have a 3 clock latency and therefore are compensated.
###
$tINIT	= int(($timing_tinit / $tCK) + 0.9999999999) * 1000; # This is the only timimg parameter in us.
$tREFI	= int(($timing_trefi / $tCK) + 0.9999999999);

$tRP 	= int(($timing_trp / $tCK) + 0.9999999999);
$tRP	= ( $tRP > 3 ) ? $tRP - 3 : 0;

$tMRD 	= int(($timing_tmrd / $tCK) + 0.9999999999);
$tMRD	= ( $tMRD > 3 ) ? $tMRD - 3 : 0;

$tRFC	= int(($timing_trfc / $tCK) + 0.9999999999);
$tRFC	= ( $tRFC > 3 ) ? $tRFC - 3 : 0;

$tRCD	= int(($timing_trcd / $tCK) + 0.9999999999);
$tRCD	= ( $tRCD > 3 ) ? $tRCD - 3 : 0;

$tWR	= int(($timing_twr / $tCK) + 0.9999999999);

if ($mem_architecture == $ARCH_DDR2)
{
	$tWR	+= $mem_cas_latency - 5;
}
else
{
	$tWR	-= 3;
}

$tWR	= 0 if ($tWR < 0);

$tRAS	= int(($timing_tras / $tCK) + 0.9999999999);
$tRAS	= ( $tRAS > 3 ) ? $tRAS - 3 : 0;

$tRC	= int(($timing_trc / $tCK) + 0.9999999999);
$tRC	= ( $tRC > 3 ) ? $tRC - 3 : 0;

###
# Find DQS delay
###

$mem_dqs_delay = 0;
$dqs_to_core_delay = 0;
$dq_to_core_delay = 0;

$stx2_dll_frequency = '0';
$stx2_dll_chain_length = '0';
$stx2_dll_buffer_mode = '0';
$stx2_dll_out_mode = '0';

$stx3_d1_delay = 0;
$stx3_d2_delay = 0;
$stx3_d3_delay = 0;
$stx3_d4_delay = 0;

if ($device_family eq 'Cyclone II')
{
	if ($timing_frequency <= 100)
	{
		$mem_dqs_delay = 37;
	}
	elsif ($timing_frequency <= 133)
	{
		$mem_dqs_delay = 22;
	}
	elsif ($timing_frequency <= 167)
	{
		$mem_dqs_delay = 9;
	}
	else
	{
		$mem_dqs_delay = 1;
	}

	$dqs_to_core_delay = 0;
	$dq_to_core_delay = 0;
}

elsif ($device_family eq 'Cyclone III' || $device_family eq 'Cyclone IV E' || $device_family eq 'Cyclone IV GX')
{
	if ($timing_frequency <= 133)
	{
		$dqs_to_core_delay = 5;
	}
	elsif ($timing_frequency <= 167)
	{
		$dqs_to_core_delay = 2;
	}
	elsif ($timing_frequency <= 200)
	{
		$dqs_to_core_delay = 1;
	}
	else
	{
		$dqs_to_core_delay = 0;
	}
}

elsif ($device_family eq 'Stratix II' || $device_family eq 'Arria GX' || $device_family eq 'Arria II GX')
{
	if ($mem_disable_ddr_blocks eq 'false' )
	{
		if ($timing_frequency <= 167)
		{
			$stx2_dll_chain_length = '12';
			$stx2_dll_buffer_mode = 'low';
			$stx2_dll_out_mode = 'delay_chain3';
		}
		elsif ($timing_frequency <= 200)
		{
			$stx2_dll_chain_length = '16';
			$stx2_dll_buffer_mode = 'high';
			$stx2_dll_out_mode = 'delay_chain4';
		}
		elsif ($timing_frequency <= 333)
		{
			$stx2_dll_chain_length = '12';
			$stx2_dll_buffer_mode = 'high';
			$stx2_dll_out_mode = 'delay_chain3';
		}
		my $tCK_ps = int($tCK * 1000.0);
		$stx2_dll_frequency = $tCK_ps."ps";
	}
	else
	{
		if ($sdram_frequency <= 133)
		{
			$dq_to_core_delay = 1;
		}
		elsif ($sdram_frequency <= 167)
		{
			$dq_to_core_delay = 2;
		}
		elsif ($sdram_frequency <= 200)
		{
			$dq_to_core_delay = 3;
		}
		else
		{
			$dq_to_core_delay = 4;
		}
	}

}
elsif ($device_family eq 'Stratix III')
{
	if ($timing_frequency <= 200)
	{
	    ( $stx3_d1_delay, $stx3_d2_delay, $stx3_d3_delay, $stx3_d4_delay ) = (7, 0, 0, 13);
	}
	elsif ($timing_frequency <= 233)
	{
	    ( $stx3_d1_delay, $stx3_d2_delay, $stx3_d3_delay, $stx3_d4_delay ) = (9, 0, 0, 13);
	}
	elsif ($timing_frequency <= 266)
	{
	    ( $stx3_d1_delay, $stx3_d2_delay, $stx3_d3_delay, $stx3_d4_delay ) = (11, 0, 0, 13);
	}
	elsif ($timing_frequency <= 300)
	{
	    ( $stx3_d1_delay, $stx3_d2_delay, $stx3_d3_delay, $stx3_d4_delay ) = (13, 0, 0, 14);
	}
	elsif ($timing_frequency <= 333)
	{
	    ( $stx3_d1_delay, $stx3_d2_delay, $stx3_d3_delay, $stx3_d4_delay ) = (15, 0, 0, 9);
	}
}

###
# SDRAM Settings
###
if ($mem_architecture == $ARCH_SDR)
{
	$mem_oe_latency = 4;
	$mem_cmd_latency = 3; 
	$mem_read_latency = 8;
	$mem_write_data_delay = 0;
	$mem_data_group_width = 9;
}

if ($mem_architecture == $ARCH_DDR)
{
	$mem_oe_latency = 4;
	$mem_read_latency = 7;
	$mem_write_data_delay = 1;
	$mem_data_group_width = 8;

	if ($mem_phase_shift eq 'true')
	{
		$mem_cmd_latency = 2;
	}
	else
	{
		$mem_cmd_latency = 3;
	}

}

elsif ($mem_architecture == $ARCH_DDR2)
{
	$mem_oe_latency = $mem_cas_latency;
	$mem_read_latency = 7;
	$mem_write_data_delay = $mem_cas_latency - 3;
	$mem_data_group_width = 8;

	if ($mem_phase_shift eq 'true')
	{
		$mem_cmd_latency = 0;
	}
	else
	{
		$mem_cmd_latency = 1;
	}

}
elsif ($mem_architecture == $ARCH_MDDR)
{
	$mem_oe_latency = 4;
	$mem_read_latency = 7;
	$mem_write_data_delay = 1;
	$mem_data_group_width = 8;

	if ($mem_phase_shift eq 'true')
	{
		$mem_cmd_latency = 2;
	}
	else
	{
		$mem_cmd_latency = 3;
	}

}

###
# Define the memory address boundaries.
# bank | row | column
###
$mem_column_addr_low 	= 0;
$mem_column_addr_high	= $mem_column_bits - 1;
$mem_row_addr_low 	= $mem_column_addr_high + 1;
$mem_row_addr_high 	= $mem_row_addr_low + $mem_row_bits - 1;
$mem_bank_addr_low 	= $mem_row_addr_high + 1;
$mem_bank_addr_high 	= $mem_bank_addr_low + $mem_bank_bits - 1;

###
# Top level
###
print "\tCreating HDL\n";

my $portnum = $#ports;	# set portnum to be the index of the last port
foreach my $port ( reverse(@ports) ) {
	my $PORT = uc $port;
	my $portmode = $port_mode{$port};

	my $avalon_data_width;
	my $data_width_ratio = 1;

	if ( $mem_architecture == $ARCH_SDR )
	{
		$avalon_data_width = $mem_data_width;
	}
	elsif ($mem_architecture != $ARCH_SDR & $portmode == $MODE_BURST & $mem_data_width == 8 & $port_data_width{$port} == 32 )
	{
		$avalon_data_width = 32;
		$data_width_ratio = 2;
	}
	else
	{
		$avalon_data_width = $mem_data_width * 2;
	}

	my $avalon_words = $num_bits / $avalon_data_width;
	my $avalon_addr_width = int((log($avalon_words)/log(2)) + 0.9999999999);

	my $port_buffer_words = $port_buffer_size{$port};
	my $port_buffer_addr_width = int((log($port_buffer_words)/log(2)) + 0.9999999999);

	my $cache_disable;
	if ( $port_cache_disable{$port} eq 'true' ) {
		$cache_disable = 1;
	} else {
		$cache_disable = 0;
	}

	if ( $portmode == $MODE_RANDOM )
	{
		&add_random_port($PORT, $portnum,
			$avalon_addr_width,
			$avalon_data_width,
			$avalon_data_width / 8,
			$port_buffer_words,
			$port_buffer_addr_width,
			$cache_disable);
	}

	my $avalon_burst_width =  int((log($port_max_burst{$port}+1)/log(2)) + 0.9999999999);
	if ( $portmode == $MODE_BURST )
	{
		&add_burst_port($PORT, $portnum,
			$avalon_addr_width,
			$avalon_data_width,
			$avalon_data_width / 8,
			$port_buffer_words,
			$port_buffer_addr_width,
			$avalon_burst_width,
			0, # burst dataenable
			$data_width_ratio); # data width ratio
	}

	$portnum--;
}

&add_sdram();

my $top = qq
{
--------------------------------------
--   Microtronix SDRAM Controller   --
--             Top Level            --
-- Generated by SOPC Builder script --
-- Build : 169/9.7.2010	    --
--------------------------------------

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
ENTITY $top_level_module_name IS
	PORT
	(
		CLK			: IN std_logic;
$top_port
END ENTITY $top_level_module_name;

-----
-- Arhictecture
-----
ARCHITECTURE top OF $top_level_module_name IS

-- Signals
SIGNAL zero			: std_logic;
SIGNAL one			: std_logic;
$top_signals

BEGIN

zero <= '0';
one  <= '1';

$top_component

	LSP_ADDR        <= $top_lsp_addr;
	LSP_DATA_IN     <= $top_lsp_data_in;
	LSP_DATA_MASK   <= $top_lsp_data_mask;
	LSP_LENGTH      <= $top_lsp_length;
$top_signals_map
END ARCHITECTURE top;
};

open FILE_OUT, ">$filename" or die "cannot create: $!";
print FILE_OUT $top;
close FILE_OUT;

###
# Generate TCL project settings file
###

# Rename current file .bak if it exists
if (-e "$directory/".$top_level_module_name."_assignments.tcl") {
  	rename("$directory/".$top_level_module_name."_assignments.tcl","$directory/".$top_level_module_name."_assignments.tcl.bak") || die $!;
}

open FILE_OUT, ">$directory/".$top_level_module_name."_assignments.tcl" or die "cannot create: $!";
select FILE_OUT;

print "# Microtronix SDRAM Assigments Script\n";
print "# Generated by the Microtronix SDRAM Controller SOPC Builder Perl Script\n";
print "# Build : 169/9.7.2010\n\n";

print "# Open project\n";
print "project_open -current_revision ".$directory.$project_name."\n";

print "\n###\n";
print "# Misc Assignments\n";
print "###\n";
print "set_instance_assignment -name FAST_OUTPUT_REGISTER ON -to ".$mem_prefix."ba\n";
print "set_instance_assignment -name FAST_OUTPUT_REGISTER ON -to ".$mem_prefix."a\n";
print "set_instance_assignment -name FAST_OUTPUT_REGISTER ON -to ".$mem_prefix."cs\n";
print "set_instance_assignment -name FAST_OUTPUT_REGISTER ON -to ".$mem_prefix."cas\n";
print "set_instance_assignment -name FAST_OUTPUT_REGISTER ON -to ".$mem_prefix."ras\n";
print "set_instance_assignment -name FAST_OUTPUT_REGISTER ON -to ".$mem_prefix."we\n";
print "\n";

print "\n###\n";
print "# Architecture-Specific Assignments\n";
print "###\n";

# SDR SDRAM
if ($mem_architecture == $ARCH_SDR)
{
	print "set_instance_assignment -name FAST_OUTPUT_REGISTER ON -to ".$mem_prefix."dqm\n";
	print "set_instance_assignment -name FAST_OUTPUT_REGISTER ON -to ".$mem_prefix."dq\n";
	print "set_instance_assignment -name FAST_INPUT_REGISTER ON -to ".$mem_prefix."dq\n";
	print "set_instance_assignment -name FAST_OUTPUT_ENABLE_REGISTER ON -to ".$mem_prefix."dq\n";
	print "set_instance_assignment -name PAD_TO_INPUT_REGISTER_DELAY 0 -to ".$mem_prefix."dq\n";
	print "set_global_assignment -name POST_FLOW_SCRIPT_FILE quartus_tan:".$top_level_module_name."_pll_settings.tcl\n";
}

# DDR SDRAM
if ($mem_architecture == $ARCH_DDR)
{

	print "set_instance_assignment -name FAST_OUTPUT_ENABLE_REGISTER ON -to ".$mem_prefix."dq\n";
	print "set_instance_assignment -name FAST_OUTPUT_ENABLE_REGISTER ON -to ".$mem_prefix."dqs\n";
	print "set_instance_assignment -name IO_STANDARD \"SSTL-2 CLASS I\" -to ".$mem_prefix."clk_out_p\n";
	print "set_instance_assignment -name IO_STANDARD \"SSTL-2 CLASS I\" -to ".$mem_prefix."clk_out_n\n";
	print "set_instance_assignment -name IO_STANDARD \"SSTL-2 CLASS I\" -to ".$mem_prefix."ba\n";
	print "set_instance_assignment -name IO_STANDARD \"SSTL-2 CLASS I\" -to ".$mem_prefix."a\n";
	print "set_instance_assignment -name IO_STANDARD \"SSTL-2 CLASS I\" -to ".$mem_prefix."cs\n";
	print "set_instance_assignment -name IO_STANDARD \"SSTL-2 CLASS I\" -to ".$mem_prefix."cas\n";
	print "set_instance_assignment -name IO_STANDARD \"SSTL-2 CLASS I\" -to ".$mem_prefix."ras\n";
	print "set_instance_assignment -name IO_STANDARD \"SSTL-2 CLASS I\" -to ".$mem_prefix."we\n";
	print "set_instance_assignment -name IO_STANDARD \"SSTL-2 CLASS I\" -to ".$mem_prefix."dqm\n";
	print "set_instance_assignment -name IO_STANDARD \"SSTL-2 CLASS I\" -to ".$mem_prefix."dq\n";
	print "set_instance_assignment -name IO_STANDARD \"SSTL-2 CLASS I\" -to ".$mem_prefix."dqs\n";
	print "set_instance_assignment -name IO_STANDARD \"SSTL-2 CLASS I\" -to ".$mem_prefix."cke\n";

	print "set_instance_assignment -name OUTPUT_ENABLE_GROUP 22 -to ".$mem_prefix."dq\n";
	print "set_instance_assignment -name OUTPUT_ENABLE_GROUP 22 -to ".$mem_prefix."dqs\n";
	print "set_instance_assignment -name OUTPUT_ENABLE_GROUP 22 -to ".$mem_prefix."dqm\n";

	if ($device_family eq 'Cyclone II')
	{
		print "set_instance_assignment -name PAD_TO_CORE_DELAY 0 -to ".$mem_prefix."dq\n";
		print "set_global_assignment -name POST_FLOW_SCRIPT_FILE quartus_sh:".$top_level_module_name."_ddr_settings.tcl\n";
	}
	elsif ($device_family eq 'Cyclone III' || $device_family eq 'Cyclone IV E' || $device_family eq 'Cyclone IV GX')
	{
		print "set_instance_assignment -name PAD_TO_CORE_DELAY 0 -to ".$mem_prefix."dq\n";
		print "set_instance_assignment -name DUAL_PURPOSE_CLOCK_PIN_DELAY ".$dqs_to_core_delay." -to ".$mem_prefix."dqs\n";
	}
	elsif ($device_family eq 'Stratix II')
	{
		if ($mem_disable_ddr_blocks eq 'true')
		{
			print "\tset_instance_assignment -name PAD_TO_CORE_DELAY ".$dq_to_core_delay." -to ".$mem_prefix."dq\n";
		}
	}
	elsif ($device_family eq 'Arria GX' || $device_family eq 'Arria II GX')
	{
#		print "set_instance_assignment -name PAD_TO_CORE_DELAY 0 -to ".$mem_prefix."dq\n";
	}
	elsif ($device_family eq 'Stratix III')
	{
		print "set_instance_assignment -name D1_DELAY ".$stx3_d1_delay." -to ".$mem_prefix."dq\n";
		print "set_instance_assignment -name D2_DELAY ".$stx3_d2_delay." -to ".$mem_prefix."dq\n";
		print "set_instance_assignment -name D3_DELAY ".$stx3_d3_delay." -to ".$mem_prefix."dq\n";

		if ($mem_disable_ddr_blocks eq 'false')
		{
			print "set_instance_assignment -name D4_DELAY ".$stx3_d4_delay." -to ".$mem_prefix."dqs\n";

			my $i;
			my $j;
			my $k;

			for ($i = 0; $i < $mem_data_width; $i=$i+8)
			{
				$j = $i/8;
				for ($k = $i; $k < $i+8; $k=$k+1)
				{
					print "set_instance_assignment -name DQ_GROUP 9 -from ".$mem_prefix."dqs[$j] -to ".$mem_prefix."dq[$k]\n";
				}
			}
		}
	}


	print "set_global_assignment -name FMAX_REQUIREMENT \"$timing_frequency MHz\" -section_id ".$mem_prefix."dqs_clock\n";
	print "set_instance_assignment -name CLOCK_SETTINGS ".$mem_prefix."dqs_clock -to ".$mem_prefix."dqs\n";
}

# DDR2 SDRAM
if ($mem_architecture == $ARCH_DDR2)
{
	print "set_instance_assignment -name FAST_OUTPUT_ENABLE_REGISTER ON -to ".$mem_prefix."dq\n";
	print "set_instance_assignment -name FAST_OUTPUT_REGISTER ON -to ".$mem_prefix."odt\n";
	print "set_instance_assignment -name IO_STANDARD \"SSTL-18 CLASS I\" -to ".$mem_prefix."clk_out_p\n";
	print "set_instance_assignment -name IO_STANDARD \"SSTL-18 CLASS I\" -to ".$mem_prefix."clk_out_n\n";
	print "set_instance_assignment -name IO_STANDARD \"SSTL-18 CLASS I\" -to ".$mem_prefix."ba\n";
	print "set_instance_assignment -name IO_STANDARD \"SSTL-18 CLASS I\" -to ".$mem_prefix."a\n";
	print "set_instance_assignment -name IO_STANDARD \"SSTL-18 CLASS I\" -to ".$mem_prefix."cs\n";
	print "set_instance_assignment -name IO_STANDARD \"SSTL-18 CLASS I\" -to ".$mem_prefix."cas\n";
	print "set_instance_assignment -name IO_STANDARD \"SSTL-18 CLASS I\" -to ".$mem_prefix."ras\n";
	print "set_instance_assignment -name IO_STANDARD \"SSTL-18 CLASS I\" -to ".$mem_prefix."we\n";
	print "set_instance_assignment -name IO_STANDARD \"SSTL-18 CLASS I\" -to ".$mem_prefix."cke\n";
	print "set_instance_assignment -name IO_STANDARD \"SSTL-18 CLASS I\" -to ".$mem_prefix."odt\n";
	print "set_instance_assignment -name IO_STANDARD \"SSTL-18 CLASS I\" -to ".$mem_prefix."dqm\n";
	print "set_instance_assignment -name IO_STANDARD \"SSTL-18 CLASS I\" -to ".$mem_prefix."dq\n";

	if (($mem_diff_dqs eq 'true') & ($device_family eq 'Stratix III'))
	{
		print "set_instance_assignment -name FAST_OUTPUT_ENABLE_REGISTER ON -to ".$mem_prefix."dqs_p\n";
		print "set_instance_assignment -name FAST_OUTPUT_ENABLE_REGISTER ON -to ".$mem_prefix."dqs_n\n";
		print "set_instance_assignment -name IO_STANDARD \"DIFFERENTIAL 1.8-V SSTL CLASS I\" -to ".$mem_prefix."dqs_p\n";
		print "set_instance_assignment -name IO_STANDARD \"DIFFERENTIAL 1.8-V SSTL CLASS I\" -to ".$mem_prefix."dqs_n\n";
		print "set_instance_assignment -name OUTPUT_ENABLE_GROUP 22 -to ".$mem_prefix."dqs_p\n";
		print "set_instance_assignment -name OUTPUT_ENABLE_GROUP 22 -to ".$mem_prefix."dqs_n\n";
	} else {
		print "set_instance_assignment -name FAST_OUTPUT_ENABLE_REGISTER ON -to ".$mem_prefix."dqs\n";
		print "set_instance_assignment -name IO_STANDARD \"SSTL-18 CLASS I\" -to ".$mem_prefix."dqs\n";
		print "set_instance_assignment -name OUTPUT_ENABLE_GROUP 22 -to ".$mem_prefix."dqs\n";
	}

	print "set_instance_assignment -name OUTPUT_ENABLE_GROUP 22 -to ".$mem_prefix."dq\n";
	print "set_instance_assignment -name OUTPUT_ENABLE_GROUP 22 -to ".$mem_prefix."dqm\n";

	if ($device_family eq 'Cyclone II')
	{
		print "set_instance_assignment -name PAD_TO_CORE_DELAY 0 -to ".$mem_prefix."dq\n";
		print "set_global_assignment -name POST_FLOW_SCRIPT_FILE quartus_sh:".$top_level_module_name."_ddr_settings.tcl\n";
	}
	elsif ($device_family eq 'Cyclone III' || $device_family eq 'Cyclone IV E' || $device_family eq 'Cyclone IV GX')
	{
		print "set_instance_assignment -name PAD_TO_CORE_DELAY 0 -to ".$mem_prefix."dq\n";
		print "set_instance_assignment -name DUAL_PURPOSE_CLOCK_PIN_DELAY ".$dqs_to_core_delay." -to ".$mem_prefix."dqs\n";
	}
	elsif ($device_family eq 'Stratix II')
	{
		if ($mem_disable_ddr_blocks eq 'true')
		{
			print "\tset_instance_assignment -name PAD_TO_CORE_DELAY ".$dq_to_core_delay." -to ".$mem_prefix."dq\n";
		}
	}
	elsif ($device_family eq 'Arria GX' || $device_family eq 'Arria II GX')
	{
#		print "set_instance_assignment -name PAD_TO_CORE_DELAY 0 -to ".$mem_prefix."dq\n";
	}
	elsif ($device_family eq 'Stratix III')
	{
		print "set_instance_assignment -name D1_DELAY ".$stx3_d1_delay." -to ".$mem_prefix."dq\n";
		print "set_instance_assignment -name D2_DELAY ".$stx3_d2_delay." -to ".$mem_prefix."dq\n";
		print "set_instance_assignment -name D3_DELAY ".$stx3_d3_delay." -to ".$mem_prefix."dq\n";

		if ($mem_disable_ddr_blocks eq 'false')
		{
			if ($mem_diff_dqs eq 'true')
			{
				print "set_instance_assignment -name D4_DELAY ".$stx3_d4_delay." -to ".$mem_prefix."dqs_p\n";
			} else {
				print "set_instance_assignment -name D4_DELAY ".$stx3_d4_delay." -to ".$mem_prefix."dqs\n";
			}
			my $i;
			my $j;
			my $k;

			for ($i = 0; $i < $mem_data_width; $i=$i+8)
			{
				$j = $i/8;
				for ($k = $i; $k < $i+8; $k=$k+1)
				{
					if ($mem_diff_dqs eq 'true')
					{
						print "set_instance_assignment -name DQ_GROUP 9 -from ".$mem_prefix."dqs_p[$j] -to ".$mem_prefix."dq[$k]\n";
					} else {
						print "set_instance_assignment -name DQ_GROUP 9 -from ".$mem_prefix."dqs[$j] -to ".$mem_prefix."dq[$k]\n";
					}
				}
			}
		}
	}

	print "set_global_assignment -name FMAX_REQUIREMENT \"$timing_frequency MHz\" -section_id ".$mem_prefix."dqs_clock\n";
	if (($mem_diff_dqs eq 'true') & ($device_family eq 'Stratix III'))
	{
		print "set_instance_assignment -name CLOCK_SETTINGS ".$mem_prefix."dqs_clock -to ".$mem_prefix."dqs_p\n";
	} else {
		print "set_instance_assignment -name CLOCK_SETTINGS ".$mem_prefix."dqs_clock -to ".$mem_prefix."dqs\n";
	}
}

# Mobile DDR SDRAM
if ($mem_architecture == $ARCH_MDDR)
{
	print "set_instance_assignment -name FAST_OUTPUT_ENABLE_REGISTER ON -to ".$mem_prefix."dq\n";
	print "set_instance_assignment -name FAST_OUTPUT_ENABLE_REGISTER ON -to ".$mem_prefix."dqs\n";
	print "set_instance_assignment -name IO_STANDARD \"1.8 V\" -to ".$mem_prefix."clk_out_p\n";
	print "set_instance_assignment -name IO_STANDARD \"1.8 V\" -to ".$mem_prefix."clk_out_n\n";
	print "set_instance_assignment -name IO_STANDARD \"1.8 V\" -to ".$mem_prefix."ba\n";
	print "set_instance_assignment -name IO_STANDARD \"1.8 V\" -to ".$mem_prefix."a\n";
	print "set_instance_assignment -name IO_STANDARD \"1.8 V\" -to ".$mem_prefix."cs\n";
	print "set_instance_assignment -name IO_STANDARD \"1.8 V\" -to ".$mem_prefix."cas\n";
	print "set_instance_assignment -name IO_STANDARD \"1.8 V\" -to ".$mem_prefix."ras\n";
	print "set_instance_assignment -name IO_STANDARD \"1.8 V\" -to ".$mem_prefix."we\n";
	print "set_instance_assignment -name IO_STANDARD \"1.8 V\" -to ".$mem_prefix."dqm\n";
	print "set_instance_assignment -name IO_STANDARD \"1.8 V\" -to ".$mem_prefix."dq\n";
	print "set_instance_assignment -name IO_STANDARD \"1.8 V\" -to ".$mem_prefix."dqs\n";
	print "set_instance_assignment -name IO_STANDARD \"1.8 V\" -to ".$mem_prefix."cke\n";

	print "set_instance_assignment -name OUTPUT_ENABLE_GROUP 22 -to ".$mem_prefix."dq\n";
	print "set_instance_assignment -name OUTPUT_ENABLE_GROUP 22 -to ".$mem_prefix."dqs\n";
	print "set_instance_assignment -name OUTPUT_ENABLE_GROUP 22 -to ".$mem_prefix."dqm\n";

	if ($device_family eq 'Cyclone II')
	{
		print "set_instance_assignment -name PAD_TO_CORE_DELAY 0 -to ".$mem_prefix."dq\n";
		print "set_global_assignment -name POST_FLOW_SCRIPT_FILE quartus_sh:".$top_level_module_name."_ddr_settings.tcl\n";
	}
	elsif ($device_family eq 'Cyclone III' || $device_family eq 'Cyclone IV E' || $device_family eq 'Cyclone IV GX')
	{
		print "set_instance_assignment -name PAD_TO_CORE_DELAY 0 -to ".$mem_prefix."dq\n";
		print "set_instance_assignment -name DUAL_PURPOSE_CLOCK_PIN_DELAY ".$dqs_to_core_delay." -to ".$mem_prefix."dqs\n";
	}
	elsif ($device_family eq 'Stratix II')
	{
		if ($mem_disable_ddr_blocks eq 'true')
		{
			print "\tset_instance_assignment -name PAD_TO_CORE_DELAY ".$dq_to_core_delay." -to ".$mem_prefix."dq\n";
		}
	}
	elsif ($device_family eq 'Arria GX' || $device_family eq 'Arria II GX')
	{
#		print "set_instance_assignment -name PAD_TO_CORE_DELAY 0 -to ".$mem_prefix."dq\n";
	}
	elsif ($device_family eq 'Stratix III')
	{
		print "set_instance_assignment -name D1_DELAY ".$stx3_d1_delay." -to ".$mem_prefix."dq\n";
		print "set_instance_assignment -name D2_DELAY ".$stx3_d2_delay." -to ".$mem_prefix."dq\n";
		print "set_instance_assignment -name D3_DELAY ".$stx3_d3_delay." -to ".$mem_prefix."dq\n";

		if ($mem_disable_ddr_blocks eq 'false')
		{
			print "set_instance_assignment -name D4_DELAY ".$stx3_d4_delay." -to ".$mem_prefix."dqs\n";

			my $i;
			my $j;
			my $k;

			for ($i = 0; $i < $mem_data_width; $i=$i+8)
			{
				$j = $i/8;
				for ($k = $i; $k < $i+8; $k=$k+1)
				{
					print "set_instance_assignment -name DQ_GROUP 9 -from ".$mem_prefix."dqs[$j] -to ".$mem_prefix."dq[$k]\n";
				}
			}
		}
	}

	print "set_global_assignment -name FMAX_REQUIREMENT \"$timing_frequency MHz\" -section_id ".$mem_prefix."dqs_clock\n";
	print "set_instance_assignment -name CLOCK_SETTINGS ".$mem_prefix."dqs_clock -to ".$mem_prefix."dqs\n";
}

print "\n";
print "# Commit assignments\n";
print "export_assignments\n";
print "# Close project\n";
print "project_close\n";

close FILE_OUT;
select STDOUT;

###
# Generate TCL PLL settings file
###
if ($mem_architecture == $ARCH_SDR)
{
	$timing_pcb_delay = $timing_pcb_delay / 1000;
	my $dq_name = $mem_prefix."dq";
	my $clk_name = $mem_prefix."clk_out";

	open FILE_IN, "<mtx_sdram_pll_settings.tcl" or die "cannot open file: $!";
	open FILE_OUT, ">$directory/".$top_level_module_name."_pll_settings.tcl" or die "cannot create: $!";
	select FILE_OUT;
	while($_ = <FILE_IN>) {

		s/^set name_dq_pin.*/set name_dq_pin "$dq_name"/;
		s/^set name_clk_pin.*/set name_clk_pin "$clk_name"/;

		s/^set clk_period.*/set clk_period\t$tCK/;
		s/^set dq_access.*/set dq_access\t$timing_tac/;
		s/^set dq_hold.*/set dq_hold\t$timing_toh/;
		s/^set trace_delay.*/set trace_delay\t$timing_pcb_delay/;

		print;
	}
	close FILE_IN;
	close FILE_OUT;
	select STDOUT;
}

###
# Generate TCL DDR atoms settings file, needed for Cyclone II
###

my $hierarchy;
$hierarchy = sprintf("%s:the_%s|mtx_sdram_controller:sdram", $top_level_module_name, $top_level_module_name);


## Cyclone II
if (($mem_architecture == $ARCH_DDR | $mem_architecture == $ARCH_DDR2 | $mem_architecture == $ARCH_MDDR) & $device_family eq 'Cyclone II')
{
	my $i;
	my $j;
	my $h;
	my $dqs_index;
	my $dq_index;

	open FILE_IN, "<mtx_sdram_ddr_settings.tcl" or die "cannot open file: $!";

	# Check if file already exists
	if (-e "$directory/".$top_level_module_name."_ddr_settings.tcl")
	{
	  	rename("$directory/".$top_level_module_name."_ddr_settings.tcl","$directory/".$top_level_module_name."_ddr_settings.tcl.bak") || die $!;
	}
	open FILE_OUT, ">$directory/".$top_level_module_name."_ddr_settings.tcl" or die "cannot create: $!";

	select FILE_OUT;
	while( <FILE_IN> ) {
		print;
	}

	print "###\n";
	print "# Open Project\n";
	print "###\n";
	print "\tproject_open [lindex \$quartus(args) 1] -current_revision\n";

	print "###\n";
	print "# Load Package\n";
	print "###\n";
	print "\tload_package report\n";
	print "\tload_report\n";

	print "\n";
	print "###\n";
	print "# DDR Atom locations\n";
	print "###\n";
	print "\n";
	print "set error 0\n";

	# Clock Outputs
	for ($i = 0; $i < $mem_clocks; $i++)
	{


		if ($mem_clocks == 1)
		{
			print "# Clock Out P\n";
			print "\tset pin_name \"".$mem_prefix."clk_out_p\"\n";
		}
		else
		{
			print "# Clock Out P[$i]\n";
			print "\tset pin_name \"".$mem_prefix."clk_out_p[$i]\"\n";
		}
		print "\tset hierarchy_name [find_full_hierarchy \"$hierarchy|mtx_sdram_ddr_output:\\\\gen_ddr_clocks:gen_ddr_clock_pairs:$i:pin_ddr_clk_p\" \$pin_name]\n";
		print "\tif {\$hierarchy_name != -1} {place_cii_clk_cells \$pin_name \$hierarchy_name} else {set error 1}\n";
		print "\n";
		print "\tset_instance_assignment -name ADV_NETLIST_OPT_ALLOWED \"NEVER ALLOW\" -to \$hierarchy_name\n";
		print "\tset_instance_assignment -name PRESERVE_REGISTER ON -to \$hierarchy_name\n";
		print "\tset_instance_assignment -name AUTO_PACKED_REGISTERS_STRATIXII OFF -to \$hierarchy_name\n";
		print "\n";

		if ($mem_clocks == 1)
		{
			print "# Clock Out N\n";
			print "\tset pin_name \"".$mem_prefix."clk_out_n\"\n";
		}
		else
		{
			print "# Clock Out N[$i]\n";
			print "\tset pin_name \"".$mem_prefix."clk_out_n[$i]\"\n";
		}
		print "\tset hierarchy_name [find_full_hierarchy \"$hierarchy|mtx_sdram_ddr_output:\\\\gen_ddr_clocks:gen_ddr_clock_pairs:$i:pin_ddr_clk_n\" \$pin_name]\n";
		print "\tif {\$hierarchy_name != -1} {place_cii_clk_cells \$pin_name \$hierarchy_name} else {set error 1}\n";
		print "\tset_instance_assignment -name ADV_NETLIST_OPT_ALLOWED \"NEVER ALLOW\" -to \$hierarchy_name\n";
		print "\tset_instance_assignment -name PRESERVE_REGISTER ON -to \$hierarchy_name\n";
		print "\tset_instance_assignment -name AUTO_PACKED_REGISTERS_STRATIXII OFF -to \$hierarchy_name\n";
		print "\n";
	}

	# DQS Groups
	$dqs_index = 0;

	for ($i = 0; $i < $mem_devices; $i++)
	{

		# Master DQS Group
		# Assuming 8 databits in a data group
		for ($j = 0; $j < 8; $j++)
		{
			$dq_index = ($i * $mem_data_width/$mem_devices) + $j;
			print "# DQ[$dq_index]\n";
			print "\tset pin_name \"".$mem_prefix."dq[$dq_index]\"\n";
			print "\tset hierarchy_name [find_full_hierarchy \"$hierarchy|mtx_sdram_ddr_bidir:\\\\gen_ddr_datapath:gen_cyclone_datapath:gen_ddr_dqs_groups:$i:gen_ddr_master_dqs\" \$pin_name]\n";
			print "\tif {\$hierarchy_name != -1} {place_cii_dq_cells \$pin_name \$hierarchy_name $j} else {set error 1}\n";
			print "\n";
		}

		print "# DQS[$dqs_index]\n";
		print "\tset pin_name \"".$mem_prefix."dqs[$dqs_index]\"\n";
		print "\tset hierarchy_name [find_full_hierarchy \"$hierarchy|mtx_sdram_ddr_bidir:\\\\gen_ddr_datapath:gen_cyclone_datapath:gen_ddr_dqs_groups:$i:gen_ddr_master_dqs\" \$pin_name]\n";
		print "\tif {\$hierarchy_name != -1} {place_cii_dqs_cells \$pin_name \$hierarchy_name} else {set error 1}\n";
		print "\n";
		print "\tset_instance_assignment -name ADV_NETLIST_OPT_ALLOWED \"NEVER ALLOW\" -to \$hierarchy_name\n";
		print "\tset_instance_assignment -name PRESERVE_REGISTER ON -to \$hierarchy_name\n";
		print "\tset_instance_assignment -name AUTO_PACKED_REGISTERS_STRATIXII OFF -to \$hierarchy_name\n";
		print "\n";
		$dqs_index++;


		# Slave DQS Group
		# Assuming 8 databits in a data group
		for ($h = 1; $h < $mem_dqs; $h++)
		{
			for ($j = 0; $j < 8; $j++)
			{
				$dq_index = ($i * $mem_data_width/$mem_devices) + ($h * 8) + $j;
				print "# DQ[$dq_index]\n";
				print "\tset pin_name \"".$mem_prefix."dq[$dq_index]\"\n";
				print "\tset hierarchy_name [find_full_hierarchy \"$hierarchy|mtx_sdram_ddr_bidir:\\\\gen_ddr_datapath:gen_cyclone_datapath:gen_ddr_dqs_groups:$i:gen_gen_ddr_slave_dqs:$h:gen_ddr_slave_dqs\" \$pin_name]\n";
				print "\tif {\$hierarchy_name != -1} {place_cii_dq_cells \$pin_name \$hierarchy_name $j} else {set error 1}\n";
				print "\n";
			}

			print "# DQS[$dqs_index]\n";
			print "\tset pin_name \"".$mem_prefix."dqs[$dqs_index]\"\n";
			print "\tset hierarchy_name [find_full_hierarchy \"$hierarchy|mtx_sdram_ddr_bidir:\\\\gen_ddr_datapath:gen_cyclone_datapath:gen_ddr_dqs_groups:$i:gen_gen_ddr_slave_dqs:$h:gen_ddr_slave_dqs\" \$pin_name]\n";
			print "\tif {\$hierarchy_name != -1} {place_cii_dqs_cells \$pin_name \$hierarchy_name} else {set error 1}\n";
			print "\n";
			$dqs_index++;

			print "\tset_instance_assignment -name ADV_NETLIST_OPT_ALLOWED \"NEVER ALLOW\" -to \$hierarchy_name\n";
			print "\tset_instance_assignment -name PRESERVE_REGISTER ON -to \$hierarchy_name\n";
			print "\tset_instance_assignment -name AUTO_PACKED_REGISTERS_STRATIXII OFF -to \$hierarchy_name\n";
			print "\n";
		}
	}

	# Data Mask
	for ($i = 0; $i < $mem_data_mask_width; $i++)
	{
		print "# DQM[$i]\n";
		print "\tset pin_name \"".$mem_prefix."dqm[$i]\"\n";
		print "\tset hierarchy_name [find_full_hierarchy \"$hierarchy|mtx_sdram_ddr_output:\\\\gen_ddr_datapath:gen_ddr_data_mask:$i:ddr_datapath_dm\" \$pin_name]\n";
		print "\tif {\$hierarchy_name != -1} {place_cii_dqm_cells \$pin_name \$hierarchy_name} else {set error 1}\n";
		print "\n";
		print "\tset_instance_assignment -name ADV_NETLIST_OPT_ALLOWED \"NEVER ALLOW\" -to \$hierarchy_name\n";
		print "\tset_instance_assignment -name PRESERVE_REGISTER ON -to \$hierarchy_name\n";
		print "\tset_instance_assignment -name AUTO_PACKED_REGISTERS_STRATIXII OFF -to \$hierarchy_name\n";
		print "\n";
	}


	print "# Remove script\n";
	print "\tif {\$error == 0} {\n";
	print "\t\tset_global_assignment -name POST_FLOW_SCRIPT_FILE -remove\n";
	print "\t\tpost_message -type warning \"Successfully assigned the DDR locations. Please re-compile the Quartus II project to apply the DDR assignments.\"\n";
	print "\t}\n";
	print "# Commit assignments\n";
	print "\texport_assignments\n";
	print "# Close project\n";
	print "\tproject_close\n";

	print "# Remove .done file\n";
	print "\tfile delete ".$top_level_module_name."_ddr_settings.done\n";

	close FILE_IN;
	close FILE_OUT;
	select STDOUT;
}

if ( $sdc_enable eq 'true' & $mem_architecture != $ARCH_SDR ) {

	# Rename current file .bak if it exists
	if (-e "$directory/".$top_level_module_name.".sdc") {
	  	rename("$directory/".$top_level_module_name.".sdc","$directory/".$top_level_module_name.".sdc.bak") || die $!;
	}

	open FILE_OUT, ">$directory/".$top_level_module_name.".sdc" or die "cannot create: $!";
	select FILE_OUT;

	print "## Microtronix SDRAM IP Timing Constraint Script\n";
	print "## Avalon Multi-port SDRAM Memory Controller IP Core\n";
	print "## Build : 169/9.7.2010\n\n";

	print "# Defines\n\n";

	print "set board_skew_ck_dqs 0.0\n";
	print "set board_skew_ck_cmd 0.0\n";
	print "set board_skew_dqs_dq 0.0\n\n";

	print "set tCK    $sdc_tck\n";
	print "set tDS    $sdc_tds\n";
	print "set tDH    $sdc_tdh\n";
	print "set tIS    $sdc_tis\n";
	print "set tIH    $sdc_tih\n";
	print "set tDSS   $sdc_tdss\n";
	print "set tDSH   $sdc_tdsh\n";
	print "set tDQSQ  $sdc_tdqsq\n";
	print "set tQHS   $sdc_tqhs\n";
	print "set tDQSCK $sdc_tdqsck\n";
	print "set tDCD   0.0\n";
	print "set tDQSS  $sdc_tdqss\n\n";

	print "set pll_ref_clk $sdc_input\n\n";

	print "set additional_addresscmd_tpd 0\n\n";

	print "set fpga_tREAD_CAPTURE_SETUP_ERROR 0\n";
	print "set fpga_tREAD_CAPTURE_HOLD_ERROR 0\n";
	print "set WR_DQS_DQ_SETUP_ERROR 0\n";
	print "set WR_DQS_DQ_HOLD_ERROR 0\n";
	print "set fpga_tCK_ADDR_CTRL_SETUP_ERROR 0\n";
	print "set fpga_tCK_ADDR_CTRL_HOLD_ERROR 0\n";
	print "set fpga_tDQSS_SETUP_ERROR 0\n";
	print "set fpga_tDSSH_HOLD_ERROR 0\n\n";

	print "proc round_3dp { x } {\n";
	print '    return [expr { round($x * 1000) / 1000.0  } ]'."\n";
	print "}\n";

	print "#######################################################\n";
	print "## Pin Assignments\n\n";

	# Address and command pins
	print "set pins(addrcmd) {";

	for ( my $addr = 0 ; $addr < $mem_row_bits ; $addr++ ) {
		print " ".$mem_prefix."a[$addr]";
		if ( (($addr + 1) % 8) == 0 & ($addr + 1) != $mem_row_bits) {
			print " \\\n                   ";
		}
	}
	print " \\\n                   ";

	for ( my $bank = 0; $bank < $mem_bank_bits ; $bank++ ) {
		print " ".$mem_prefix."ba[$bank]";
	}
	print " \\\n                   ";

	print " ".$mem_prefix."cs"." ".$mem_prefix."cas"." ".$mem_prefix."ras"." ".$mem_prefix."we"." ".$mem_prefix."cke";

	if ( $mem_architecture == $ARCH_DDR2 ) {
		print " ".$mem_prefix."odt }\n\n";
	} else {
		print " }\n\n";
	}

	# Data mask pins
	print "set pins(dqm)     {";
	for ( my $mask = 0; $mask < $mem_data_mask_width; $mask++ ) {
		print " ".$mem_prefix."dqm[$mask]";
	}
	print " }\n\n";
	
	# Clock pins
	my $clock_out_p_grp;
	my $clock_out_n_grp;
	if ( $mem_clocks == 1 ) {
		$clock_out_p_grp = $mem_prefix.'clk_out_p ';
		$clock_out_n_grp = $mem_prefix.'clk_out_n ';
	} else {
		for ( my $clock_pair = 0; $clock_pair < $mem_clocks; $clock_pair++ ) {
			$clock_out_p_grp .= $mem_prefix.'clk_out_p['.$clock_pair.'] ';
			$clock_out_n_grp .= $mem_prefix.'clk_out_n['.$clock_pair.'] ';
		}
	}

	print "set pins(ck_p)    { $clock_out_p_grp}\n";
	print "set pins(ck_n)    { $clock_out_n_grp}\n\n";

	# DQS Groups
	for ( my $dqs = 0; $dqs < $mem_dqs * $mem_devices; $dqs++ ) {
		my $pins_per_dqs = $mem_data_width / ( $mem_devices * $mem_dqs );

		if ( $mem_diff_dqs eq 'true' ) {
			print 'set dqgroup_'.$dqs.'   "{'.$mem_prefix.'dqs_p['.$dqs.']} {';
		} else {
			print 'set dqgroup_'.$dqs.'   "{'.$mem_prefix.'dqs['.$dqs.']} {';
		}
		
		my $min_pin_num = $dqs * $pins_per_dqs;
		foreach my $pin_num ( $min_pin_num .. $min_pin_num + $pins_per_dqs - 1 ) {
			print ' '.$mem_prefix.'dq['.$pin_num.']';
		}
		print ' }"'."\n";
	}
	print "\n";

	print "set pins(dqgroup) [list";
	for ( my $dqs = 0; $dqs < $mem_dqs * $mem_devices; $dqs++ ) {
		print ' $dqgroup_'.$dqs;
	}
	print "]\n\n";


	# Clock creation
	print "#######################################################\n";
	print "# Create Clocks\n\n";

	print "# Create all master source clocks in your design.\n";

	print 'create_clock -name sdram_base_clock -period $pll_ref_clk [get_ports your_clock_pin]'."\n\n";

	print "# Derive PLL clocks\n";
	print "derive_pll_clocks\n\n";

	print "#######################################################\n\n";

	print "# The clock paths will be different depending on the FPGA and the PLL's used in your desgin.\n";
	print "# In the TimeQuest console window, type 'derive_pll_clocks' to obtain the clock\n";
	print "# paths generated by pll.\n";
	print "# For eg, In Cyclone III architecture the Clock path is:\n";
	print "# {inst1|altpll_component|auto_generated|pll1|clk[0]}\n";
	print "# where inst1 is the instance name of one of the PLLs in the design\n";
	print "# altpll_component|auto_generated|pll1 is the string path used in Cyclone III, this\n";
	print "# will be different if using Stratix II or III.\n";
	print "# clk[0] is the first output clock of the PLL.\n";
	print "# For e.g the Stratix II PLL output clock path is {{inst|altpll_component|pll1|clk[0]}}\n\n";

	print "set system_clk {inst1|altpll_component|auto_generated|pll1|clk[0]}\n";
	print "set sdram_clk {inst2|altpll_component|auto_generated|pll1|clk[0]}\n";
	print "set sdram_write_clk {inst2|altpll_component|auto_generated|pll1|clk[1]}\n";

	print "#######################################################\n\n";

	print 'set_false_path -from [get_clocks $system_clk] -to [get_clocks $sdram_clk]'."\n";
	print 'set_false_path -from [get_clocks $sdram_clk] -to  [get_clocks $system_clk]'."\n\n";

	print 'set_false_path -from [get_clocks $system_clk] -to [get_clocks $sdram_write_clk]'."\n";
	print 'set_false_path -from [get_clocks $sdram_write_clk] -to  [get_clocks $system_clk]'."\n\n";

	print "# If additional port clocks are used in your design, add the timing constraints for those clocks\n";
	print "# in this script and set the necessary false paths.  E.g:\n\n";

	print '#set_false_path -from port_clk -to [get_clocks $sdram_clk]'."\n";
	print '#set_false_path -from [get_clocks $sdram_clk] -to  port_clk'."\n";
	print "#\n";
	print '#set_false_path -from port_clk -to [get_clocks $sdram_write_clk]'."\n";
	print '#set_false_path -from [get_clocks $sdram_write_clk] -to  port_clk'."\n\n";

	print "#######################################################\n\n";

	if ( $mem_diff_dqs eq 'true' ) {
		print "set_false_path -from * -to ".$mem_prefix."dqs_n*\n";
		print "set_false_path -from ".$mem_prefix."dqs_n* -to *\n\n";
	}

	print "# Output clocks\n\n";

	print 'foreach clockname $pins(ck_p) {'."\n";
	print '    create_generated_clock -add -multiply_by 1 -source $sdram_clk -name $clockname $clockname'."\n";
	print '    set_false_path -from * -to [get_ports $clockname]'."\n";
	print '    set_false_path -from  $sdram_write_clk -to [get_clocks $clockname]'."\n";
	print "}\n\n";

	print 'foreach clockname $pins(ck_n) {'."\n";
	print '    create_generated_clock -add -multiply_by 1 -invert -source $sdram_clk -name $clockname $clockname'."\n";
	print '    set_false_path -from * -to [get_ports $clockname]'."\n";
	print '    set_false_path -from $sdram_write_clk -to [get_clocks $clockname]'."\n";
	print "}\n\n";

	print "#######################################################\n\n";

	print "# Output path\n";
	print "set i 0\n";
	print 'foreach dqgroup $pins(dqgroup) {'."\n\n";

	print '    set dqspin [lindex $dqgroup 0]'."\n\n";

	print '    foreach ck $pins(ck_p) {'."\n";
	print '        set_output_delay -min [round_3dp [expr {$tCK * $tDQSS - $board_skew_ck_dqs}]] -clock $ck [get_ports $dqspin] -add_delay'."\n";
	print '        set_output_delay -max [round_3dp [expr {$tCK - $tCK * $tDQSS + $board_skew_ck_dqs}]] -clock $ck [get_ports $dqspin] -add_delay'."\n";
	print "    }\n\n";
  
	print '    foreach ckn $pins(ck_n) {'."\n";
	print '        set_output_delay -min [round_3dp [expr {$tCK * $tDQSS - $board_skew_ck_dqs}]] -clock $ckn [get_ports $dqspin] -add_delay -clock_fall'."\n";
	print '        set_output_delay -max [round_3dp [expr {$tCK - $tCK * $tDQSS + $board_skew_ck_dqs}]] -clock $ckn [get_ports $dqspin] -add_delay -clock_fall'."\n";
	print "    }\n\n";

	print '    create_generated_clock -add -source $sdram_clk -multiply_by 1 -name ${dqspin}_out [get_ports $dqspin]'."\n\n";

	print '    set_false_path -fall_from  $sdram_clk -to [get_ports $dqspin]'."\n\n";

	print '    set_output_delay  -clock ${dqspin}_out -max [round_3dp [expr {$board_skew_dqs_dq + $tDS + $WR_DQS_DQ_SETUP_ERROR}]] [lindex $dqgroup 1]'."\n";
	print '    set_output_delay  -clock ${dqspin}_out -min [round_3dp [expr {-$tDH - $board_skew_dqs_dq - $tDCD - $WR_DQS_DQ_HOLD_ERROR}]] [lindex $dqgroup 1]'."\n";
  
	print '    set_output_delay -add_delay -clock_fall -clock ${dqspin}_out -max [round_3dp [expr {$board_skew_dqs_dq + $tDS + $WR_DQS_DQ_SETUP_ERROR}]] [lindex $dqgroup 1]'."\n";
	print '    set_output_delay -add_delay -clock_fall -clock ${dqspin}_out -min [round_3dp [expr {-$tDH - $board_skew_dqs_dq - $tDCD - $WR_DQS_DQ_HOLD_ERROR}]] [lindex $dqgroup 1]'."\n\n";

	print '    set_output_delay -add_delay  -clock ${dqspin}_out -max [round_3dp [expr {$board_skew_dqs_dq + $tDS + $WR_DQS_DQ_SETUP_ERROR}]] [lindex $pins(dqm) $i]'."\n";
	print '    set_output_delay -add_delay  -clock ${dqspin}_out -min [round_3dp [expr {-$tDH - $board_skew_dqs_dq - $tDCD - $WR_DQS_DQ_HOLD_ERROR}]] [lindex $pins(dqm) $i]'."\n\n";

	print '    set_output_delay -add_delay -clock_fall -clock ${dqspin}_out -max [round_3dp [expr {$board_skew_dqs_dq + $tDS + $WR_DQS_DQ_SETUP_ERROR}]] [lindex $pins(dqm) $i]'."\n";
	print '    set_output_delay -add_delay -clock_fall -clock ${dqspin}_out -min [round_3dp [expr {-$tDH - $board_skew_dqs_dq - $tDCD - $WR_DQS_DQ_HOLD_ERROR}]] [lindex $pins(dqm) $i]'."\n\n";

	print '    set_false_path -from [get_clocks ${dqspin}_out] -to [get_clocks $system_clk]'."\n";
	print '    set_false_path -from [get_clocks $system_clk]   -to [get_clocks ${dqspin}_out]'."\n\n";

	print '    set_false_path -setup -rise_from [get_clocks ${dqspin}_out] -fall_to [get_clocks $sdram_write_clk]'."\n";
	print '    set_false_path -setup -fall_from [get_clocks ${dqspin}_out] -rise_to [get_clocks $sdram_write_clk]'."\n";
	print '    set_false_path -setup -rise_from [get_clocks $sdram_write_clk] -fall_to [get_clocks ${dqspin}_out]'."\n";
	print '    set_false_path -setup -fall_from [get_clocks $sdram_write_clk] -rise_to [get_clocks ${dqspin}_out]'."\n\n";

	print '    set_false_path -hold -rise_from [get_clocks ${dqspin}_out] -fall_to [get_clocks $sdram_write_clk]'."\n";
	print '    set_false_path -hold -fall_from [get_clocks ${dqspin}_out] -rise_to [get_clocks $sdram_write_clk]'."\n";
	print '    set_false_path -hold -rise_from [get_clocks $sdram_write_clk] -fall_to [get_clocks ${dqspin}_out]'."\n";
	print '    set_false_path -hold -fall_from [get_clocks $sdram_write_clk] -rise_to [get_clocks ${dqspin}_out]'."\n\n";
  
	print "    # Add necessary false paths for additional port clocks in your design\n\n";

	print '#    set_false_path -from [get_clocks ${dqspin}_out] -to port_clk'."\n";
	print '#    set_false_path -from port_clk -to [get_clocks ${dqspin}_out]'."\n\n";

	print '    set_false_path -to [get_keepers {*|mtx_sdram_ddr_bidir:*|dq_reg_in*}] -from ${dqspin}_out'."\n";
	print '    set_false_path -to [get_keepers {*|mtx_sdram_controller:*|sdram_data_from_sdram*}] -from ${dqspin}_out'."\n\n";

	print '    set_false_path -from [get_keepers {*|mtx_sdram_controller:*|sdram_lsp_burst_counter_reset}] -fall_to ${dqspin}_out'."\n";
	print '    set_false_path -from [get_keepers {*|mtx_sdram_controller:*|sdram_lsp_burst_counter_reset}] -rise_to ${dqspin}_out'."\n\n";

	print '    set_false_path -from [get_keepers {*|mtx_sdram_controller:*|sdram_reset}] -fall_to ${dqspin}_out'."\n";
	print '    set_false_path -from [get_keepers {*|mtx_sdram_controller:*|sdram_reset}] -rise_to ${dqspin}_out'."\n\n";

	print '    set i [expr {$i + 1}]'."\n";
	print "}\n";

	print 'set_false_path -from [get_keepers {*|mtx_sdram_controller:*|sdram_reset}] -to [get_keepers {*|mtx_sdram_ddr_bidir:*|dq_reg_in_*}]'."\n";
	print 'set_false_path -from [get_keepers {*|mtx_sdram_controller:*|sdram_reset}] -to [get_keepers {*|mtx_sdram_ddr_bidir:*|dqs_clk_active}]'."\n\n";

	print "#######################################################\n\n";
	print "# Input path\n";
	print 'foreach dqgroup $pins(dqgroup) {'."\n\n";

	print '    set dqspin [lindex $dqgroup 0]'."\n\n";

	print '    create_clock -period $tCK -name ${dqspin}_in $dqspin -add'."\n";
	print '    create_clock -add -period $tCK -name v_${dqspin}_in'."\n\n";

	print '    set_input_delay -add_delay -clock v_${dqspin}_in -max [round_3dp [expr {$tDQSQ + $board_skew_dqs_dq}]] [lindex $dqgroup 1]'."\n";
	print '    set_input_delay -add_delay -clock v_${dqspin}_in -min [round_3dp [expr {-$tQHS - $board_skew_dqs_dq}]] [lindex $dqgroup 1]'."\n\n";

	print '    set_input_delay -add_delay -clock_fall -clock v_${dqspin}_in -max [round_3dp [expr {$tDQSQ + $board_skew_dqs_dq}]] [lindex $dqgroup 1]'."\n";
	print '    set_input_delay -add_delay -clock_fall -clock v_${dqspin}_in -min [round_3dp [expr {-$tQHS - $board_skew_dqs_dq}]] [lindex $dqgroup 1]'."\n\n";

	print '    set_false_path -from [get_clocks ${dqspin}_in] -to [get_clocks $system_clk]'."\n";
	print '    set_false_path -from [get_clocks $system_clk]  -to [get_clocks ${dqspin}_in]'."\n";

	print "    # Add necessary false paths for additional port clocks in your design\n";

	print "#    set_false_path -from [get_clocks ${dqspin}_in] -to port_clk\n";
	print "#    set_false_path -from port_clk -to [get_clocks ${dqspin}_in]\n\n";

	print '    set_false_path -from [get_keepers {*|mtx_sdram_controller:*|sdram_lsp_burst_counter_reset}] -fall_to ${dqspin}_in'."\n";
	print '    set_false_path -from [get_keepers {*|mtx_sdram_controller:*|sdram_lsp_burst_counter_reset}] -rise_to ${dqspin}_in'."\n\n";

	print '    set_false_path -from [get_keepers {*|mtx_sdram_controller:*|sdram_reset}] -fall_to ${dqspin}_in'."\n";
	print '    set_false_path -from [get_keepers {*|mtx_sdram_controller:*|sdram_reset}] -rise_to ${dqspin}_in'."\n\n";

	if ( $random_ports > 0 ) {
		print '    set_false_path -from [get_keepers {*|mtx_sdram_avalon_random:*|sdram_cache_select}] -to ${dqspin}_in'."\n";
		print '    set_false_path -from [get_keepers {*|mtx_sdram_avalon_random:*|sdram_reset*}] -to ${dqspin}_in'."\n";
	}
	if ( $burst_ports > 0 ) {
		print '    set_false_path -from [get_keepers {*|mtx_sdram_avalon_burst:*|sdram_reset*}] -to ${dqspin}_in'."\n";
	}
	print "\n}\n\n";

	print '# Set false paths for DDR same edge capture, must be done after all clocks are created'."\n";
	print 'foreach dqgroup $pins(dqgroup) {'."\n\n";

	print '    set dqspin [lindex $dqgroup 0]'."\n\n";

	print '    # On same edge capture DDR interface the paths from RISE to FALL and'."\n";
	print '    # from FALL to RISE are not valid for setup analysis'."\n";
	print '    set_false_path -setup -rise_from [get_clocks v_*_in] -fall_to [get_clocks ${dqspin}_in]'."\n";
	print '    set_false_path -setup -fall_from [get_clocks v_*_in] -rise_to [get_clocks ${dqspin}_in]'."\n\n";
  
	print '    # On same edge capture DDR interface the paths from RISE to RISE and'."\n";
	print '    # FALL to FALL are not valid for hold analysis'."\n";
	print '    set_false_path -hold -rise_from [get_clocks v_*_in] -rise_to [get_clocks ${dqspin}_in]'."\n";
	print '    set_false_path -hold -fall_from [get_clocks v_*_in] -fall_to [get_clocks ${dqspin}_in]'."\n\n";

	print "\n}\n\n";

	print "# exclusive grouping for input and output\n";
	if ( $mem_diff_dqs eq 'true' ) {
		print 'set_clock_groups -exclusive -group [get_clocks {*'.$mem_prefix.'dqs_p[*]_in}] -group [get_clocks {*'.$mem_prefix.'dqs_p[*]_out}]'."\n\n";
	} else {
		print 'set_clock_groups -exclusive -group [get_clocks {*'.$mem_prefix.'dqs[*]_in}] -group [get_clocks {*'.$mem_prefix.'dqs[*]_out}]'."\n\n";
	}

	print "#######################################################\n";

	print "# Address Command\n";
	print 'foreach ckclock_p $pins(ck_p) {'."\n";
	print '    set_output_delay -add_delay -clock $ckclock_p -max [round_3dp [expr { $tIS + $board_skew_ck_cmd + $tDCD + $additional_addresscmd_tpd + $fpga_tCK_ADDR_CTRL_SETUP_ERROR}]] $pins(addrcmd)'."\n";
	print '    set_output_delay -add_delay -clock $ckclock_p -min [round_3dp [expr {-$tIH - $board_skew_ck_cmd - $tDCD + $additional_addresscmd_tpd - $fpga_tCK_ADDR_CTRL_HOLD_ERROR}]]  $pins(addrcmd)'."\n";
	print "}\n\n";

	print 'foreach ckclock_n $pins(ck_n) {'."\n";
	print '    set_output_delay -clock_fall -add_delay -clock $ckclock_n -max [round_3dp [expr { $tIS + $board_skew_ck_cmd + $tDCD + $additional_addresscmd_tpd + $fpga_tCK_ADDR_CTRL_SETUP_ERROR}]] $pins(addrcmd)'."\n";
	print '    set_output_delay -clock_fall -add_delay -clock $ckclock_n -min [round_3dp [expr {-$tIH - $board_skew_ck_cmd - $tDCD + $additional_addresscmd_tpd - $fpga_tCK_ADDR_CTRL_HOLD_ERROR}]]  $pins(addrcmd)'."\n";
	print "}\n\n";

	if ( $burst_ports > 0 ) {
		print "#######################################################\n\n";

		print "set_false_path -from [get_keepers {*|mtx_sdram_avalon_burst:*|sdram_in_fifo_write_pointer*}] -to [get_keepers {*|mtx_sdram_avalon_burst:*|av_sdram_fifo_write_pointer_gray_sync*}]\n";
		print "set_false_path -from [get_keepers {*|mtx_sdram_avalon_burst:*|sdram_in_fifo_write_pointer*}] -to [get_keepers {*|mtx_sdram_avalon_burst:*|sdram_fifo_write_pointer_gray_sync*}]\n";
		print "set_false_path -from [get_keepers {*|mtx_sdram_avalon_burst:*|sdram_reset}] -to [get_keepers {*|mtx_sdram_avalon_burst:*|sdram_in_fifo_write_pointer*}]\n";
		print "set_false_path -from [get_keepers {*|mtx_sdram_avalon_burst:*|sdram_reset}] -to [get_keepers {*|mtx_sdram_avalon_burst:*|sdram_lsp_write_delayed*}]\n\n";
	}

	print "#######################################################\n\n";

	print "set_false_path -from [get_keepers {*|mtx_sdram_controller:*|sdram_suppress_first_write*}] -to [get_keepers {*|mtx_sdram_controller:*|lsp_sdram_suppress_first_write*}]\n";
	print "set_false_path -from [get_keepers {*|mtx_sdram_controller:*|dqs_lsp_end_set[*]}] -to [get_keepers {*|mtx_sdram_controller:*|sdram_dqs_lsp_end_set[*]}]\n";
	print "set_false_path -from [get_keepers {*|mtx_sdram_controller:*|lsp_ddr_burst_counter_run[*]}] -to [get_keepers {*|mtx_sdram_controller:*|sdram_lsp_ddr_burst_counter_run_sync}]\n";
	print "set_false_path -from [get_keepers {*|mtx_sdram_controller:*|sdram_burst_counter[*]}] -to [get_keepers {*|mtx_sdram_controller:*|dqs_sdram_burst_completed[*]}]\n";
	print "set_false_path -from [get_keepers {*|mtx_sdram_controller:*|sdram_column_words[*]}] -to [get_keepers {*|mtx_sdram_controller:*|lsp_ddr_burst_counter[*][*]}]\n";
	print "set_false_path -from [get_keepers {*|mtx_sdram_controller:*|mtx_sdram_controller_scheduler:scheduler|sdram_select[*]}] -to [get_keepers {*|mtx_sdram_controller:*|lsp_sel[*][*]}]\n";
	print "set_false_path -from [get_keepers {*|mtx_sdram_controller:*|lsp_ddr_burst_counter_run[*]}] -to [get_keepers {*|mtx_sdram_controller:*|sdram_lsp_ddr_burst_counter_run_sync}]\n";
	print 'set_false_path -from [get_keepers {*|mtx_sdram_controller:*|sdram_reset}] -fall_to [get_clocks $sdram_clk]'."\n";
	print 'set_false_path -from [get_keepers {*|mtx_sdram_controller:*|sdram_reset}] -to [get_clocks $sdram_write_clk]'."\n\n";

	print "#######################################################\n\n";

	print "derive_clock_uncertainty\n";

	close FILE_OUT;
	select STDOUT;
}

