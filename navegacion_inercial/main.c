
/*
 * Implementacion de las ecuaciones de actitud y navegación inercial
 * (attitud and inertial navigation)
 *
 * Paul D Groves Navigation using Inertial Sensors Tutorial 
 * submission to IEEE AESS Systems Magazine 2013; Revised March 2014
 */

#include "matrix.h"
#include "nav_utils.h"
#include "magn.h"
#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>


// --- Variables globales compartidas ---
Matrix* C_ib;
Matrix* V_gamma;	// el vector de GRAVEDAD

pthread_mutex_t lock;

double velocidad_y = 0;

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


// ------------------------------------------------------------
// Envuelve un ángulo a [-PI, PI]
// ------------------------------------------------------------
double wrap_pi(double angle)
{
    while (angle >  M_PI) angle -= 2.0 * M_PI;
    while (angle < -M_PI) angle += 2.0 * M_PI;
    return angle;
}

// ------------------------------------------------------------
// Fusión INS + Magnetómetro (filtro complementario simple)
// ------------------------------------------------------------
void actualizar_yaw_con_magnetometro(double *yaw_ins, double yaw_mag)
{
    // Ganancia de corrección (suave)
    const double Kp = 0.02;   // puedes probar entre 0.01 y 0.1

    // Error de rumbo entre magnetómetro e INS
    double error = wrap_pi(yaw_mag - *yaw_ins);

    // Corrige suavemente el yaw del INS
    *yaw_ins += Kp * error;

    // Mantener yaw envuelto
    *yaw_ins = wrap_pi(*yaw_ins);
}


#define GRAVEDAD 9.80665
long long int muestra = 0;

void * actitud(void *arg) {


	double theta, phi, psi;
	// IMUData imu;

	double ax, ay, az;
	double wx, wy, wz, dt;
	//double bias_gx, bias_gy, bias_gz;
	imu_init(&ax, &ay, &az);

	attitud_determination_zero(ax, ay, az, &theta, &phi);

	psi = 0.0;
	C_ib = attitude_matrix_init(phi, theta, psi);

	/* vector velocidad */
	Matrix* V_ib = M_zero(3, 1);
	/* vector posicion */
	Matrix* Pos_ib = M_zero(3, 1);

	/* intentamos una vel constante */

	Matrix* v_body = M_create(3, 1);
	M_set(v_body, 0, 0, 0.0);
	M_set(v_body, 1, 0, 0.7);
	M_set(v_body, 2, 0, 0.0);

	while (1) {
		// Leer giroscopios (rad/s)
		leer_imu(&ax, &ay, &az, &wx, &wy, &wz, &dt);

		double x, y, z, grados;
		long magn_timestamp;
		magnetometro_get((double) current_timestamp, &x, &y, &z, &grados, &magn_timestamp);
		printf("muestra=%i, GRADOS timestamp=%li curr=%li \n", muestra, magn_timestamp, current_timestamp);
		/* 
		if ((previous_timestamp <= magn_timestamp) && (magn_timestamp <= current_timestamp)) {
			printf("muestra=%i, GRADOS=%f \n", muestra, grados);

			// IMPORTANTE hago signo menos porque del magnetometro
			// tengo sentido horario e IMU y ecuaciones sentido
			// antihorario
			double psi_mag = -(grados * M_PI / 180.0);
			psi = get_yaw_from_Cib();

			// suavizado para no reemplazar brusco
			actualizar_yaw_con_magnetometro(&psi, psi_mag);

			phi = get_roll_from_Cib();
			theta = get_pitch_from_Cib();

	        	pthread_mutex_lock(&lock);
			M_free(C_ib);
			C_ib = attitude_matrix_init(phi, theta, psi);
        		pthread_mutex_unlock(&lock);
		}
		*/

		if (acceleration_zero(ax, ay, az)) {
			printf("muestra=%i, MAGNITUD=%f \n", muestra, sqrt(ax*ax + ay*ay + az*az));
			attitud_determination_zero(ax, ay, az, &theta, &phi);
			psi = get_yaw_from_Cib();

	        	pthread_mutex_lock(&lock);
			M_free(C_ib);
			C_ib = attitude_matrix_init(phi, theta, psi);
        		pthread_mutex_unlock(&lock);
			continue;
		}

		printf("muestra=%lli, ax=%f, ay=%f, az=%f, wx=%f, wy=%f, wz=%f dt=%f\n",
	       		muestra++, ax, ay, az, wx, wy, wz, dt);

		Matrix* Omega_dt = gyro_matrix_build(wx, wy, wz, dt);
		Matrix* C_new = attitude_update(C_ib, Omega_dt);

		pthread_mutex_lock(&lock);
		M_free(C_ib);
		M_free(Omega_dt);
		C_ib = C_new;
		pthread_mutex_unlock(&lock);

		phi = get_roll_from_Cib();
		theta = get_pitch_from_Cib();
		psi = get_yaw_from_Cib();
		printf("GRADOS TODOS roll=%f, pitch=%f, yaw=%f \n", phi*180/M_PI, theta*180/M_PI, psi*180/M_PI);

		// f_b: vector 3x1 medido por el acelerómetro (en marco cuerpo)
		Matrix* f_b = accel_vector_build(ax, ay, az);

		// Aplica ecuación (20): f_i = C_ib * f_b
		Matrix* f_i = M_mult(C_ib, f_b);

		
		/* a_i = f_i + V_gamma : Groves (21) */
		Matrix* a_i = M_add(f_i, V_gamma);
		printf("ACEL muestra=%lli, ax=%f, ay=%f, az=%f, wx=%f, wy=%f, wz=%f dt=%f\n",
	       		muestra, M_get(a_i, 0, 0), M_get(a_i, 1, 0), M_get(a_i, 2, 0), wx, wy, wz, dt);

		/* calculamos nueva velocidad: Groves (24) */
		M_scale(a_i, dt);
		Matrix* V_temp = M_add(V_ib, a_i);

		M_free(V_ib);
		V_ib = V_temp;
		velocidad_y = M_get(V_ib, 1, 0);


		/* calculamos la nueva posición en cada eje */
		/*
		// FALTA escalar la Vel de V_ib * 9.8 
		Matrix* V_temp2 = M_add(V_ib, M_zero(3,1));
		M_scale(V_temp2, dt);
		Matrix* V_temp3 = M_add(Pos_ib, V_temp2);
		M_free(Pos_ib);
		M_free(V_temp2);
		Pos_ib = V_temp3;
		*/

		/* calculamos la nueva posición en usando el HACK de vel CONSTANTE cada eje */
		Matrix* v_world = M_mult(C_ib, v_body);
		M_scale(v_world, dt);
		Matrix* V_temp3 = M_add(Pos_ib, v_world);
		M_free(Pos_ib);
		M_free(v_world);
		Pos_ib = V_temp3;


		printf("VEL muestra=%lli, vx=%f, vy=%f, vz=%f  \n",
	       		muestra, M_get(V_ib, 0, 0)*GRAVEDAD, M_get(V_ib, 1, 0)*GRAVEDAD, M_get(V_ib, 2, 0)*GRAVEDAD);
		if (muestra>65000) {
		printf("POS muestra=%lli, %f %f %f \n",
	       		muestra, M_get(Pos_ib, 0, 0), M_get(Pos_ib, 1, 0), M_get(Pos_ib, 2, 0));
		}

		if (muestra == 50000) {
			M_set(V_ib, 0, 0, 0);
			M_set(V_ib, 1, 0, 0);
			M_set(V_ib, 2, 0, 0);
			velocidad_y = 0;
			M_set(Pos_ib, 0, 0, 0);
			M_set(Pos_ib, 1, 0, 0);
			M_set(Pos_ib, 2, 0, 0);
		}

		// Liberar
		M_free(f_b);
		M_free(f_i);
		M_free(a_i);

		display_redraw();
	}




    return 0;
}

int main(int argc, char** argv)
{
	pthread_mutex_init(&lock, NULL);

	acceleration_zero_init();
	// Inicializa matriz identidad
	C_ib = M_identity(3);

	// El vector de GRAVEDAD en g
	V_gamma = M_create(3,1);
	M_set(V_gamma, 0,0, 0.0);
	M_set(V_gamma, 1,0, 0.0);
	M_set(V_gamma, 2,0, -1.0);

	display_init(argc, argv);

	leer_magnetometro_data("magnetometro.txt");

	// Inicia thread que actualiza C_ib
	pthread_t th;
	pthread_create(&th, NULL, actitud, NULL);

	display_loop();

	// Limpieza
	pthread_mutex_destroy(&lock);
	M_free(C_ib);
}
