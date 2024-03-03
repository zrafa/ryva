
#include <stdio.h>

#define RAW_FILE_X_UP	"ejex_arriba.txt"
#define RAW_FILE_X_DOWN	"ejex_abajo.txt"
#define RAW_FILE_Y_UP	"ejey_arriba.txt"
#define RAW_FILE_Y_DOWN	"ejey_abajo.txt"
#define RAW_FILE_Z_UP	"ejez_arriba.txt"
#define RAW_FILE_Z_DOWN	"ejez_abajo.txt"

void main(void) {
	int n, a, b, c, d, e, f;
	double ad, bd, cd, dd, ed, fd;
	char buf[400];

	FILE *ff;

	ff = fopen("test", "r");
	while (fgets(buf, sizeof(buf), ff)) {
		sscanf(buf, "%i %i,%i,%i,%i,%i,%i", &n, &a, &b, &c, &d, &e, &f);
		printf("%i %i %i  %i  %i  %i \n", a, b, c, d, e, f);
		ad = 1.0 *a;
		bd = 1.0 *b;
		cd = 1.0 *c;
		dd = 1.0 *d;
		ed = 1.0 *e;
		fd = 1.0 *f;
		printf("%f %f %f  %f  %f  %f \n", ad, bd, cd, dd, ed, fd);
	}


}
