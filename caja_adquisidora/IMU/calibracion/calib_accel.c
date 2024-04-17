/*
 * programa para calibrar el acelerometro
 *
 * las ecuaciones son de NotasCursoPosicion.pdf, by Favio Masson
 */

#include <stdio.h>

#define RAW_FILE_X_UP	"ejex_arriba.txt"
#define RAW_FILE_X_DOWN	"ejex_abajo.txt"
#define RAW_FILE_Y_UP	"ejey_arriba.txt"
#define RAW_FILE_Y_DOWN	"ejey_abajo.txt"
#define RAW_FILE_Z_UP	"ejez_arriba.txt"
#define RAW_FILE_Z_DOWN	"ejez_abajo.txt"

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
	return (sum/i);
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

	fup = raw_mean_from_file(f1, n) * gravity / 255.0;
	fdown = raw_mean_from_file(f2, n) * gravity / 255.0;
	/* ecuacion (2.88), pagina 118, NotasCursoPosicion.pdf */
	/* calculamos sesgo */
	sesgo_d = (fup + fdown) / 2; 
	*s = sesgo_d;

	/* calculamos factor de escala */
	factor_d = (fup - fdown - (2*gravity)) / (2*gravity);
	*fs = factor_d;

}

void main(void) {
	double sesgo_d, factor_d;

	sesgo(RAW_FILE_X_UP, RAW_FILE_X_DOWN, 1, &sesgo_d, &factor_d);
	printf("sesgo de acelerometro en x : %f \n", sesgo_d);
	printf("factor de escala de acelerometro en x : %f \n\n", factor_d);

	sesgo(RAW_FILE_Y_UP, RAW_FILE_Y_DOWN, 2, &sesgo_d, &factor_d);
	printf("sesgo de acelerometro en y : %f \n", sesgo_d);
	printf("factor de escala de acelerometro en y : %f \n\n", factor_d);

	sesgo(RAW_FILE_Z_UP, RAW_FILE_Z_DOWN, 3, &sesgo_d, &factor_d);
	printf("sesgo de acelerometro en z : %f \n", sesgo_d);
	printf("factor de escala de acelerometro en z : %f \n\n", factor_d);
}
