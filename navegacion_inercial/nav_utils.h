
#ifndef NAV_UTILS_H
#define NAV_UTILS_H


typedef struct {
    long timestamp;
    int ax, ay, az;
    int gx, gy, gz;
    long delta_t;
} IMUData;

extern int bias_gx;
extern int bias_gy;
extern int bias_gz;
extern double bias_ax;
extern double bias_ay;
extern double bias_az;
extern double factor_de_escala_ax;
extern double factor_de_escala_ay;
extern double factor_de_escala_az;

int leer_siguiente_imu(IMUData *data);
void leer_imu(double *ax, double *ay, double *az,
		double *gx, double *gy, double *gz, double *dt);

Matrix* accel_vector_build(double ax, double ay, double az);

void acceleration_zero_init(void);
int acceleration_zero(double ax, double ay, double az);


/* -----------------------------------------------------------
   Determinación inicial de actitud (nivelación)
   Usa acelerómetros: ax, ay, az  (en m/s²)
   Devuelve pitch (θ) y roll (φ)
----------------------------------------------------------- */
void attitud_determination_zero(double ax, double ay, double az,
                                double* theta, double* phi);

/* -----------------------------------------------------------
   Construye matriz inicial C_ib (convención ZYX de Groves)
   φ = roll, θ = pitch, ψ = yaw (asumido 0 al inicio)
----------------------------------------------------------- */
Matrix* attitude_matrix_init(double phi, double theta);

/* -----------------------------------------------------------
   Construye la matriz incremental [I - Ω*dt]
   según ecuación (19) de Groves
----------------------------------------------------------- */
Matrix* gyro_matrix_build(double wx, double wy, double wz, double dt);

/* -----------------------------------------------------------
   Actualización de actitud (Groves ec. 19)
   C_new = [I - Ω*dt] * C_prev
----------------------------------------------------------- */
Matrix* attitude_update(const Matrix* C_prev, const Matrix* Omega_dt);

#endif // NAV_UTILS_H

