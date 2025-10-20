
#ifndef MATRIX_H
#define MATRIX_H

#include <stdlib.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

typedef struct {
    size_t rows;
    size_t cols;
    double* data;
} Matrix;

// Creación y destrucción
Matrix* M_create(size_t rows, size_t cols);
void M_free(Matrix* m);

// Acceso a elementos
double M_get(const Matrix* m, size_t row, size_t col);
void M_set(Matrix* m, size_t row, size_t col, double value);

// Operaciones básicas
Matrix* M_mult(const Matrix* a, const Matrix* b);
Matrix* M_add(const Matrix* a, const Matrix* b);
Matrix* M_sub(const Matrix* a, const Matrix* b);
void M_scale(Matrix* m, double scalar);

// Utilidades
Matrix* M_zero(size_t rows, size_t cols);
Matrix* M_identity(size_t n);
void M_print(const Matrix* m);

// Operaciones de transposición
Matrix* M_transpose(const Matrix* m);

// Operaciones de rotación (2D)
Matrix* M_rotation_2d(double angle_radians);
void M_apply_rotation_2d(Matrix* points, double angle_radians); // Para matrices de puntos 2D
								     //

Matrix* M_transpose_gsl(const Matrix* m);
Matrix* M_rotation_3d_x(double angle_radians);


#endif
