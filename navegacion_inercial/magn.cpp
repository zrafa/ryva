/*
 * detectar_troncos.cpp: sistema de posicionamiento de un tractor 
 * dentro de las hileras de una chacra.
 */

#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>

#include <sstream>
#include <cmath>
#include <limits>
#include <algorithm>

#include <unistd.h>

#include <opencv2/opencv.hpp>

#include "magn.h"




using namespace std;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 


std::vector<MagnetometroData> data_magnetometro;


// Función para leer los datos del archivo magnetometro.txt
std::vector<MagnetometroData> readMagnetometroData(const std::string& filename) {
    // std::vector<MagnetometroData> data;
    std::ifstream file(filename);
    std::string line;

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        double valor1, x, y, z; 
	long timestamp;
        char ch;

        // Leer el primer valor (valor1) y descartarlo
        iss >> valor1;

        // Leer los valores [x, y, z]
        iss >> ch; // Leer el '['
        iss >> x >> ch >> y >> ch >> z >> ch; // Leer x, y, z y el ']'

        // Leer el timestamp
        iss >> timestamp;

        // Almacenar los datos en el vector
        data_magnetometro.push_back({timestamp, x, y, z});

    }

    // return data;
    return data_magnetometro;
}

    // Aplicar el offset de calibración
    double x_offset = -984.6073282;
    double y_offset =  373.97952865;
    double z_offset =  313.13908064;

    // Matriz de transformación (obtenida de la calibración)
    cv::Mat transformation = (cv::Mat_<double>(3, 3) <<
 0.95534472, -0.02389597,  0.0,        
 -0.02389597,  1.04829138,  0.0,        
  0.0,          0.0,          0.99909221);


extern "C" {

// Función para obtener los valores del magnetómetro 
// más cercanos a la marca de tiempo
void magnetometro_get(long tiempo_ms, double* x, double* y, double* z, double* grados, long *magn_timestamp) {
    double min_diff = std::numeric_limits<double>::max();
    MagnetometroData closest_data;

    // Buscar el registro más cercano a tiempo_ms
    for (const auto& entry : data_magnetometro) {
        double diff = std::abs(entry.timestamp - tiempo_ms);

        if (diff < min_diff) {
            min_diff = diff;
            closest_data = entry;
	    *magn_timestamp = entry.timestamp;
        }
    }

    *x = closest_data.x - x_offset;
    *y = closest_data.y - y_offset;
    *z = closest_data.z - z_offset;

    // Datos crudos del magnetómetro (ejemplo: [-1450.0, -1387.0, 25.0])
    cv::Mat raw_data = (cv::Mat_<double>(1, 3) << *x, *y, *z);

	cv::Mat calibrated_data = raw_data * transformation.t();
    // Obtener los componentes x, y, z de calibrated_data
    *x = calibrated_data.at<double>(0, 0);  // Componente x
    *y = calibrated_data.at<double>(0, 1);  // Componente y
    *z = calibrated_data.at<double>(0, 2);  // Componente z
						  
    // Calcular los grados (ángulo en el plano XY)
    *grados = std::atan2(*y, *x) * 180 / CV_PI;
    if (*grados < 0) {
        *grados += 360;
    }
}


void* leer_magnetometro_data(const char* filename) {
    auto data = new std::vector<MagnetometroData>(readMagnetometroData(filename));
    return data; // devuelve puntero opaco
}

}   // fin de extern "C"
