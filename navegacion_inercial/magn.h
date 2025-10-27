/*
 * detectar_troncos.cpp: sistema de posicionamiento de un tractor 
 * dentro de las hileras de una chacra.
 */

#ifndef MAGN_H
#define MAGN_H

#ifdef __cplusplus
extern "C" {
#endif

// using namespace std;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 


struct MagnetometroData {
    long timestamp; // Marca de tiempo en milisegundos
    double x, y, z;   // Valores crudos del magnetómetro
};

// Función para leer los datos del archivo magnetometro.txt
// std::vector<MagnetometroData> readMagnetometroData(const std::string& filename);
// No std:: ni templates ni clases aquí
void* leer_magnetometro_data(const char* filename);

// Función para obtener los valores del magnetómetro 
// más cercanos a la marca de tiempo
void magnetometro_get(long tiempo_ms, double* x, double* y, double* z, double* grados, long *magn_timestamp);


#ifdef __cplusplus
}
#endif

#endif  // de MAGN_H
