	set title "Gráfico: OrbTrajectory"
	set xlabel "x [m]"
	set ylabel "z [m]"

	# set output 'salida1.png'

	# Para poner una cuadrícula
	set grid ytics lc rgb "#bbbbbb" lw 1 lt 0
	set grid xtics lc rgb "#bbbbbb" lw 1 lt 0

# Los archivos a plottear

plot "OrbCameraTrajectory.pc.txt" using 4:12 with lines, "OrbCameraTrajectory.raspberry.txt" using 4:12 with lines, "CameraTrajectory.pc.foja_cero.txt" using 4:12 with lines, "CameraTrajectory.raspberry.foja_cero.txt" using 4:12 with lines


pause 100

