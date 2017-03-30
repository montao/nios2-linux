set terminal png large size 1000,800
set output "nbench.png"

set multiplot

set xdata time          # The x axis data is time
set timefmt "%Y_%b_%d"  # The dates in the file look like 2010_Jan_31
set format x "%b %d"    # On the x-axis, we want tics like Jan 31
set grid
set xlabel "Date"
set ylabel "Iterations/sec."
set xtics rotate
set pointsize 0.5

set size 0.5,0.5
set origin 0,0.5
set title "nbench-1"
plot "nbench_data" using 1:2 with linespoints pointtype 4 lt 1 title "Numeric Sort", \
         "nbench_data" using 1:6 with linespoints pointtype 4 lt 2 title "Fourier", \
         "nbench_data" using 1:8 with linespoints pointtype 4 lt 3 title "IDEA", \
         "nbench_data" using 1:9 with linespoints pointtype 4 lt 4 title "Huffman"

set size 0.5,0.5
set origin 0,0
set title "nbench-2"
plot "nbench_data" using 1:4 with linespoints pointtype 4 lt 5 title "Bitfield"

set size 0.5,0.5
set origin 0.5,0.5
set title "nbench-3"
plot "nbench_data" using 1:3 with linespoints pointtype 4 lt 6 title "String Sort", \
         "nbench_data" using 1:5 with linespoints pointtype 4 lt 7 title "FP Emulation", \
         "nbench_data" using 1:7 with linespoints pointtype 4 lt 8 title "Assignment", \
         "nbench_data" using 1:11 with linespoints pointtype 4 lt 9 title "LU Decomposition"

set size 0.5,0.5
set origin 0.5,0
set yrange [0:]
set title "nbench-4"
plot "nbench_data" using 1:10 with linespoints pointtype 4 lt 10 title "Neural Net"


unset multiplot
