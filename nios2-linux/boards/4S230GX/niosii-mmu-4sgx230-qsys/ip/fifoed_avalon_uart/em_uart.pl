use strict;
use mk_em_uart;
my $project = e_project->new(@ARGV);
mk_em_uart::make($project,$project->WSA());