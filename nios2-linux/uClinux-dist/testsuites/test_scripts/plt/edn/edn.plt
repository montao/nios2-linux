set terminal png large size 600,800
set output "edn.png"

set multiplot

set xdata time          # The x axis data is time
set timefmt "%Y_%b_%d"  # The dates in the file look like 2010_Jan_31
set format x "%b %d"    # On the x-axis, we want tics like Jan 31

set grid
set xlabel "Date"
set ylabel "Cycles per Loop"

set xtics rotate

set style data linespoints
set pointsize 0.5

set size 1,0.5
set origin 0,0.5
set title "edn-1"
plot "edn_data" using 1:2 pt 5 title "overhead", \
         "edn_data" using 1:3 pt 5 title "vec_mpy1", \
         "edn_data" using 1:4 pt 5 title "mac", \
         "edn_data" using 1:7 pt 5 title "latsynth", \
         "edn_data" using 1:8 pt 5 title "iir1", \
         "edn_data" using 1:9 pt 5 title "codebook"

set size 1,0.5
set origin 0,0
set title "edn-2"
plot "edn_data" using 1:5 pt 5 title "fir", \
         "edn_data" using 1:6 pt 5 title "fir_no_red_ld", \
         "edn_data" using 1:10 pt 5 title "jpegdct"

unset multiplot
