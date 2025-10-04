/*
 * programa para calibrar el giroscopo
 *
 * las ecuaciones son de NotasCursoPosicion.pdf, by Favio Masson
 */

#include <stdio.h>

#define RAW_FILE_X_UP	"giroscopo_ejex.dat"
#define RAW_FILE_X_DOWN	"giroscopo_ejex_abajo.dat"
#define RAW_FILE_Y_UP	"giroscopo_ejey.dat"
#define RAW_FILE_Y_DOWN	"giroscopo_ejey_abajo.dat"
#define RAW_FILE_Z_UP	"giroscopo_ejez.dat"
#define RAW_FILE_Z_DOWN	"giroscopo_ejez_abajo.dat"

#define RADIAN	57.29577951

/* velocidad angular del laboratorio de calibracion (33 RPM) */
#define VEL_ANG_LAB 3.49065850417644488034

#define gravity (9.80665)

/* 
 * Promedia una de las columnas de los datos crudos
 * raw file contents:
 * timestamp ax, ay, az, gx, gy, gz
 * n argument is the column of interest
 */
double raw_mean_from_file(const char *raw_file, int n) 
{
	int raw[7];
	int i;
	int sum;
	char buf[400];

	FILE *ff;

	ff = fopen(raw_file, "r");

	/* remove title line */
	fgets(buf, sizeof(buf), ff);

	i = 0;
	sum = 0;
	while (fgets(buf, sizeof(buf), ff)) {
		//printf("sum : %i \n", sum);
		sscanf(buf, "%li %i %i %i %i %i %i", &raw[0], &raw[1], &raw[2], &raw[3], &raw[4], &raw[5], &raw[6]);
		sum = sum + raw[n];
		i++;
	}

	fclose(ff);
	return ((double)sum/i);
}


/*
 * calcula el sesgo y factor de escala 
 * 	raw file contents:
 * 	timestamp ax, ay, az, gx, gy, gz
 * 	n argument is the column of interest
 * 	f arguments are the raw data files
 */
void sesgo(const char *f1, const char *f2, int n, double *s, double *fs)
{
	double fup, fdown, sesgo_int;
	double sesgo_d, factor_d;
	fup = (raw_mean_from_file(f1, n) / 14.375) / RADIAN;
	printf("la media up : %f\n", raw_mean_from_file(f1, n));
	printf("la media up : %f\n", fup);
	fdown = (raw_mean_from_file(f2, n) / 14.375) / RADIAN;
	printf("la media down : %f\n", raw_mean_from_file(f2, n));
	printf("la media down : %f\n", fdown);

	/* ecuacion (2.92), pagina 119, NotasCursoPosicion.pdf */
	/* calculamos sesgo */
	sesgo_d = (fup + fdown) / 2; 
	//sesgo_d = sesgo_d * gravity / 255.0;
	*s = sesgo_d;

	/* calculamos factor de escala - ecuacion (2.93) */
	/* IMPORTANTE : la idea es que (fup - fdown) sea cercano a 2*VEL_ANG_LAB
	 * 	        de manera tal que el término superior  
	 * 	        (fup-fdown)-(2*VEL_ANG_LAB) sea cercano a cero.
	 * 	        En mi caso, hay que dar vuelta fup y fdown!! .
	 */
	// factor_d = ((fup - fdown - 2*VEL_ANG_LAB) / (2*VEL_ANG_LAB));
	factor_d = ((fdown - fup - 2*VEL_ANG_LAB) / (2*VEL_ANG_LAB));
	*fs = factor_d;

//	return sesgo_d;
}

void main(void) {
	double sesgo_d, factor_d;

	printf("VALORES RESULTADOS EN RADIANES \n\n\r");
	sesgo(RAW_FILE_X_UP, RAW_FILE_X_DOWN, 4, &sesgo_d, &factor_d);
	printf("sesgo de giroscopo en x : %f \n", sesgo_d);
	printf("factor de escala de giroscopo en x : %f \n\n", factor_d);

	sesgo(RAW_FILE_Y_UP, RAW_FILE_Y_DOWN, 5, &sesgo_d, &factor_d);
	printf("sesgo de giroscopo en y : %f \n", sesgo_d);
	printf("factor de escala de giroscopo en y : %f \n\n", factor_d);

	sesgo(RAW_FILE_Z_UP, RAW_FILE_Z_DOWN, 6, &sesgo_d, &factor_d);
	printf("sesgo de giroscopo en z : %f \n", sesgo_d);
	printf("factor de escala de giroscopo en z : %f \n\n", factor_d);
}
