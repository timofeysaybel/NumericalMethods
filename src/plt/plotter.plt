set xrange [0:10]
set xlabel "time"
set logscale y
plot "plt/res.dat" u 1:2 w l title "Зависимость нормы решения от времени", "plt/res.dat" u 1:3 w l title "e(λt)", "plt/res.dat" u 1:4 w l title "Зависимость нормы мод. решения от времени"
pause mouse any