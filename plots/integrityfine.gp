set terminal postscript eps enhanced colour size 6, 4.8 font ",24"
set output "integrityfine.eps"

set style line 1 lc rgb '#A00000' pt 1 ps 2 dt 1 lw 3 # --- red
set style line 2 lc rgb '#5060D0' pt 2 ps 1 dt 1 lw 3 # --- blue
set style line 3 lc rgb '#00A000' pt 3 ps 1 dt 1 lw 3 # --- green
set style line 4 lc rgb '#000000' pt 4 ps 1 dt 4 lw 3 # --- blk
set style line 5 lc rgb '#00A0A0' pt 5 ps 1 dt 4 lw 3 # --- green
set style line 6 lc rgb '#A0A000' pt 6 ps 1 dt 4 lw 3 # --- green
set style line 7 lc rgb '#0A00A0' pt 7 ps 1 dt 3 lw 3 # --- green
set style line 8 lc rgb '#777777' pt 8 ps 1 dt 3 lw 3 # --- green
set style line 9 lc rgb '#AA00AA' pt 9 ps 1 dt 3 lw 3 # --- green
set style line 10 lc rgb '#A00000' pt 10 ps 1 dt 1 lw 3 # --- red
set style line 11 lc rgb '#5060D0' pt 11 ps 1 dt 1 lw 3 # --- blue
set style line 12 lc rgb '#777777' pt 26 ps 1 dt 3 lw 3 # --- green
set style line 13 lc rgb '#AA00AA' pt 27 ps 1 dt 3 lw 3  # --- blk
set style line 14 lc rgb '#00A0A0' pt 14 ps 1 dt 4 lw 3 # --- green
set style line 15 lc rgb '#A0A000' pt 15 ps 1 dt 4 lw 3 # --- green
set style line 16 lc rgb '#0A00A0' pt 16 ps 1 dt 3 lw 3 # --- green
set style line 17 lc rgb '#777777' pt 17 ps 1 dt 3 lw 3 # --- green
set style line 18 lc rgb '#AA00AA' pt 18 ps 1 dt 3 lw 3 # --- green
set style line 19 lc rgb '#A00000' pt 19 ps 1 dt 1 lw 3 # --- red
set style line 20 lc rgb '#5060D0' pt 20 ps 1 dt 1 lw 3 # --- blue
set style line 21 lc rgb '#00A000' pt 21 ps 1 dt 5 lw 3 # --- green
set style line 22 lc rgb '#000000' pt 22 ps 1 dt 4 lw 3 # --- blk
set style line 23 lc rgb '#00A0A0' pt 23 ps 1 dt 4 lw 3 # --- green
set style line 24 lc rgb '#A0A000' pt 24 ps 1 dt 4 lw 3 # --- green
set style line 25 lc rgb '#0A00A0' pt 25 ps 1 dt 3 lw 6 # --- green

set style line 11 lc rgb '#000000' lt 1
set border back ls 11
set tics nomirror

set style line 12 lc rgb '#000000' lt 0 lw 1

set style line 13 lc rgb '#A000A0' lt 0 lw 1

set grid back ls 12

set xlabel 'Number of GPEs'
set ylabel 'Slowdown'

set xrange [0:12]

set key above

set yrange [1:12]

set logscale y
     set format y ""
set ytics add ("" 1.05, "1" 1, "1.25" 1.25, "1.5" 1.5, "2" 2, "2.5" 2.5, \
                    "3" 3, "3.5" 3.5, "4" 4, "5" 5, "6" 6, "7" 7, "9" 9, "12" 12)
set key autotitle columnheader above
plot for [i=0:19] 'integrityfine.data' using 1:2 index i with linespoints,\
  1  lc rgb '#444444' linetype 1 title ""
