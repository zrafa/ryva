/*
 * gps.cpp: procesa las tramas del gps
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

using namespace std;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

struct GPS_position {
    double latitude;  // Latitud en grados decimales
    double longitude; // Longitud en grados decimales

    GPS_position(double lat = 0.0, double lon = 0.0) : latitude(lat), longitude(lon) {}
};

	// GPS_position pos1(latitud, longitud);
	// GPS_position pos2(arbol.latitud, arbol.longitud);
	



double haversine_distance(const GPS_position& pos1, const GPS_position& pos2)
{
    const double R = 6371.0; // Radio de la Tierra en kilómetros

    double lat1 = pos1.latitude * M_PI / 180.0;
    double lon1 = pos1.longitude * M_PI / 180.0;
    double lat2 = pos2.latitude * M_PI / 180.0;
    double lon2 = pos2.longitude * M_PI / 180.0;

    double dlat = lat2 - lat1;
    double dlon = lon2 - lon1;

    double a = sin(dlat / 2) * sin(dlat / 2) +
               cos(lat1) * cos(lat2) *
               sin(dlon / 2) * sin(dlon / 2);
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));

    return R * c; // Distancia en kilómetros
}

// Estructura para almacenar los datos GPS
struct GPS_data {
    long timestamp_us;
    double latitude;
    double longitude;
};


// obtener latitud y longitud en base a una marca de tiempo
void gps_get_lat_lon(long tiempo_us, double *latitud, double *longitud, long int *timestamp) 
{
    ifstream file("gps.txt");
    if (!file.is_open()) {
        cerr << "Error: No se pudo abrir el archivo gps.txt" << endl;
        return;
    }

    // Variables para almacenar la trama más cercana
    GPS_data closest_data;
    long min_time_diff = numeric_limits<long>::max();
    string line;
    long prev_timestamp_us = 0;

    // Leer el archivo línea por línea
    while (getline(file, line)) {
        if (line.find("$GNRMC") != string::npos) {
            // Procesar la línea $GNRMC
            stringstream ss(line);
            string token;
            vector<string> tokens;
            while (getline(ss, token, ',')) {
                tokens.push_back(token);
            }

            // Extraer latitud y longitud
            double latitude = stod(tokens[3].substr(0, 2)) + stod(tokens[3].substr(2)) / 60.0;
            double longitude = stod(tokens[5].substr(0, 3)) + stod(tokens[5].substr(3)) / 60.0;
            if (tokens[4] == "S") latitude *= -1;
            if (tokens[6] == "W") longitude *= -1;

            // Calcular la diferencia de tiempo con el tiempo_us proporcionado
            long time_diff = abs(prev_timestamp_us - tiempo_us);

            // Si esta trama es la más cercana hasta ahora, guardarla
            if (time_diff < min_time_diff) {
                min_time_diff = time_diff;
                closest_data = {prev_timestamp_us, latitude, longitude};
            }
        } else {
            // Extraer la marca de tiempo (en us)
            stringstream ss(line);
            ss >> prev_timestamp_us;
        }
    }

    file.close();

    // Verificar si min_time_diff está dentro del rango
    if (min_time_diff <= 1000000) {
	    // rango_inferior && min_time_diff <= rango_superior) {
        *latitud = (double) closest_data.latitude;
        *longitud = (double) closest_data.longitude;
	*timestamp = closest_data.timestamp_us;
    } else {
        *latitud = (double) -1.0;
        *longitud = (double) -1.0;
	*timestamp = 0;
    }

}

long int gps_ts = -1;
double gps_lat, gps_lon;
double gps_vel = 0;

struct VEL_data {
	double vel;
	long ts;	// timestamp
};

#define N_VEL 5
struct VEL_data velocidades[N_VEL];
void velocidades_init(void)
{
	int i;
	for (i=0;i<N_VEL;i++) {
		velocidades[i].vel = 0;
		velocidades[i].ts = 0;
	}
}

int vel_i = 0;

extern "C" {


double velocidad_media(double vel, long ts)
{
	double media_p = 0;
	int i;

	if ((vel != -1) && (velocidades[vel_i].ts != ts)) {
		vel_i++;
		if (vel_i == N_VEL)
			vel_i = 0;

		velocidades[vel_i].vel = vel;
		velocidades[vel_i].ts = ts;
	}
	
	for (i=0;i<N_VEL;i++) {
		media_p += velocidades[i].vel;
	}
	media_p /= N_VEL;

	return media_p;
}


void gps_velocidad(long int ts, double *vel, long *ts_current)
{
	double lat, lon;
	long int timestamp;

	long int dist =  (ts > gps_ts) ? (ts - gps_ts) : (gps_ts - ts);
	if (dist < 500) {
		*vel = -1;
		return;
	}

	gps_get_lat_lon(ts*1000, &lat, &lon, &timestamp); 

	if (gps_ts == -1) {  // primera muestra
		gps_lat = lat;
		gps_lon = lon;
		gps_ts = timestamp/1000;
		gps_vel = 0;
		*vel = -1;
		return;
	}

	
	
	GPS_position pos1(gps_lat, gps_lon);
	GPS_position pos2(lat, lon);

	double distancia = haversine_distance(pos1, pos2);
	gps_vel = distancia*1000 / ((timestamp/1000.0 - ts)/1000.0);
	gps_lat = lat;
	gps_lon = lon;
	gps_ts = timestamp/1000;

	*vel = gps_vel;
	*ts_current = timestamp/1000;
}
}
