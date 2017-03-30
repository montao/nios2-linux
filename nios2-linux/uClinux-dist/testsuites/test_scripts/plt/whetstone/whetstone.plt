set terminal png large size 800,600
set output "whetstone.png"

set timefmt "%Y_%b_%d"  # The dates in the file look like 2010_Jan_31
set xdata time          # The x axis data is time
set format x "%b %d"    # On the x-axis, we want tics like Jan 31

set grid
set xlabel "Date"
set ylabel "MIPS"
set xtics rotate

set key below nobox
set title "C Converted Double Precision Whetstones"

plot 'whetstone_data' using 1:2 title columnheader(2), \
         for [i=3:41] '' using 1:i title columnheader(i)
