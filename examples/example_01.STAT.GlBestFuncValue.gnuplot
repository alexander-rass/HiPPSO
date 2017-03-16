set terminal png size 800,600
set output 'example_01.STAT.GlBestFuncValue.png'
set xlabel 'iterations'
set ylabel 'global attractor value'
set grid
set logscale y
plot   'example_01.STAT.GlBestFuncValue.txt' using 1:2 with lines title 'global attractor value'
