
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <unistd.h>
 
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

enum colores {rojo, verde, azul};

#define NUM_POINTS 500 // Cambia este valor según la cantidad de puntos que tengas
#define THRESHOLD 1e-16
#define N 340




void dot(SDL_Renderer* rend, int x, int y, enum colores c)
{
	SDL_Rect r;
    	r.x = x;
    	r.y = y;
    	r.w = 5;	/* tamaño del dot (punto) */
    	r.h = 5;

	if (c == rojo)
		SDL_SetRenderDrawColor(rend, 255, 0, 0, SDL_ALPHA_OPAQUE);
	else if (c == verde)
		SDL_SetRenderDrawColor(rend, 0, 255, 0, SDL_ALPHA_OPAQUE);
	else /* if (c == azul) */
		SDL_SetRenderDrawColor(rend, 0, 0, 255, SDL_ALPHA_OPAQUE);

	SDL_RenderFillRect(rend, &r );

}

/* posicion en pixeles del valor 5 en x */
int cen_x = 563;
int cen_y = 507;

void dibujar_obstaculo(SDL_Renderer* rend, int angulo, int radio)
{
	static const double PI = 3.1415926535;
	double x, y;

	radio = radio * 92 /100; /* 92 pixeles es el ancho de un cuadrado en la cuadricula */
	angulo = (-1) * (angulo-180); /* invertimos izq y derecha para que el dibujo coincida */

	x = radio * cos(angulo * PI / 180);
	y = radio * sin(angulo * PI / 180);
	dot(rend, cen_x - x, cen_y - y, rojo);
}



void raw_load(const char *raw_file, double x1[], double y1[]) 
{
        int i;
        char buf[400];

        double n1, n2, n3;
        FILE *ff;

        ff = fopen(raw_file, "r");

        /* remove title line */
        fgets(buf, sizeof(buf), ff);

        i = 0;
        while (fgets(buf, sizeof(buf), ff)) {
                //printf("%s\n", buf);
                sscanf(buf, "[%lf, %lf, %lf]", &n1, &n2, &n3);
                //printf("%lf %lf \n", n1, n2);
                x1[i] = n1 * 1.0 / 12000;
                y1[i] = n2 * 1.0 / 12000;
                //printf("%lf %lf \n", x1[i], y1[i]);
                i++;
                if (i == N)
                        break;
        }

        fclose(ff);
        return;
}



int main(int argc, char *argv[])
{
 
	double x1[N],y1[N],z1[N], dist[N];

	int i, cant;
	double x_tmp;
	double factor;


    // retutns zero on success else non-zero
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("error initializing SDL: %s\n", SDL_GetError());
    }
    SDL_Window* win = SDL_CreateWindow("Lidar Lite sobre SERVO DEMO GRAFICO", // creates a window
                                       SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED,
                                       1000, 1000, 0);
                                       // 1000, 1000, 0);
 
    // triggers the program that controls
    // your graphics hardware and sets flags
    Uint32 render_flags = SDL_RENDERER_ACCELERATED;
 
    // creates a renderer to render our images
    SDL_Renderer* rend = SDL_CreateRenderer(win, -1, render_flags);
 
    // creates a surface to load an image into the main memory
    SDL_Surface* surface;
 
    // please provide a path for your image
    surface = IMG_Load("axis3.bmp");
 
    // loads image to our graphics hardware memory.
    SDL_Texture* tex = SDL_CreateTextureFromSurface(rend, surface);
 
    // clears main-memory
    SDL_FreeSurface(surface);
 
    int close = 0;
 
    // speed of box
    int speed = 300;
 
	double frotate;
        int x,y;
	SDL_GetRendererOutputSize(rend, &x, &y);

	frotate=0.02;
	SDL_RenderCopyEx(rend, tex, NULL, NULL, frotate, NULL, SDL_FLIP_NONE);

	raw_load("mag5.txt", &x1[0], &y1[0]);

	cant = N;
    	 
	/* datos obtenidos desde ellipse_fit */
	double x_offset = -2350.65;
	double y_offset =  5374.13;
	double alfa = 0.0515871;
	double width = 2648.26;
	double height = 2338.17;

	int cx = 500, cy = 500;
	/* dibujamos el centro (coordenadas [0,0]) */
	dot(rend, cx, cy, rojo);

	/* dibujamos la elipse de los datos crudos */
	for (i=0; i<cant; i++) {
		printf("NUEVO %lf, %lf \n", x1[i], y1[i]);
		dot(rend, (int)(x1[i]*500+cx), (int)(y1[i]*500+cy), rojo);
	}


	
	for (i=0; i<cant; i++) {

		/* corregimos el hard iron de cada punto */
		x1[i] = x1[i] - x_offset;
		y1[i] = y1[i] - y_offset;

		/* dibujamos con hard iron corregido */
		dot(rend, (int)(x1[i]*500+cx), (int)(y1[i]*500+cy), rojo);
	}


	printf("ALFA : %lf \n", alfa);
	/* rotamos la elipse con la matriz de rotacion [ECUACION (5) y (6)] */
	for (i=0; i<cant; i++) {
		x_tmp = x1[i] * cosf(alfa) + y1[i] * sinf(alfa);
		y1[i] = (-1) * x1[i] * sinf(alfa) + y1[i] * cosf(alfa);
		x1[i] = x_tmp;

		/* dibujamos cada punto luego de rotarlo */
		dot(rend, (int)(x1[i]*500+cx), (int)(y1[i]*500+cy), azul);
	}

	

	/* entiendo que r y q no cambian [ECUACION (7)] */
	factor = height / width;
	for (i=0; i<cant; i++) {
		x1[i] = x1[i] / factor;
	}

	/* alfa negativo */
	alfa = (-1) * alfa;
	for (i=0; i<cant; i++) {
		x_tmp = x1[i] * cosf(alfa) + y1[i] * sinf(alfa);
		y1[i] = (-1) * x1[i] * sinf(alfa) + y1[i] * cosf(alfa);
		x1[i] = x_tmp;
		dot(rend, (int)(x1[i]*500+cx), (int)(y1[i]*500+cy), verde);
	}

    // annimation loop de la grafica, esto ya no es parte de la calibracion
    while (!close) {
        SDL_Event event;
 
        // Events management
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
 
            case SDL_QUIT:
                // handling of close button
                close = 1;
                break;
 
                default:
                    break;
                }
            }
 
 
        SDL_RenderPresent(rend);

 
        // triggers the double buffers
        // for multiple rendering
 
        // calculates to 60 fps
        SDL_Delay(1000 / 60);
    }
 
    // destroy texture
    SDL_DestroyTexture(tex);
 
    // destroy renderer
    SDL_DestroyRenderer(rend);
 
    // destroy window
    SDL_DestroyWindow(win);
     
    // close SDL
    SDL_Quit();
 
    return 0;
}
