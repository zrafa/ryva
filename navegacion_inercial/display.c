
#include <GL/glut.h>
#include <pthread.h>
#include <unistd.h>
#include <math.h>
#include "matrix.h"

// --- Variables globales compartidas ---
extern Matrix* C_ib;
extern pthread_mutex_t lock;

// --- Prototipos ---
void draw_body_frame(const Matrix* C_ib);
void* actualizar(void* arg);
void display(void);

void initOpenGL(void)
{
    glClearColor(0.1, 0.1, 0.1, 1.0);  // fondo gris oscuro
    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, 1.0, 0.1, 10.0);  // FOV 60°, z-near=0.1, z-far=10

    glMatrixMode(GL_MODELVIEW);
}


// Dibuja la caja con la orientación de C_ib
void draw_body_frame(const Matrix* C_ib)
{
    glPushMatrix();

    // Matriz 4x4 para OpenGL (column-major)
    double M[16] = {
        M_get(C_ib, 0, 0), M_get(C_ib, 1, 0), M_get(C_ib, 2, 0), 0.0,
        M_get(C_ib, 0, 1), M_get(C_ib, 1, 1), M_get(C_ib, 2, 1), 0.0,
        M_get(C_ib, 0, 2), M_get(C_ib, 1, 2), M_get(C_ib, 2, 2), 0.0,
        0.0,                0.0,                0.0,                1.0
    };

    glMultMatrixd(M);
    glColor3f(1.0, 1.0, 1.0);
    glutWireCube(1.0);

    glPopMatrix();
}


// Thread que actualiza C_ib cada segundo
/*
void* actualizar(void* arg)
{
    double t = 0.0;

    while (1) {
        // Calcula una rotación lenta alrededor del eje Z
        double phi = 0.0;             // roll
        double theta = 0.0;           // pitch
        double psi = t;               // yaw cambia con el tiempo

        double cphi = cos(phi), sphi = sin(phi);
        double cth = cos(theta), sth = sin(theta);
        double cpsi = cos(psi), spsi = sin(psi);

        Matrix* nueva = M_create(3,3);
        // Matriz C_ib (ZYX)
        M_set(nueva, 0,0, cpsi*cth);
        M_set(nueva, 0,1, cpsi*sth*sphi - spsi*cphi);
        M_set(nueva, 0,2, cpsi*sth*cphi + spsi*sphi);
        M_set(nueva, 1,0, spsi*cth);
        M_set(nueva, 1,1, spsi*sth*sphi + cpsi*cphi);
        M_set(nueva, 1,2, spsi*sth*cphi - cpsi*sphi);
        M_set(nueva, 2,0, -sth);
        M_set(nueva, 2,1, cth*sphi);
        M_set(nueva, 2,2, cth*cphi);

        pthread_mutex_lock(&lock);
        M_free(C_ib);
        C_ib = nueva;
        pthread_mutex_unlock(&lock);

        glutPostRedisplay();  // pide redibujar

        t += 0.1;  // avanza el ángulo
        sleep(1);
    }
    return NULL;
}
*/


// Dibuja la escena actual
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(2.0, 2.0, 2.0,   // cámara
              0.0, 0.0, 0.0,   // mira al origen
              0.0, 0.0, 1.0);  // "up"

    pthread_mutex_lock(&lock);
    draw_body_frame(C_ib);
    pthread_mutex_unlock(&lock);

    glutSwapBuffers();
}

void display_redraw()
{
	glutPostRedisplay();
}

void display_init(int argc, char** argv)
{
    // pthread_mutex_init(&lock, NULL);

    // Inicializa matriz identidad
    // C_ib = M_identity(3);

    // Inicializa OpenGL
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Visualización actitud C_ib");

    glEnable(GL_DEPTH_TEST);

    glutDisplayFunc(display);
    glutIdleFunc(display);

    // Inicia thread que actualiza C_ib
    //pthread_t th;
    //pthread_create(&th, NULL, actualizar, NULL);

    initOpenGL();
}

void display_loop()
{
    glutMainLoop();  // bloquea aquí (thread sigue corriendo)
}

