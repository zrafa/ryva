
#include "matrix.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <gsl/gsl_blas.h>

Matrix* M_create(size_t rows, size_t cols) {
    Matrix* m = (Matrix*)malloc(sizeof(Matrix));
    if (!m) return NULL;

    m->rows = rows;
    m->cols = cols;
    m->data = (double*)calloc(rows * cols, sizeof(double));

    if (!m->data) {
        free(m);
        return NULL;
    }

    return m;
}

void M_free(Matrix* m) {
    if (m) {
        free(m->data);
        free(m);
    }
}

double M_get(const Matrix* m, size_t row, size_t col) {
    return m->data[row * m->cols + col];
}

void M_set(Matrix* m, size_t row, size_t col, double value) {
    m->data[row * m->cols + col] = value;
}

Matrix* M_mult(const Matrix* a, const Matrix* b) {
    if (a->cols != b->rows) {
        fprintf(stderr, "Error: Dimensiones incompatibles para multiplicación\n");
        return NULL;
    }

    Matrix* result = M_create(a->rows, b->cols);
    if (!result) return NULL;

    // Usamos GSL internamente para mejor rendimiento
    gsl_matrix_view A = gsl_matrix_view_array(a->data, a->rows, a->cols);
    gsl_matrix_view B = gsl_matrix_view_array(b->data, b->rows, b->cols);
    gsl_matrix_view C = gsl_matrix_view_array(result->data, result->rows, result->cols);

    gsl_blas_dgemm(CblasNoTrans, CblasNoTrans, 1.0, &A.matrix, &B.matrix, 0.0, &C.matrix);

    return result;
}

Matrix* M_add(const Matrix* a, const Matrix* b) {
    if (a->rows != b->rows || a->cols != b->cols) {
        fprintf(stderr, "Error: Dimensiones incompatibles para suma\n");
        return NULL;
    }

    Matrix* result = M_create(a->rows, a->cols);
    if (!result) return NULL;

    for (size_t i = 0; i < a->rows * a->cols; i++) {
        result->data[i] = a->data[i] + b->data[i];
    }

    return result;
}

Matrix* M_sub(const Matrix* a, const Matrix* b) {
    if (a->rows != b->rows || a->cols != b->cols) {
        fprintf(stderr, "Error: Dimensiones incompatibles para resta\n");
        return NULL;
    }

    Matrix* result = M_create(a->rows, a->cols);
    if (!result) return NULL;

    for (size_t i = 0; i < a->rows * a->cols; i++) {
        result->data[i] = a->data[i] - b->data[i];
    }

    return result;
}

void M_scale(Matrix* m, double scalar) {
    for (size_t i = 0; i < m->rows * m->cols; i++) {
        m->data[i] *= scalar;
    }
}

Matrix* M_zero(size_t rows, size_t cols) {
    return M_create(rows, cols); // calloc ya inicializa a cero
}

Matrix* M_identity(size_t n) {
    Matrix* m = M_zero(n, n);
    if (!m) return NULL;

    for (size_t i = 0; i < n; i++) {
        M_set(m, i, i, 1.0);
    }

    return m;
}

void M_print(const Matrix* m) {
    for (size_t i = 0; i < m->rows; i++) {
        for (size_t j = 0; j < m->cols; j++) {
            printf("%8.3f ", M_get(m, i, j));
        }
        printf("\n");
    }
}

Matrix* M_transpose(const Matrix* m) {
    Matrix* result = M_create(m->cols, m->rows);
    if (!result) return NULL;

    for (size_t i = 0; i < m->rows; i++) {
        for (size_t j = 0; j < m->cols; j++) {
            M_set(result, j, i, M_get(m, i, j));
        }
    }
    return result;
}

Matrix* M_rotation_2d(double angle_radians) {
    Matrix* rot = M_create(2, 2);
    if (!rot) return NULL;

    double c = cos(angle_radians);
    double s = sin(angle_radians);

    M_set(rot, 0, 0, c);
    M_set(rot, 0, 1, -s);
    M_set(rot, 1, 0, s);
    M_set(rot, 1, 1, c);

    return rot;
}

void M_apply_rotation_2d(Matrix* points, double angle_radians) {
    if (points->cols != 2) {
        fprintf(stderr, "Error: La matriz debe contener puntos 2D (Nx2)\n");
        return;
    }

    Matrix* rot = M_rotation_2d(angle_radians);
    if (!rot) return;

    // Rotar cada punto
    for (size_t i = 0; i < points->rows; i++) {
        double x = M_get(points, i, 0);
        double y = M_get(points, i, 1);

        M_set(points, i, 0, x * M_get(rot, 0, 0) + y * M_get(rot, 0, 1));
        M_set(points, i, 1, x * M_get(rot, 1, 0) + y * M_get(rot, 1, 1));
    }

    M_free(rot);
}

Matrix* M_transpose_gsl(const Matrix* m) {
    Matrix* result = M_create(m->cols, m->rows);
    if (!result) return NULL;

    gsl_matrix_view src = gsl_matrix_view_array(m->data, m->rows, m->cols);
    gsl_matrix_view dst = gsl_matrix_view_array(result->data, result->rows, result->cols);

    gsl_matrix_transpose_memcpy(&dst.matrix, &src.matrix);
    return result;
}


Matrix* M_rotation_3d_x(double angle_radians) {
    Matrix* rot = M_create(3, 3);
    double c = cos(angle_radians);
    double s = sin(angle_radians);

    M_set(rot, 0, 0, 1); M_set(rot, 0, 1, 0); M_set(rot, 0, 2, 0);
    M_set(rot, 1, 0, 0); M_set(rot, 1, 1, c); M_set(rot, 1, 2, -s);
    M_set(rot, 2, 0, 0); M_set(rot, 2, 1, s); M_set(rot, 2, 2, c);

    return rot;
}

// Similar para M_rotation_3d_y y M_rotation_3d_z
