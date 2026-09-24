/*
 * calibrar_imu.c
 *
 * Calibracion de acelerometro por ajuste de elipsoide (metodo multi-posicion).
 *
 * Idea: en reposo, el modulo de la lectura de aceleracion (ya calibrada)
 * tiene que dar exactamente 1 g, sin importar la orientacion del sensor.
 * Si se toman lecturas en muchas orientaciones distintas, los puntos
 * (ax,ay,az) crudos caen sobre un elipsoide (por el sesgo y la escala
 * de cada eje). Ajustar ese elipsoide a una esfera de radio 1 da
 * directamente el sesgo y el factor de escala de cada eje.
 *
 * El programa:
 *   1. Lee datos_crudos.txt (timestamp_ms ax ay az gx gy gz por linea).
 *   2. Detecta automaticamente los tramos donde el sensor estuvo quieto
 *      (varianza local del modulo de aceleracion baja), usando el propio
 *      piso de ruido del archivo como referencia -- no hace falta marcar
 *      nada a mano.
 *   3. Promedia cada tramo quieto -> un punto (ax,ay,az) por posicion.
 *   4. Ajusta el elipsoide a esos puntos por minimos cuadrados lineales
 *      (mismo tipo de matematica que la calibracion de magnetometros:
 *      hard iron / soft iron).
 *   5. Imprime sesgo y factor de escala por eje, y una verificacion.
 *
 * Compilar:  gcc -O2 -o calibrar_imu calibrar_imu.c -lm
 * Uso:       ./calibrar_imu [datos_crudos.txt]
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* ---- parametros ajustables ---- */
#define WINDOW_MS           400.0  /* ancho de la ventana para medir varianza local */
#define STATIC_THRESH_MULT  6.0   /* umbral = piso_de_ruido * este factor */
#define MIN_STATIC_MS       300.0 /* duracion minima para contar como "una posicion" */
#define SKIP_FRACTION        0.2  /* se descarta el primer 20% de cada tramo (asentamiento) */

typedef struct {
    double t, ax, ay, az, gx, gy, gz;
} Sample;

typedef struct {
    Sample *data;
    size_t len, cap;
} SampleArray;

typedef struct {
    double ax, ay, az;
    int nmuestras;
} Posicion;

static void sa_push(SampleArray *a, Sample s) {
    if (a->len == a->cap) {
        a->cap = a->cap ? a->cap * 2 : 4096;
        a->data = realloc(a->data, a->cap * sizeof(Sample));
        if (!a->data) { fprintf(stderr, "sin memoria\n"); exit(1); }
    }
    a->data[a->len++] = s;
}

static int leer_datos(const char *ruta, SampleArray *out) {
    FILE *f = fopen(ruta, "r");
    if (!f) { fprintf(stderr, "No pude abrir '%s'\n", ruta); return -1; }
    char linea[512];
    int malformadas = 0;
    while (fgets(linea, sizeof(linea), f)) {
        Sample s;
        int n = sscanf(linea, "%lf %lf %lf %lf %lf %lf %lf",
                        &s.t, &s.ax, &s.ay, &s.az, &s.gx, &s.gy, &s.gz);
        if (n != 7) { malformadas++; continue; }
        sa_push(out, s);
    }
    fclose(f);
    if (malformadas)
        fprintf(stderr, "Aviso: %d linea(s) ignoradas por formato invalido\n", malformadas);
    return 0;
}

static int cmp_double(const void *a, const void *b) {
    double da = *(const double *)a, db = *(const double *)b;
    return (da > db) - (da < db);
}

/* mediana del intervalo entre muestras consecutivas, en ms */
static double estimar_dt_ms(SampleArray *sa) {
    size_t n = sa->len;
    if (n < 2) return 10.0;
    double *dts = malloc((n - 1) * sizeof(double));
    for (size_t i = 1; i < n; i++) dts[i - 1] = sa->data[i].t - sa->data[i - 1].t;
    qsort(dts, n - 1, sizeof(double), cmp_double);
    double mediana = dts[(n - 1) / 2];
    free(dts);
    return (mediana < 1.0) ? 1.0 : mediana;
}

/* varianza local (ventana deslizante) del modulo de aceleracion */
static double *varianza_local(SampleArray *sa, int medio_ancho) {
    size_t n = sa->len;
    double *mod = malloc(n * sizeof(double));
    for (size_t i = 0; i < n; i++) {
        double x = sa->data[i].ax, y = sa->data[i].ay, z = sa->data[i].az;
        mod[i] = sqrt(x * x + y * y + z * z);
    }
    double *var = malloc(n * sizeof(double));
    for (size_t i = 0; i < n; i++) {
        long lo = (long)i - medio_ancho; if (lo < 0) lo = 0;
        long hi = (long)i + medio_ancho; if (hi >= (long)n) hi = (long)n - 1;
        int cnt = (int)(hi - lo + 1);
        double media = 0;
        for (long k = lo; k <= hi; k++) media += mod[k];
        media /= cnt;
        double v = 0;
        for (long k = lo; k <= hi; k++) { double d = mod[k] - media; v += d * d; }
        var[i] = v / cnt;
    }
    free(mod);
    return var;
}

/* detecta tramos quietos y devuelve un punto (promedio) por cada uno */
static int detectar_posiciones(SampleArray *sa, Posicion **out) {
    size_t n = sa->len;
    double dt_ms = estimar_dt_ms(sa);
    int medio_ancho = (int)lround((WINDOW_MS / 2.0) / dt_ms);
    if (medio_ancho < 3) medio_ancho = 3;

    double *var = varianza_local(sa, medio_ancho);
    double minvar = var[0];
    for (size_t i = 1; i < n; i++) if (var[i] < minvar) minvar = var[i];
    double umbral = minvar * STATIC_THRESH_MULT;
    if (umbral <= 0) umbral = 1e-6;

    char *quieto = malloc(n);
    for (size_t i = 0; i < n; i++) quieto[i] = (var[i] <= umbral) ? 1 : 0;
    free(var);

    int min_muestras = (int)lround(MIN_STATIC_MS / dt_ms);
    if (min_muestras < 5) min_muestras = 5;

    Posicion *pos = NULL;
    int cap = 0, count = 0;
    size_t i = 0;
    while (i < n) {
        if (quieto[i]) {
            size_t j = i;
            while (j < n && quieto[j]) j++;
            int largo = (int)(j - i);
            if (largo >= min_muestras) {
                size_t salto = (size_t)(largo * SKIP_FRACTION);
                size_t desde = i + salto, hasta = j;
                double sx = 0, sy = 0, sz = 0; int cnt = 0;
                for (size_t k = desde; k < hasta; k++) {
                    sx += sa->data[k].ax; sy += sa->data[k].ay; sz += sa->data[k].az;
                    cnt++;
                }
                if (cnt > 0) {
                    if (count == cap) { cap = cap ? cap * 2 : 16; pos = realloc(pos, cap * sizeof(Posicion)); }
                    pos[count].ax = sx / cnt; pos[count].ay = sy / cnt; pos[count].az = sz / cnt;
                    pos[count].nmuestras = cnt;
                    count++;
                }
            }
            i = j;
        } else {
            i++;
        }
    }
    free(quieto);
    *out = pos;
    return count;
}

/* resuelve A*x = b, sistema de 6x6, por eliminacion gaussiana con pivoteo */
static int resolver6(double A[6][6], double b[6], double x[6]) {
    for (int col = 0; col < 6; col++) {
        int piv = col;
        double mejor = fabs(A[col][col]);
        for (int r = col + 1; r < 6; r++)
            if (fabs(A[r][col]) > mejor) { mejor = fabs(A[r][col]); piv = r; }
        if (mejor < 1e-12) return -1;
        if (piv != col) {
            for (int c = 0; c < 6; c++) { double t = A[col][c]; A[col][c] = A[piv][c]; A[piv][c] = t; }
            double t = b[col]; b[col] = b[piv]; b[piv] = t;
        }
        for (int r = col + 1; r < 6; r++) {
            double f = A[r][col] / A[col][col];
            for (int c = col; c < 6; c++) A[r][c] -= f * A[col][c];
            b[r] -= f * b[col];
        }
    }
    for (int r = 5; r >= 0; r--) {
        double s = b[r];
        for (int c = r + 1; c < 6; c++) s -= A[r][c] * x[c];
        x[r] = s / A[r][r];
    }
    return 0;
}

/* ajusta el elipsoide A x^2+B y^2+C z^2+D x+E y+F z = 1 y extrae sesgo/escala */
static int ajustar_elipsoide(Posicion *p, int n, double sesgo[3], double escala[3]) {
    if (n < 6) {
        fprintf(stderr, "Hacen falta al menos 6 posiciones distintas (se detectaron %d)\n", n);
        return -1;
    }
    double AtA[6][6]; double Atb[6];
    memset(AtA, 0, sizeof(AtA));
    memset(Atb, 0, sizeof(Atb));
    for (int i = 0; i < n; i++) {
        double x = p[i].ax, y = p[i].ay, z = p[i].az;
        double fila[6] = { x * x, y * y, z * z, x, y, z };
        for (int r = 0; r < 6; r++) {
            for (int c = 0; c < 6; c++) AtA[r][c] += fila[r] * fila[c];
            Atb[r] += fila[r];
        }
    }
    double sol[6];
    if (resolver6(AtA, Atb, sol) != 0) {
        fprintf(stderr, "Sistema mal condicionado: las posiciones estan muy agrupadas\n");
        return -1;
    }
    double A = sol[0], B = sol[1], C = sol[2], D = sol[3], E = sol[4], F = sol[5];
    if (A <= 0 || B <= 0 || C <= 0) {
        fprintf(stderr, "Ajuste invalido: probar con mas posiciones o mas variadas\n");
        return -1;
    }
    escala[0] = 1.0 / sqrt(A); escala[1] = 1.0 / sqrt(B); escala[2] = 1.0 / sqrt(C);
    sesgo[0] = -D / (2 * A);   sesgo[1] = -E / (2 * B);   sesgo[2] = -F / (2 * C);
    return 0;
}

int main(int argc, char **argv) {
    const char *ruta = (argc > 1) ? argv[1] : "datos_crudos.txt";

    SampleArray sa; sa.data = NULL; sa.len = 0; sa.cap = 0;
    if (leer_datos(ruta, &sa) != 0) return 1;
    printf("Muestras leidas de '%s': %zu\n", ruta, sa.len);
    if (sa.len < 50) {
        fprintf(stderr, "Muy pocas muestras validas.\n");
        free(sa.data);
        return 1;
    }

    Posicion *pos;
    int npos = detectar_posiciones(&sa, &pos);
    printf("Posiciones estaticas detectadas: %d\n", npos);
    for (int i = 0; i < npos; i++)
        printf("  %2d) ax=%8.2f  ay=%8.2f  az=%8.2f   (%d muestras)\n",
               i + 1, pos[i].ax, pos[i].ay, pos[i].az, pos[i].nmuestras);
    if (npos < 9)
        fprintf(stderr, "Aviso: con pocas posiciones el ajuste es menos robusto; "
                         "conviene 9-12 o mas, bien variadas.\n");

    double sesgo[3], escala[3];
    if (ajustar_elipsoide(pos, npos, sesgo, escala) != 0) {
        free(pos); free(sa.data);
        return 1;
    }

    printf("\nResultado (sesgo en cuentas crudas, escala en cuentas por g):\n");
    printf("  eje X:  sesgo = %9.3f    factor_escala = %9.3f\n", sesgo[0], escala[0]);
    printf("  eje Y:  sesgo = %9.3f    factor_escala = %9.3f\n", sesgo[1], escala[1]);
    printf("  eje Z:  sesgo = %9.3f    factor_escala = %9.3f\n", sesgo[2], escala[2]);

    printf("\nVerificacion (deberia dar ~1.000 en cada posicion):\n");
    for (int i = 0; i < npos; i++) {
        double cx = (pos[i].ax - sesgo[0]) / escala[0];
        double cy = (pos[i].ay - sesgo[1]) / escala[1];
        double cz = (pos[i].az - sesgo[2]) / escala[2];
        double mod = sqrt(cx * cx + cy * cy + cz * cz);
        printf("  %2d) |a| calibrado = %.4f g\n", i + 1, mod);
    }

    free(pos);
    free(sa.data);
    return 0;
}
