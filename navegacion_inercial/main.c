
/*
 * Implementacion de las ecuaciones de actitud y navegación inercial
 * (attitud and inertial navigation)
 *
 * Paul D Groves Navigation using Inertial Sensors Tutorial 
 * submission to IEEE AESS Systems Magazine 2013; Revised March 2014
 */

#include "matrix.h"
#include "nav_utils.h"
#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>


// --- Variables globales compartidas ---
Matrix* C_ib;
pthread_mutex_t lock;

void display_init(int argc, char** argv);
void display_loop();
void display_redraw();

void* actualizar(void* arg)
{
    double t = 0.0;

    while (1) {
        // Calcula una rotación lenta alrededor del eje Z
        double phi = 0.0;             // roll
        double theta = 0.0;           // pitch
        double psi = t;               // yaw cambia con el tiempo

        double cphi = cos(phi), sphi = sin(phi);
        double cth = cos(theta), sth = sin(theta);
        double cpsi = cos(psi), spsi = sin(psi);

        Matrix* nueva = M_create(3,3);
        // Matriz C_ib (ZYX)
        M_set(nueva, 0,0, cpsi*cth);
        M_set(nueva, 0,1, cpsi*sth*sphi - spsi*cphi);
        M_set(nueva, 0,2, cpsi*sth*cphi + spsi*sphi);
        M_set(nueva, 1,0, spsi*cth);
        M_set(nueva, 1,1, spsi*sth*sphi + cpsi*cphi);
        M_set(nueva, 1,2, spsi*sth*cphi - cpsi*sphi);
        M_set(nueva, 2,0, -sth);
        M_set(nueva, 2,1, cth*sphi);
        M_set(nueva, 2,2, cth*cphi);

        pthread_mutex_lock(&lock);
        M_free(C_ib);
        C_ib = nueva;
        pthread_mutex_unlock(&lock);

        display_redraw();  // glutPostRedisplay();  // pide redibujar

        t += 0.1;  // avanza el ángulo
        sleep(1);
    }
    return NULL;
}


void imu_init(double *acelx, double *acely, double *acelz)
{
	int i;
	IMUData imu;
	long long int ax, ay, az, gx, gy, gz;
	ax=0; ay=0; az=0; gx=0; gy=0; gz=0;

	int CANT_MUESTRAS = 1000;

	/* primeras muestras para obtener actitud 
	 * y calcular sesgo de giroscopo */
	for (i=0; i<CANT_MUESTRAS; i++) {
		leer_siguiente_imu(&imu);
		ax += imu.ax; ay += imu.ay; az += imu.az;
		gx += imu.gx; gy += imu.gy; gz += imu.gz;
	}
	ax /= CANT_MUESTRAS; ay /= CANT_MUESTRAS; az /= CANT_MUESTRAS;
	gx /= CANT_MUESTRAS; gy /= CANT_MUESTRAS; gz /= CANT_MUESTRAS;

	// convertimos datos crudos de acelerometros a g (quitando bias etc)
	*acelx = ( (ax * 2.0 / 512.0) - bias_ax ) / (1.0 + factor_de_escala_ax);
	*acely = ( (ay * 2.0 / 512.0) - bias_ay ) / (1.0 + factor_de_escala_ay);
	*acelz = ( (az * 2.0 / 512.0) - bias_az ) / (1.0 + factor_de_escala_az);
	bias_gx = gx;
	bias_gy = gy;
	bias_gz = gz;
	printf("ax=%f, ay=%f, az=%f, bias_gx=%i, bias_gy=%i, bias_gz=%i \n",
	       *acelx, *acely, *acelz, bias_gx, bias_gy, bias_gz);
	//exit(0);
}


long long int muestra = 0;

void * actitud(void *arg) {


	double theta, phi;
	// IMUData imu;

	double ax, ay, az;
	double wx, wy, wz, dt;
	//double bias_gx, bias_gy, bias_gz;
	imu_init(&ax, &ay, &az);


	attitud_determination_zero(ax, ay, az, &theta, &phi);

	//Matrix* C_ib = attitude_matrix_init(phi, theta);
	C_ib = attitude_matrix_init(phi, theta);

	while (1) {
		// Leer giroscopios (rad/s)
		leer_imu(&ax, &ay, &az, &wx, &wy, &wz, &dt);
		printf("muestra=%lli, ax=%f, ay=%f, az=%f, wx=%f, wy=%f, wz=%f dt=%f\n",
	       		muestra++, ax, ay, az, wx, wy, wz, dt);

		Matrix* Omega_dt = gyro_matrix_build(wx, wy, wz, dt);
		Matrix* C_new = attitude_update(C_ib, Omega_dt);

	        pthread_mutex_lock(&lock);
        	M_free(C_ib);
		M_free(Omega_dt);
		C_ib = C_new;
        	pthread_mutex_unlock(&lock);

		//M_free(C_ib);
		//M_free(Omega_dt);
		//C_ib = C_new;

		display_redraw();
		// ESPERAR
		// sleep(1);

	}



    // Operaciones
    // printf("Matriz A:\n");
    // M_print(A);


    return 0;
}

int main(int argc, char** argv)
{
	pthread_mutex_init(&lock, NULL);

	// Inicializa matriz identidad
	C_ib = M_identity(3);

	display_init(argc, argv);

	// Inicia thread que actualiza C_ib
	pthread_t th;
	pthread_create(&th, NULL, actitud, NULL);

	display_loop();

	// Limpieza
	pthread_mutex_destroy(&lock);
	M_free(C_ib);
}
