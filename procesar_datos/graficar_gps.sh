#!/bin/bash

# Nombre del archivo que contiene las tramas crudas del GPS
input_file="gps_data.txt"

# Nombre del archivo de salida que contendrá las coordenadas en metros
output_file="gps_coordinates_meters.dat"

# Nombre del archivo PNG que se generará
output_image="trayectoria_gps_metros.png"

# Borrar el archivo de salida si existe
> "$output_file"

# Referencia inicial para convertir a metros (el primer punto)
initial_lat=""
initial_lon=""

# Extraer las coordenadas de las tramas GPS y convertirlas a metros
while IFS=',' read -r _ _ _ lat hem_lat lon hem_lon _; do
    # Convertir latitud a decimal
    lat_deg=${lat:0:2}
    lat_min=${lat:2}
    lat_decimal=$(echo "$lat_deg + $lat_min / 60" | bc -l)

    # Aplicar hemisferio
    [[ "$hem_lat" == "S" ]] && lat_decimal=$(echo "-$lat_decimal" | bc)

    # Convertir longitud a decimal
    lon_deg=${lon:0:3}
    lon_min=${lon:3}
    lon_decimal=$(echo "$lon_deg + $lon_min / 60" | bc -l)

    # Aplicar hemisferio
    [[ "$hem_lon" == "W" ]] && lon_decimal=$(echo "-$lon_decimal" | bc)

    # Establecer el punto de referencia inicial
    if [[ -z "$initial_lat" ]]; then
        initial_lat=$lat_decimal
        initial_lon=$lon_decimal
    fi

    # Calcular la distancia en metros desde el punto inicial
    delta_lat=$(echo "$lat_decimal - $initial_lat" | bc -l)
    delta_lon=$(echo "$lon_decimal - $initial_lon" | bc -l)

    # Convertir la distancia a metros (1 grado de latitud = 111320 m; 1 grado de longitud en la latitud 38S ≈ 88440 m)
    meters_y=$(echo "$delta_lat * 111320" | bc -l)
    meters_x=$(echo "$delta_lon * 88440" | bc -l)

    # Escribir las coordenadas en metros en el archivo de salida
    echo "$meters_x $meters_y" >> "$output_file"
done < "$input_file"

# Generar el script de gnuplot
gnuplot_script=$(cat << EOF
set terminal pngcairo size 800,600 enhanced font 'Verdana,10'
set output "$output_image"
set title "Trayectoria GPS en Metros"
set xlabel "Desplazamiento Este-Oeste (metros)"
set ylabel "Desplazamiento Norte-Sur (metros)"
set grid
set size ratio -1  # Mantener la misma escala en ambos ejes
set xtics 2
set datafile separator " "
plot "$output_file" using 1:2 with lines title "Ruta GPS" lw 2 lt rgb "blue"
EOF
)

# Ejecutar gnuplot usando el script generado
echo "$gnuplot_script" | gnuplot

echo "Gráfico generado en metros: $output_image"

