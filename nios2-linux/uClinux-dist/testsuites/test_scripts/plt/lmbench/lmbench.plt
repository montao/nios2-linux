set terminal png large size 600,900
set output "lmbench.png"

set multiplot

set xdata time          # The x axis data is time
set timefmt "%Y_%b_%d"  # The dates in the file look like 2010_Jan_31
set format x "%b %d"    # On the x-axis, we want tics like Jan 31

set grid
set ylabel "Context Switch Time (ms)"
set xtics rotate
set key out

set style data linespoints
set pointsize 0.5

set size 1,0.33
set origin 0,0.68
set title "Lmbench: Process Size of 0 kB"
plot "lmbench_data_size0" using 1:2 pt 5 lt 1 title "2 processes", \
         "lmbench_data_size0" using 1:3 pt 5 lt 2 title "8 processes", \
         "lmbench_data_size0" using 1:4 pt 5 lt 3 title "16 processes"

set size 1,0.33
set origin 0,0.35
set title "Lmbench: Process Size of 16 kB"
plot "lmbench_data_size16" using 1:2 pt 5 lt 4 title "2 processes", \
         "lmbench_data_size16" using 1:3 pt 5 lt 5 title "8 processes", \
         "lmbench_data_size16" using 1:4 pt 5 lt 6 title "16 processes"

set size 1,0.33
set origin 0,0.02
set title "Lmbench: Process Size of 64 kB"
plot "lmbench_data_size64" using 1:2 pt 5 lt 7 title "2 processes", \
         "lmbench_data_size64" using 1:3 pt 5 lt 8 title "8 processes", \
         "lmbench_data_size64" using 1:4 pt 5 lt 9 title "16 processes"


unset multiplot
