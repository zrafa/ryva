
#include <stdio.h>

#define RAW_FILE_X_UP	"ejex_arriba.txt"
#define RAW_FILE_X_DOWN	"ejex_abajo.txt"
#define RAW_FILE_Y_UP	"ejey_arriba.txt"
#define RAW_FILE_Y_DOWN	"ejey_abajo.txt"
#define RAW_FILE_Z_UP	"ejez_arriba.txt"
#define RAW_FILE_Z_DOWN	"ejez_abajo.txt"

#define gravity ((double) 9.80665)

/* 
 * Promedia una de las columnas de los datos crudos
 * raw file contents:
 * timestamp ax, ay, az, gx, gy, gz
 * n: argument is the column of interest
 */
int raw_mean_from_file(const char *raw_file, int n) 
{
	long long int raw[7];
	int i;
	long long int sum;
	char buf[400];

	FILE *ff;

	ff = fopen(raw_file, "r");

	/* remove title line */
	fgets(buf, sizeof(buf), ff);

	i = 0;
	sum = 0;
	while (fgets(buf, sizeof(buf), ff)) {
		sscanf(buf, "%i %i,%i,%i,%i,%i,%i", &raw[0], &raw[1], &raw[2], &raw[3], &raw[4], &raw[5], &raw[6]);
		sum = sum + raw[n];
		i++;
	}
	printf("%i %i %i\n", sum, i, (sum/i));

	fclose(ff);

	return (sum/i);
}


/* 
 * raw file contents:
 * timestamp ax, ay, az, gx, gy, gz
 * n: argument is the column of interest
 * f arguments are the raw data files
 */
double sesgo(const char *f1, const char *f2, int n)
{
	int fup_x, fdown_x, sesgo_int;
	double sesgo_d;
	fup_x = raw_mean_from_file(f1, n);
	fdown_x = raw_mean_from_file(f2, n);
	
	sesgo_int = (fup_x + fdown_x) / 2; /* ecuacion (2.88), pagina 118, NotasCursoPosicion.pdf */
	sesgo_d = sesgo_int * gravity / 255.0;

	return sesgo_d;
}

void main(void) {
	double sesgo_d;

	sesgo_d = sesgo(RAW_FILE_X_UP, RAW_FILE_X_DOWN, 1);
	printf("sesgo de acelerometro en x : %f \n", sesgo_d);

	sesgo_d = sesgo(RAW_FILE_Y_UP, RAW_FILE_Y_DOWN, 2);
	printf("sesgo de acelerometro en y : %f \n", sesgo_d);

	sesgo_d = sesgo(RAW_FILE_Z_UP, RAW_FILE_Z_DOWN, 3);
	printf("sesgo de acelerometro en z : %f \n", sesgo_d);
}
