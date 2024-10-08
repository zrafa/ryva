
set title "Trayectoria Inercial (Plano X-Y)"
set xlabel "Posición X (m)"
set ylabel "Posición Y (m)"
set grid
set size ratio -1
plot "trayectoria_xy.dat" with lines title "Trayectoria X-Y"
pause -1
