
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <unistd.h>
 
enum colores {rojo, verde, azul};

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
	double x1, y1;

	radio = radio * 92 /100; /* 92 pixeles es el ancho de un cuadrado en la cuadricula */
	angulo = (-1) * (angulo-180); /* invertimos izq y derecha para que el dibujo coincida */

	x1 = radio * cos(angulo * PI / 180);
	y1 = radio * sin(angulo * PI / 180);
	dot(rend, cen_x - x1, cen_y - y1, rojo);
}



int main(int argc, char *argv[])
{
 
	int x2,y2,z2;
	float x1[1000],y1[1000],z1[1000];
	float r2, max_r2, max_r2_y1, min_r2;
	float alfa;
	float factor;


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
    surface = IMG_Load("axis.bmp");
 
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

 char *line = NULL;
 char *bkp = NULL;
 char *bkp2 = NULL;
line = malloc(80);
bkp2 = line;
bkp = malloc(80);

 //char line[80];
  size_t len = 0;
  ssize_t lineSize = 0;

float min_x = 10000;
float max_x = -10000;
float min_y = 10000;
float max_y = -10000;
float x_offset, y_offset;

	frotate=0.02;
	SDL_RenderCopyEx(rend, tex, NULL, NULL, frotate, NULL, SDL_FLIP_NONE);

	lineSize = 10;
	int i = 0;
	int cant = 0;

	/* cargamos los datos crudos del sensor y calculamos el offset */
	while ((i<1000) && (lineSize != -1)) {

		line = bkp2;
		lineSize = getline(&line, &len, stdin);
	
		printf("linesize: %i - linea: %s \n", lineSize, line);

		/* si fin de archivo calculamos el offset */
		if (lineSize < 1) { 
			x_offset = (max_x + min_x) / 2;
			y_offset = (max_y + min_y) / 2;
			printf("x_offset:%f, y_offset:%f \n", x_offset, y_offset);
			break;
		}

		if (*line != '[') {sleep(20); continue; }
		if (strlen(line) < 8) continue;
	
		bkp=line+1;
		while(*line != ',') line++;
		*line = '\0';
		x2=atoi(bkp);
		bkp=line+1;
		while(*line != ',') line++;
		*line = '\0';
		y2=atoi(bkp);
		bkp=line+1;
		while(*line != ']') line++;
		*line = '\0';
		z2=atoi(bkp);
		printf("x: %i, y: %i, z: %i \n", x2, y2, z2);
		
		/* pasamos a gauss cada dato crudo */
		x1[i] = (x2 * 1.0) / 12000;
		y1[i] = (y2 * 1.0) / 12000;
		z1[i] = (z2 * 1.0) / 12000;
	
		/* min y max */
		if (x1[i]<min_x) min_x = x1[i];
		if (x1[i]>max_x) max_x = x1[i];
		if (y1[i]<min_y) min_y = y1[i];
		if (y1[i]>max_y) max_y = y1[i];
	
		printf("x1f: %f, y1f: %f, z1f: %f \n", x1[i], y1[i], z1[i]);
		i++;
	}
	cant = i;
    	 
	/* dibujamos el centro (coordenadas [0,0]) */
	dot(rend, 500, 300, rojo);

	/* dibujamos la elipse de los datos crudos */
	for (i=0; i<cant; i++) {
		dot(rend, (int)(x1[i]*500+500), (int)(y1[i]*500+300), rojo);
		printf("x1: %i, z1: %i \n", (int)(x1[i]*500+500), (int)(y1[i]*500+300));
	}

	max_r2 = 0;
	min_r2 = 10000;
	for (i=0; i<cant; i++) {

		/* corregimos el hard iron de cada punto */
		x1[i] = x1[i] - x_offset;
		y1[i] = y1[i] - y_offset;

		/* distancia desde el centro al punto [ECUACION (3) ] */
		r2 = sqrtf(powf(x1[i],2) + powf(y1[i],2));
		/* nos quedamos con el mayor y el menor (r y q) */
		if (r2 > max_r2) {
			max_r2 = r2;
			max_r2_y1 = y1[i];
		} else if (r2 < min_r2) {
			min_r2 = r2;
		};

		/* dibujamos con hard iron corregido */
		dot(rend, (int)(x1[i]*500+500), (int)(y1[i]*500+300), rojo);
		printf("x1: %i, z1: %i \n", (int)(x1[i]*500+500), (int)(y1[i]*500+300));
	}
	alfa = asinf((max_r2_y1 / max_r2));

	/* rotamos la elipse con la matriz de rotacion [ECUACION (5) y (6)] */
	for (i=0; i<cant; i++) {
		x1[i] = x1[i] * cosf(alfa) + y1[i] * sinf(alfa);
		y1[i] = (-1) * x1[i] * sinf(alfa) + y1[i] * cosf(alfa);
		/* dibujamos cada punto luego de rotarlo */
		dot(rend, (int)(x1[i]*500+500), (int)(y1[i]*500+300), azul);
	}

	/* entiendo que r y q no cambian [ECUACION (7)] */
	factor = min_r2 / max_r2;
	for (i=0; i<cant; i++) {
		x1[i] = x1[i] / factor;
	}

	/* alfa negativo */
	alfa = (-1) * alfa;
	for (i=0; i<cant; i++) {
		x1[i] = x1[i] * cosf(alfa) + y1[i] * sinf(alfa);
		y1[i] = (-1) * x1[i] * sinf(alfa) + y1[i] * cosf(alfa);
		dot(rend, (int)(x1[i]*500+500), (int)(y1[i]*500+300), verde);
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
 
 
  	// SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);
        // clears the screen
        // SDL_RenderClear(rend);



        SDL_RenderPresent(rend);

 
        // triggers the double buffers
        // for multiple rendering
 
        // calculates to 60 fps
        SDL_Delay(1000 / 60);
	//sleep(20);
	//break;
    }
 
  free(line);
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
