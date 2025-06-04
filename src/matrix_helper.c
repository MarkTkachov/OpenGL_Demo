#include "../inc/matrix_helper.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <assert.h>

mat4 *mat4_alloc()
{
    mat4 *m = malloc(MAT4_BYTESIZE);
    memset(m, 0, MAT4_BYTESIZE);
    return m;
}

GLfloat mat4_get(mat4 *m, int x, int y)
{
    assert(x >= 0 && x < 4 && y >= 0 && y < 4);
    assert(m != NULL);

    return m[x * 4 + y];
}

void mat4_set(mat4 *m, GLfloat val, int x, int y)
{
    assert(x >= 0 && x < 4 && y >= 0 && y < 4);
    assert(m != NULL);

    m[x * 4 + y] = val;
}

void mat4_print(mat4 *m) {
    assert(m != NULL);

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            printf("%f ", mat4_get(m, j, i));
        }
        printf("\n");
    }
}

GLfloat mat4_determinant(mat4 *m) {
    assert(m != NULL);

    GLfloat det = 0;
    for (int i = 0; i < 4; i++)
    {
        // create submatrix by removing row 0 and column i
        mat3 submat[MAT3_SIZE];
        memset(submat, 0, MAT3_BYTESIZE);
        // iterate over rows
        for (int j = 1; j < 4; j++)
        {
            // iterate over columns
            for (int k = 0; k < 4; k++)
            {
                
                if (k == i)
                    continue;
                // set value in submatrix
                // if k > i, we need to shift the column index
                // to the left by 1
                mat3_set(submat, mat4_get(m, j, k),  j - 1, k > i ? k - 1 : k);
            }
        }
        // even submatrices are positive, odd submatrices are negative due zero indexing
        det += ((i % 2) ? -1 : 1) * mat4_get(m, 0, i) * mat3_determinant(submat);
    }
    return det;
}

void mat4_inverse(mat4 *out, mat4 *in) {
    assert(out != NULL && in != NULL);

    GLfloat det = mat4_determinant(in);
    if (det == 0)
    {
        printf("Matrix is not invertible\n");
        return;
    }
    mat4 buffer[MAT4_SIZE];
    memset(buffer, 0, MAT4_BYTESIZE);
    // iterate over rows
    for (int i = 0; i < 4; i++)
    {
        // iterate over columns
        for (int j = 0; j < 4; j++)
        {
            // create submatrix by removing row i and column j
            mat3 submat[MAT3_SIZE];
            memset(submat, 0, MAT3_BYTESIZE);
            // iterate over rows
            for (int k = 0; k < 4; k++)
            {
                // iterate over columns
                for (int l = 0; l < 4; l++)
                {
                    if (k == i || l == j)
                        continue;
                    // set value in submatrix
                    // if k > i, we need to shift the row index
                    // to the left by 1
                    mat3_set(submat, mat4_get(in, k, l), k > i ? k - 1 : k,  l > j ? l - 1 : l);
                }
            }
            // even submatrices are positive, odd submatrices are negative due zero indexing
            mat4_set(buffer, ((i + j) % 2 ? -1 : 1) * mat3_determinant(submat) / det, j, i);
        }
    }
    memcpy(out, buffer, MAT4_BYTESIZE);
}

mat3 *mat3_alloc()
{
    mat3 *m = malloc(MAT3_BYTESIZE);
    memset(m, 0, MAT3_BYTESIZE);
    return m;
}

GLfloat mat3_get(mat3 *m, int x, int y)
{
    assert(x >= 0 && x < 3 && y >= 0 && y < 3);

    return m[x * 3 + y];
}

void mat3_set(mat3 *m, GLfloat val, int x, int y)
{
    assert(x >= 0 && x < 3 && y >= 0 && y < 3);

    m[x * 3 + y] = val;
}

void mat3_print(mat3 *m) {
    assert(m != NULL);

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            printf("%f ", mat3_get(m, j, i));
        }
        printf("\n");
    }
}

GLfloat mat3_determinant(mat3 *m) {
    assert(m != NULL);

    return m[0] * m[4] * m[8] + m[1] * m[5] * m[6] + m[2] * m[3] * m[7]
         - m[2] * m[4] * m[6] - m[1] * m[3] * m[8] - m[0] * m[5] * m[7];
}

void mat3_inverse(mat3 *out, mat3 *in) {
    assert(out != NULL && in != NULL);

    GLfloat det = mat3_determinant(in);
    if (det == 0)
    {
        printf("Matrix is not invertible\n");
        return;
    }
    mat3 buffer[MAT3_SIZE];
    memset(buffer, 0, MAT3_BYTESIZE);
    // iterate over rows
    for (int i = 0; i < 3; i++)
    {
        // iterate over columns
        for (int j = 0; j < 3; j++)
        {
            // create submatrix by removing row i and column j
            mat2 submat[MAT2_SIZE];
            memset(submat, 0, MAT2_BYTESIZE);
            // iterate over rows
            for (int k = 0; k < 3; k++)
            {
                // iterate over columns
                for (int l = 0; l < 3; l++)
                {
                    if (k == i || l == j)
                        continue;
                    // set value in submatrix
                    // if k > i, we need to shift the row index
                    // to the left by 1
                    mat2_set(submat, mat3_get(in, k, l), k > i ? k - 1 : k,  l > j ? l - 1 : l);
                }
            }
            // even submatrices are positive, odd submatrices are negative due zero indexing
            mat3_set(buffer, ((i + j) % 2 ? -1 : 1) * mat2_determinant(submat) / det, j, i);
        }
    }
    memcpy(out, buffer, MAT3_BYTESIZE);
}

mat2 *mat2_alloc()
{
    mat2 *m = malloc(MAT2_BYTESIZE);
    memset(m, 0, MAT2_BYTESIZE);
    return m;
}

GLfloat mat2_get(mat2 *m, int x, int y)
{
    assert(x >= 0 && x < 2 && y >= 0 && y < 2);
    assert(m != NULL);
    return m[x * 2 + y];
}

void mat2_set(mat2 *m, GLfloat val, int x, int y)
{
    assert(x >= 0 && x < 2 && y >= 0 && y < 2);

    m[x * 2 + y] = val;
}

void mat2_print(mat2 *m) {
    assert(m != NULL);

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            printf("%f ", mat2_get(m, j, i));
        }
        printf("\n");
    }
}

GLfloat mat2_determinant(mat2 *m) {
    assert(m != NULL);

    return m[0] * m[3] - m[1] * m[2];
}

void mat4_to_mat3(mat3 *out, mat4 *in) {
    assert(out != NULL && in != NULL);

    mat3 buffer[MAT3_SIZE];
    memset(buffer, 0, MAT3_BYTESIZE);
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            mat3_set(buffer, mat4_get(in, i, j), i, j);
        }
    }
    memcpy(out, buffer, MAT3_BYTESIZE);
}

void mat4_multiply(mat4 *out, mat4 *a, mat4 *b)
{
    assert(out != NULL && a != NULL && b != NULL);

    mat4 buffer[MAT4_SIZE];
    memset(buffer, 0, MAT4_BYTESIZE);
    for (int x = 0; x < 4; x++)
    {
        for (int y = 0; y < 4; y++)
        {
            GLfloat val = 0;
            for (int i = 0; i < 4; i++)
            {
                val += mat4_get(a, i, y) * mat4_get(b, x, i);
            }
            mat4_set(buffer, val, x, y);
        }
    }
    memcpy(out, buffer, MAT4_BYTESIZE);
}

vec3 *vec3_alloc() {
    vec3 *v = malloc(VEC3_BYTESIZE);
    memset(v, 0, VEC3_BYTESIZE);
    return v;
}

void vec3_add(vec3 *out, vec3 *a, vec3 *b) {
    assert(out != NULL && a != NULL && b != NULL);

    vec3 v[VEC3_SIZE];
    memset(v, 0, VEC3_BYTESIZE);
    memcpy(v, a, VEC3_BYTESIZE);
    v[0] += b[0];
    v[1] += b[1];
    v[2] += b[2];
    memcpy(out, v, VEC3_BYTESIZE);
}

void vec3_sub(vec3 *out, vec3 *a, vec3 *b) {
    assert(out != NULL && a != NULL && b != NULL);

    vec3 v[VEC3_SIZE];
    memset(v, 0, VEC3_BYTESIZE);
    memcpy(v, a, VEC3_BYTESIZE);
    v[0] -= b[0];
    v[1] -= b[1];
    v[2] -= b[2];
    memcpy(out, v, VEC3_BYTESIZE);
}

vec3 vec3_scalar_mult(vec3 *a, vec3 *b) {
    assert(a != NULL && b != NULL);

    return a[0]*b[0] + a[1]*b[1] + a[2]*b[2];
}

void vec3_cross(vec3 *out, vec3 *a, vec3 *b) {
    assert(out != NULL && a != NULL && b != NULL);

    vec3 v[VEC3_SIZE];
    memset(v, 0, VEC3_BYTESIZE);
    // y1*z2 - z1*y2
    v[0] = a[1] * b[2] - a[2] * b[1];
    // z1*x2 - x1*z2
    v[1] = a[2] * b[0] - a[0] * b[2];
    // x1*y2 - y1*x2
    v[2] = a[0] * b[1] - a[1] * b[0];
    memcpy(out, v, VEC3_BYTESIZE);
}

void vec3_normalize(vec3 *out, vec3 *in) {
    assert(out != NULL && in != NULL);

    double l = sqrt(in[0]*in[0] + in[1]*in[1] + in[2]*in[2]);
    vec3 v[VEC3_SIZE];
    memset(v, 0, VEC3_BYTESIZE);
    memcpy(v, in, VEC3_BYTESIZE);
    v[0] /= l;
    v[1] /= l;
    v[2] /= l;
    memcpy(out, v, VEC3_BYTESIZE);
}

void vec3_component_mult(vec3 *out, vec3 *a, vec3 *b) {
    assert(out != NULL && a != NULL && b != NULL);

    vec3 v[VEC3_SIZE];
    memset(v, 0, VEC3_BYTESIZE);
    memcpy(v, a, VEC3_BYTESIZE);
    v[0] *= b[0];
    v[1] *= b[1];
    v[2] *= b[2];
    memcpy(out, v, VEC3_BYTESIZE);
}

void vec3_print(vec3 *v) {
    assert(v != NULL);

    printf("%f %f %f\n", v[0], v[1], v[2]);
}

vec4 *vec4_alloc() {
    vec4 *v = malloc(VEC4_BYTESIZE);
    memset(v, 0, VEC4_BYTESIZE);
    return v;
}

void vec4_add(vec4 *out, vec4 *a, vec4 *b) {
    assert(out != NULL && a != NULL && b != NULL);

    vec4 v[VEC4_SIZE];
    memset(v, 0, VEC4_BYTESIZE);
    memcpy(v, a, VEC4_BYTESIZE);
    v[0] += b[0];
    v[1] += b[1];
    v[2] += b[2];
    v[3] += b[3];
    memcpy(out, v, VEC4_BYTESIZE);
}

void vec4_sub(vec4 *out, vec4 *a, vec4 *b) {
    assert(out != NULL && a != NULL && b != NULL);

    vec4 v[VEC4_SIZE];
    memset(v, 0, VEC4_BYTESIZE);
    memcpy(v, a, VEC4_BYTESIZE);
    v[0] -= b[0];
    v[1] -= b[1];
    v[2] -= b[2];
    v[3] -= b[3];
    memcpy(out, v, VEC4_BYTESIZE);
}

vec4 vec4_scalar_mult(vec4 *a, vec4 *b) {
    assert(a != NULL && b != NULL);

    return a[0]*b[0] + a[1]*b[1] + a[2]*b[2] + a[3]*b[3];
}


void vec4_normalize(vec4 *out, vec4 *in) {
    assert(out != NULL && in != NULL);

    double l = sqrt(in[0]*in[0] + in[1]*in[1] + in[2]*in[2] + in[3]*in[3]);
    vec4 v[VEC4_SIZE];
    memset(v, 0, VEC4_BYTESIZE);
    memcpy(v, in, VEC4_BYTESIZE);
    v[0] /= l;
    v[1] /= l;
    v[2] /= l;
    v[3] /= l;
    memcpy(out, v, VEC4_BYTESIZE);
}

void vec4_component_mult(vec4 *out, vec4 *a, vec4 *b) {
    assert(out != NULL && a != NULL && b != NULL);

    vec4 v[VEC4_SIZE];
    memset(v, 0, VEC4_BYTESIZE);
    memcpy(v, a, VEC4_BYTESIZE);
    v[0] *= b[0];
    v[1] *= b[1];
    v[2] *= b[2];
    v[3] *= b[3];
    memcpy(out, v, VEC4_BYTESIZE);
}

void vec4_print(vec4 *v) {
    assert(v != NULL);

    printf("%f %f %f %f\n", v[0], v[1], v[2], v[3]);
}

void mat4_transpose(mat4 *out, mat4 *in)
{
    assert(out != NULL && in != NULL);

    mat4 buffer[MAT4_SIZE];
    memset(buffer, 0, MAT4_BYTESIZE);
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            mat4_set(buffer, mat4_get(in, j, i), i, j);
        }
    }
    memcpy(out, buffer, MAT4_BYTESIZE);
}

void mat3_transpose(mat3 *out, mat3 *in)
{
    assert(out != NULL && in != NULL);

    mat3 buffer[MAT3_SIZE];
    memset(buffer, 0, MAT3_BYTESIZE);
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            mat3_set(buffer, mat3_get(in, j, i), i, j);
        }
    }
    memcpy(out, buffer, MAT3_BYTESIZE);
}

void identity(mat4 *out)
{
    assert(out != NULL);

    mat4 buffer[MAT4_SIZE] =
        {1, 0, 0, 0,
         0, 1, 0, 0,
         0, 0, 1, 0,
         0, 0, 0, 1};
    memcpy(out, buffer, MAT4_BYTESIZE);
}

void translate(mat4 *out, mat4 *in, vec3 *v)
{
    assert(out != NULL && in != NULL && v != NULL);

    mat4 buffer[MAT4_SIZE];
    memset(buffer, 0, MAT4_BYTESIZE);
    identity(buffer);
    mat4_set(buffer, v[0], 3, 0);
    mat4_set(buffer, v[1], 3, 1);
    mat4_set(buffer, v[2], 3, 2);
    mat4_multiply(out, in, buffer);
}

void scale(mat4 *out, mat4 *in, vec3 *v) {
    assert(out != NULL && in != NULL && v != NULL);

    mat4 buffer[MAT4_SIZE];
    memset(buffer, 0, MAT4_BYTESIZE);
    identity(buffer);
    mat4_set(buffer, v[0], 0, 0);
    mat4_set(buffer, v[1], 1, 1);
    mat4_set(buffer, v[2], 2, 2);
    mat4_multiply(out, in, buffer);
}

void rotatez(mat4 *out, mat4 *in, GLfloat angle_rad) {
    assert(out != NULL && in != NULL);

    mat4 buffer[MAT4_SIZE];
    memset(buffer, 0, MAT4_BYTESIZE);
    identity(buffer);
    mat4_set(buffer, cos(angle_rad), 0, 0);
    mat4_set(buffer, -sin(angle_rad), 1, 0);
    mat4_set(buffer, sin(angle_rad), 0, 1);
    mat4_set(buffer, cos(angle_rad), 1, 1);
    mat4_multiply(out, in, buffer);
}

void rotatey(mat4 *out, mat4 *in, GLfloat angle_rad) {
    assert(out != NULL && in != NULL);

    mat4 buffer[MAT4_SIZE];
    memset(buffer, 0, MAT4_BYTESIZE);
    identity(buffer);
    mat4_set(buffer, -sin(angle_rad), 0, 2);
    mat4_set(buffer, cos(angle_rad), 0, 0);
    mat4_set(buffer, sin(angle_rad), 2, 0);
    mat4_set(buffer, cos(angle_rad), 2, 2);
    mat4_multiply(out, in, buffer);
}

void rotatex(mat4 *out, mat4 *in, GLfloat angle_rad) {
    assert(out != NULL && in != NULL);

    mat4 buffer[MAT4_SIZE];
    memset(buffer, 0, MAT4_BYTESIZE);
    identity(buffer);
    mat4_set(buffer, cos(angle_rad), 1, 1);
    mat4_set(buffer, -sin(angle_rad), 2, 1);
    mat4_set(buffer, sin(angle_rad), 1, 2);
    mat4_set(buffer, cos(angle_rad), 2, 2);
    mat4_multiply(out, in, buffer);
}

void rotateaxis(mat4 *out, mat4 *in, vec3 *axis, GLfloat angle_rad) {
    assert(out != NULL && in != NULL && axis != NULL);

    mat4 buffer[MAT4_SIZE];
    memset(buffer, 0, MAT4_BYTESIZE);
    identity(buffer);

    vec3 n[VEC3_SIZE];
    vec3_normalize(n, axis);

    GLfloat c = cos(angle_rad);
    GLfloat s = sin(angle_rad);
    GLfloat t = 1 - c;

    mat4_set(buffer, t * n[0] * n[0] + c, 0, 0);
    mat4_set(buffer, t * n[0] * n[1] - s * n[2], 1, 0);
    mat4_set(buffer, t * n[0] * n[2] + s * n[1], 2, 0);
    
    mat4_set(buffer, t * n[1] * n[0] + s * n[2], 0, 1);
    mat4_set(buffer, t * n[1] * n[1] + c, 1, 1);
    mat4_set(buffer, t * n[1] * n[2] - s * n[0], 2, 1);

    mat4_set(buffer, t * n[2] * n[0] - s * n[1], 0, 2);
    mat4_set(buffer, t * n[2] * n[1] + s * n[0], 1, 2);
    mat4_set(buffer, t * n[2] * n[2] + c, 2, 2);

    mat4_multiply(out, in, buffer);
}

void lookAt(mat4 *out, vec3 *eye, vec3 *center, vec3 *up) {
    assert(out != NULL && eye != NULL && center != NULL && up != NULL);

    mat4 m[MAT4_SIZE];
    memset(m, 0, MAT4_BYTESIZE);
    identity(m);

    vec3 n[VEC3_SIZE], v[VEC3_SIZE], u[VEC3_SIZE];
    vec3_sub(n, eye, center);
    vec3_cross(u, up, n);
    vec3_cross(v, n, u);

    vec3_normalize(n, n);
    vec3_normalize(u, u);
    vec3_normalize(v, v);

    //write vectors to matrix
    for (int i = 0; i < VEC3_SIZE; i++)
    {
        mat4_set(m, u[i], i, 0);
        mat4_set(m, v[i], i, 1);
        mat4_set(m, n[i], i, 2);
    }

    mat4_set(m, -vec3_scalar_mult(u, eye), 3, 0);
    mat4_set(m, -vec3_scalar_mult(v, eye), 3, 1);
    mat4_set(m, -vec3_scalar_mult(n, eye), 3, 2);

    memcpy(out, m, MAT4_BYTESIZE);
    
}

void perspective(mat4 *out, GLfloat fovy, GLfloat aspect, GLfloat near, GLfloat far) {
    assert(out != NULL);

    mat4 m[MAT4_SIZE];
    memset(m, 0, MAT4_BYTESIZE);
    GLfloat t = near * tan(fovy/2);
    GLfloat r = t * aspect;

    mat4_set(m, 1/r, 0, 0);
    mat4_set(m, 1/t, 1, 1);
    mat4_set(m, (-1 / near) * ((far + near)/(far - near)), 2, 2);
    mat4_set(m, -1/near, 2, 3);
    mat4_set(m, -2 * far / (far - near), 3, 2);

    memcpy(out, m, MAT4_BYTESIZE);
}
