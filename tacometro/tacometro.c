
/* tacometro.c
 *
 * Aprende dos mesetas (UP y DOWN) de un sensor y detecta
 * transiciones entre ellas. Las medias se adaptan continuamente
 * a la deriva del sensor.
 */

#include <stdint.h>

/* ===================== Configuración ===================== */

/* Rango útil del sensor. Fuera de esto se descarta la muestra. */
#define X_MIN_VALIDO     0       /* ecos inválidos / ruido suelen dar 0 */
#define X_MAX_VALIDO     4000     /* ajustar según sensor */

/* Separación mínima entre mesetas para considerar la calibración hecha. */
#define SEP_MINIMA       20

/* Peso de cada muestra nueva (0.01 = lento/estable, 0.1 = rápido/ruidoso).
 * Memoria efectiva ≈ 1/ALPHA muestras. */
#define ALPHA            0.05

/* Fracción central entre las dos mesetas que NO entrena a ninguna.
 * Evita que las muestras de transición arrastren las medias al medio. */
#define FRACC_MUERTA     0.30

/* ===================== Estado interno ===================== */

double   media_up   = 0.0;
double   media_down = 0.0;

int     inicializado = 0;
int     calibrada    = 0;

uint8_t         estado = 0;              /* 0 = DOWN, 1 = UP */
static uint16_t confirmaciones = 0;

static inline int x_valido(uint16_t x)
{
    return (x >= X_MIN_VALIDO) && (x <= X_MAX_VALIDO);
}

void calibrar(uint16_t x)
{
    if (!x_valido(x))
        return;

    /* Primera muestra válida: las dos medias arrancan ahí. */
    if (!inicializado) {
        media_up     = (double)x;
        media_down   = (double)x;
        inicializado = 1;
        return;
    }

    /*
     * Fase de descubrimiento:
     * Mientras las dos mesetas no estén claramente separadas, cada una
     * persigue su extremo (la de arriba el máximo, la de abajo el mínimo).
     * En cuanto la separación supera SEP_MINIMA, pasamos a fase adaptativa.
     */
    if (!calibrada) {
        if ((double)x > media_up) {
            media_up += ALPHA * ((double)x - media_up);
        }
        else if ((double)x < media_down) {
            media_down += ALPHA * ((double)x - media_down);
        }

        if ((media_up - media_down) >= (double)SEP_MINIMA) {
            calibrada = 1;
        }
        return;
    }

    /*
     * Fase adaptativa:
     * Cada muestra entrena la meseta más cercana, excepto si cae en la
     * zona muerta central (transición entre estados). Eso mantiene a las
     * medias pegadas a sus respectivas mesetas aunque el sensor derive.
     */
    double sep    = media_up - media_down;
    double t_bajo = media_down + sep * (FRACC_MUERTA * 0.5);
    double t_alto = media_up   - sep * (FRACC_MUERTA * 0.5);

    if ((double)x >= t_alto) {
        media_up += ALPHA * ((double)x - media_up);
    }
    else if ((double)x <= t_bajo) {
        media_down += ALPHA * ((double)x - media_down);
    }
    /* zona muerta: se ignora para el aprendizaje */
}

uint8_t detectar_taco(uint16_t x, uint16_t muestras_necesarias)
{
    if (!calibrada || !x_valido(x))
        return estado;

    double  limite    = (media_up + media_down) * 0.5;
    uint8_t candidato = ((double)x >= limite) ? 1 : 0;

    if (candidato != estado) {
        if (++confirmaciones >= muestras_necesarias) {
            estado         = candidato;
            confirmaciones = 0;
        }
    } else {
        confirmaciones = 0;
    }
    return estado;
}

