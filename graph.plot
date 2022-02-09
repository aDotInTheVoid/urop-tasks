set terminal pdf
set output "__tmp.pdf"

set boxwidth 1
set style fill solid
set xlabel "Number of instructions in a basic block"
set yrange [0:*]
set xrange [1:*]
set ylabel "Frequency"
set title filename

plot "hist.dat" using 1:2 with boxes notitle
