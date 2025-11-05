
#include "matrix.h"
#include "nav_utils.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

extern Matrix* C_ib;

// bias/sesgo de los datos crudos de los giroscopos
int bias_gx;
int bias_gy;
int bias_gz;

long previous_timestamp = -1;
long current_timestamp = -1;

// bias/sesgo de los acelerometros (calibrado segun apunte de Favio)
double bias_ax = 0.077539;
double bias_ay = -0.039652;
double bias_az = -0.058538;
double factor_de_escala_ax = 0.016197;
double factor_de_escala_ay = 0.011726;
double factor_de_escala_az = -0.007254;

#define AZ_N 20
double az_mag[AZ_N];

void acceleration_zero_init(void)
{
	int i;
	for (i=0; i<AZ_N; i++)
		az_mag[i] = -1.0;
}

int acceleration_zero(double ax, double ay, double az)
{
	static int n = 0;
	int i;

	az_mag[n] = sqrt(ax*ax + ay*ay + az*az);
	n++; if (n == AZ_N) n = 0;
	for (i=0; i<AZ_N; i++) {
		if ((az_mag[i] < 0.99) || (az_mag[i] > 1.009)) {
			return 0;
		}
	}
	acceleration_zero_init();
	return 1;
}


// devuelve valores del acelerometro en g (quitando bias etc)
// devuelve valores del giroscopo en radianes por segundo (quitando bias etc)
// y delta_t en segundos
void leer_imu(double *ax, double *ay, double *az, 
		double *wx, double *wy, double *wz, double *dt)
{
        IMUData imu;

	previous_timestamp = current_timestamp;

        leer_siguiente_imu(&imu);
	// convertimos datos crudos de acelerometros a g (quitando bias etc)
        *ax = ( (imu.ax * 2.0 / 512.0) - bias_ax ) / (1.0 + factor_de_escala_ax);
        *ay = ( (imu.ay * 2.0 / 512.0) - bias_ay ) / (1.0 + factor_de_escala_ay);
        *az = ( (imu.az * 2.0 / 512.0) - bias_az ) / (1.0 + factor_de_escala_az);

        // 14.375 factor de escala de nuestra IMU
	// 57.29577951 grados = 1 radian
	// convertimos datos crudos giroscopo a radianes por segundo
        *wx = (imu.gx - bias_gx) / 14.375 / 57.29577951;
        *wy = (imu.gy - bias_gy) / 14.375 / 57.29577951;
        *wz = (imu.gz - bias_gz) / 14.375 / 57.29577951;

        *dt = imu.delta_t / 1000.0;

	current_timestamp = imu.timestamp;
}


/* -----------------------------------------------------------
   Determinación inicial de actitud (nivelación)
   Usa acelerómetros: ax, ay, az  (en m/s² o en g)
   Devuelve pitch (θ) y roll (φ)
----------------------------------------------------------- */
void attitud_determination_zero(double ax, double ay, double az,
                                double* theta, double* phi)
{
    // pitch (θ) y roll (φ) según ecuaciones típicas de nivelación
    *theta = atan2(-ax, sqrt(ay*ay + az*az));   // pitch
    *phi   = atan2(ay, az);                     // roll
}


// --- Yaw (ψ) ---
double get_yaw_from_Cib(void)
{
	/* C_ib es global en todo el sistema */
    // ψ = atan2(C_ib[1][0], C_ib[0][0])
    return atan2(M_get(C_ib, 1, 0), M_get(C_ib, 0, 0));
}

// --- Pitch (θ) ---
double get_pitch_from_Cib(void)
{
	/* C_ib es global en todo el sistema */
    // θ = -asin(C_ib[2][0])
    return -asin(M_get(C_ib, 2, 0));
}

// --- Roll (φ) ---
double get_roll_from_Cib(void)
{
	/* C_ib es global en todo el sistema */
    // φ = atan2(C_ib[2][1], C_ib[2][2])
    return atan2(M_get(C_ib, 2, 1), M_get(C_ib, 2, 2));
}



/* -----------------------------------------------------------
   Construye matriz inicial C_ib (convención ZYX de Groves)
   φ = roll, θ = pitch, ψ = yaw (psi debe ser cero 0 al inicio
   				 si no hay magnetometro)
----------------------------------------------------------- */
Matrix* attitude_matrix_init(double phi, double theta, double psi)
{
    Matrix* C = M_create(3, 3);

    double cphi = cos(phi), sphi = sin(phi);
    double cth  = cos(theta), sth = sin(theta);
    double cpsi = cos(psi), spsi = sin(psi);

    // C_ib según convención ZYX (yaw–pitch–roll)
    M_set(C, 0, 0, cpsi*cth);
    M_set(C, 0, 1, cpsi*sth*sphi - spsi*cphi);
    M_set(C, 0, 2, cpsi*sth*cphi + spsi*sphi);

    M_set(C, 1, 0, spsi*cth);
    M_set(C, 1, 1, spsi*sth*sphi + cpsi*cphi);
    M_set(C, 1, 2, spsi*sth*cphi - cpsi*sphi);

    M_set(C, 2, 0, -sth);
    M_set(C, 2, 1, cth*sphi);
    M_set(C, 2, 2, cth*cphi);

    return C;
}

/* -----------------------------------------------------------
   Construye el vector de la fuerza obtenida por los acelerometros
----------------------------------------------------------- */
Matrix* accel_vector_build(double ax, double ay, double az)
{
	Matrix* f_b = M_create(3, 1);
	M_set(f_b, 0, 0, ax);   // componente X del acelerómetro
	M_set(f_b, 1, 0, ay);   // componente Y
	M_set(f_b, 2, 0, az);   // componente Z

	return f_b;
}

/* -----------------------------------------------------------
   Construye la matriz incremental [I + Ω*dt]
   wx, wy y wz : valores en radianes por segundo
   dt: en segundos (ej: 0.001 = 1ms) 
   según ecuación (19) de Groves
----------------------------------------------------------- */
Matrix* gyro_matrix_build(double wx, double wy, double wz, double dt)
{
    Matrix* Omega = M_create(3, 3);

    // Matriz Ω (antisimétrica)
    M_set(Omega, 0, 0, 0.0);
    M_set(Omega, 0, 1, -wz);
    M_set(Omega, 0, 2, wy);

    M_set(Omega, 1, 0, wz);
    M_set(Omega, 1, 1, 0.0);
    M_set(Omega, 1, 2, -wx);

    M_set(Omega, 2, 0, -wy);
    M_set(Omega, 2, 1, wx);
    M_set(Omega, 2, 2, 0.0);

    // Escalar por dt
    M_scale(Omega, dt);

    // [I + Ω*dt]
    Matrix* I = M_identity(3);
    Matrix* Omega_dt = M_add(I, Omega);

    M_free(Omega);
    M_free(I);
    return Omega_dt;
}


/* -----------------------------------------------------------
   Actualización de actitud (Groves ec. 19)
   C_new = [I - Ω*dt] * C_prev
----------------------------------------------------------- */
Matrix* attitude_update(const Matrix* C_prev, const Matrix* Omega_dt)
{
    Matrix* C_new = M_mult(Omega_dt, C_prev);
    return C_new;
}


// La función devuelve 1 si hay más datos, 0 si se llegó al final del archivo
int leer_siguiente_imu(IMUData *data) {
    static FILE *fp = NULL;
    static long last_timestamp = 0;
    static long sample = 0;

    // Abrir el archivo solo una vez
    if (fp == NULL) {
        fp = fopen("imu.txt", "r");
        if (fp == NULL) {
            perror("Error al abrir imu.txt");
            return 0;
        }
    }

    sample++;
    printf("sample=%li \n", sample);

    // Leer una línea del archivo
    char linea[256];
    if (fgets(linea, sizeof(linea), fp) == NULL) {
        // Llegamos al final del archivo
        fclose(fp);
        fp = NULL;
	exit(0);
        return 0;
    }

    // Parsear los valores
    if (sscanf(linea, "%ld %d %d %d %d %d %d",
               &data->timestamp,
               &data->ax, &data->ay, &data->az,
               &data->gx, &data->gy, &data->gz) != 7) {
        fprintf(stderr, "Línea con formato inválido: %s\n", linea);
        return leer_siguiente_imu(data);  // intentar siguiente línea
    }

    // Calcular delta_t
    if (last_timestamp == 0)
        data->delta_t = 0;  // primera muestra
    else
        data->delta_t = data->timestamp - last_timestamp;

    printf("timestamp=%li\n", data->timestamp);
    last_timestamp = data->timestamp;

    return 1;
}
