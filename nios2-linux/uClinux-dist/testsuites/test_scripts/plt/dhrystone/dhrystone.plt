set terminal png large size 1200,2400
set output "dhrystone.png"

set multiplot

set xdata time          # The x axis data is time
set timefmt "%Y_%b_%d"  # The dates in the file look like 2010_Jan_31
set xlabel "Date\n"
set format x "%b %d"    # On the x-axis, we want tics like Jan 31
set xtics rotate
set grid
set key below nobox
set style data linespoints
set pointsize 0.5

set size 1,0.33
set origin 0,0.66
set ylabel "Dhrystones"
#set yrange [300000:]
set title "Dhrystones per Second"
plot 'dhrystone_data_1' using 1:2 notitle, \
         for [i=3:81] '' using 1:i notitle


set size 1,0.33
set origin 0,0.33
set ylabel "Dhrystones"
#set yrange [150:]
set title "Dhrystone MIPS"
plot 'dhrystone_data_2' using 1:2 notitle, \
         for [i=3:81] '' using 1:i notitle


set size 1,0.33
set origin 0,0
set ylabel "Dhrystones"
#set yrange [0:]
set title "DMIPS/MHz"
plot 'dhrystone_data_3' using 1:2 title columnheader(2), \
         for [i=3:81] '' using 1:i title columnheader(i)


unset multiplot
