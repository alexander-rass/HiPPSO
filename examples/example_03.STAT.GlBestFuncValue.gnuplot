set terminal png size 800,600
set output 'example_03.STAT.GlBestFuncValue.png'
set xlabel 'iterations'
set ylabel 'global attractor value'
set grid
set logscale y
set format y "10^{%L}"
plot   'example_03a.STAT.GlBestFuncValue.txt' using 1:2 with lines title 'global attractor value - adjusting precision','example_03b.STAT.GlBestFuncValue.txt' using 1:2 with lines title 'global attractor value - constant precision'
#plot   'example_03b.STAT.GlBestFuncValue.txt' using 1:2 with lines title 'global attractor value'

