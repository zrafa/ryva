
#!/usr/bin/env python3
import asyncio
import websockets
import json
import random
import base64
import os
import sys
import glob
import subprocess
# Archivos donde se guardan los datos de los sensores
imu_file = sys.argv[1]
lidar_file = sys.argv[2]
magne_file = sys.argv[3]
gps_file = sys.argv[4]
dir_file = sys.argv[5]

# Function to encode an image to Base64
def encode_image_to_base64(image_path):
    with open(image_path, "rb") as image_file:
        return base64.b64encode(image_file.read()).decode('utf-8')

# Función para leer la última línea de un archivo
def read_sensor_data(filename):
        with open(filename, "r") as file:
                lines = file.readlines()
                if lines:
                    return lines[-2].strip()  # Retornar la última línea sin saltos de línea
                return "N/A"  # En caso de que el archivo esté vacío


def filter_gps_data(gps_lines):
    # Filtrar las líneas que comienzan con "$GNRMC"
    return [line for line in gps_lines if line.startswith("$GNRMC")]


# Function to send updated image every 500ms
@asyncio.coroutine  # For Python 3.5.3, you need to use @asyncio.coroutine decorator
async def send_sensor_data(websocket, path):
    n=0
    while True:
        # Leer y promediar 20 muestras (~100 ms de datos a 100 Hz) por cada sensor
        imu_lines = read_sensor_data(imu_file)
        lidar_lines = read_sensor_data(lidar_file)
        magne_lines = read_sensor_data(magne_file)
        gps_lines = read_sensor_data(gps_file)
        filtered_gps_lines = filter_gps_data(gps_lines)

        # Preparar los datos en un diccionario
        data = {
            "imu": imu_lines,
            "lidar": lidar_lines,
            "magnetometro": magne_lines,
            "gps": filtered_gps_lines
        }

        n=n+1
        if n==10:
            n=0
            # Obtener todos los archivos jpg en el directorio
            jpg_files = glob.glob(os.path.join(dir_file, "*.jpg"))

            # Ordenar los archivos por fecha de modificación (de más antiguo a más reciente)
            jpg_files.sort(key=os.path.getmtime)

            # Verificamos si hay más de un archivo para obtener el penúltimo
            if len(jpg_files) > 1:
                # Obtenemos el archivo más reciente antes del último
                recent_file = jpg_files[-2]
            else:
                print("No hay suficientes archivos JPG en el directorio.")

            # Ejecutamos jpegoptim para reducir el tamaño del archivo JPEG sin pérdida significativa de calidad
            # subprocess.run(["jpegoptim", "--max=50", recent_file])
            # Leer la imagen (suponiendo que la imagen se llama 'image.jpg')
            with open(recent_file, "rb") as image_file:
                data["image"] = base64.b64encode(image_file.read()).decode('utf-8')

        # Enviar los datos como un JSON
        message = json.dumps(data)
        # print("Enviando datos:", message)  # Agregar mensaje de depuración
        await websocket.send(message)

        # Esperar 500 ms
        await asyncio.sleep(0.5)

# Main function compatible with Python 3.5.3
@asyncio.coroutine
def main():
    ws_server = yield from websockets.serve(send_sensor_data, "0.0.0.0", 8765)
    yield from ws_server.wait_closed()

# Start event loop for Python 3.5.3
if __name__ == "__main__":
    loop = asyncio.get_event_loop()
    loop.run_until_complete(main())
    loop.run_forever()



