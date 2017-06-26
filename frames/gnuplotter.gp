#! /usr/bin/gnuplot

set title title

set xrange [0:5000]

set terminal jpeg
set output out


set style line 1 lc rgb "blue" lt 1 lw 1 pt 7 ps 0.5
set style line 2 lc rgb "grey" lt 1 lw 1 pt 7 ps 0.5


plot in index 0 title "E" with impulses ls 1

