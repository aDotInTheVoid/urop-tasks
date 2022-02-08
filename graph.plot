set terminal pdf
set output "__tmp.pdf"

set boxwidth 0.5
set style fill solid
set xlabel "Number of instructions in a basic block"
set yrange [0:*]
set ylabel "Frequency"

plot "hist.dat" using 1:3:xtic(2) with boxes notitle