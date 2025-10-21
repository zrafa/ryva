
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


/*
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
*/


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

    // --- Cara superior (Z = +0.5) ---
    glColor3f(0.2f, 0.6f, 1.0f); // azul claro
    glBegin(GL_QUADS);
        glVertex3f(-0.5f, -0.5f,  0.5f);
        glVertex3f( 0.5f, -0.5f,  0.5f);
        glVertex3f( 0.5f,  0.5f,  0.5f);
        glVertex3f(-0.5f,  0.5f,  0.5f);
    glEnd();

    // --- Cara lateral (X = +0.5) ---
    glColor3f(1.0f, 0.3f, 0.3f); // rojo suave
    glBegin(GL_QUADS);
        glVertex3f( 0.5f, -0.5f, -0.5f);
        glVertex3f( 0.5f,  0.5f, -0.5f);
        glVertex3f( 0.5f,  0.5f,  0.5f);
        glVertex3f( 0.5f, -0.5f,  0.5f);
    glEnd();

    // --- Wireframe del cubo ---
    glColor3f(1.0f, 1.0f, 1.0f);
    glutWireCube(1.0);

    glPopMatrix();
}





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

