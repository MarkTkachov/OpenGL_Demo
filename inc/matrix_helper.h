#ifndef MATRIX_HELPER_H
#define MATRIX_HELPER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

typedef GLfloat mat4;
typedef GLfloat mat3;
typedef GLfloat mat2;
typedef GLfloat vec3;
typedef GLfloat vec4;

//size of mat4 in GLfloats
#define MAT4_SIZE 16
//size of mat4 in bytes
#define MAT4_BYTESIZE (sizeof(mat4)*MAT4_SIZE)

#define MAT3_SIZE 9
#define MAT3_BYTESIZE (sizeof(mat3)*MAT3_SIZE)

#define MAT2_SIZE 4
#define MAT2_BYTESIZE (sizeof(mat2)*MAT2_SIZE)

#define VEC3_SIZE 3
#define VEC3_BYTESIZE (sizeof(vec3)*VEC3_SIZE)

#define VEC4_SIZE 4
#define VEC4_BYTESIZE (sizeof(vec4)*VEC4_SIZE)



mat4 *mat4_alloc();
GLfloat mat4_get(mat4 *m, int x, int y);
void mat4_set(mat4 *m, GLfloat val, int x, int y);
void mat4_print(mat4 *m);
GLfloat mat4_determinant(mat4 *m);
void mat4_inverse(mat4 *out, mat4 *in);

mat3 *mat3_alloc();
GLfloat mat3_get(mat3 *m, int x, int y);
void mat3_set(mat3 *m, GLfloat val, int x, int y);
void mat3_print(mat3 *m);
GLfloat mat3_determinant(mat3 *m);
void mat3_inverse(mat3 *out, mat3 *in);

mat2 *mat2_alloc();
GLfloat mat2_get(mat2 *m, int x, int y);
void mat2_set(mat2 *m, GLfloat val, int x, int y);
void mat2_print(mat2 *m);
GLfloat mat2_determinant(mat2 *m);

void mat4_to_mat3(mat3 *out, mat4 *in);

vec3 *vec3_alloc();
// out = a + b
void vec3_add(vec3 *out, vec3 *a, vec3 *b);
// out = a - b
void vec3_sub(vec3 *out, vec3 *a, vec3 *b);
// scalar product of vectors a and b
// returns a*b
vec3 vec3_scalar_mult(vec3 *a, vec3 *b);
// cross product of a and b
// out = a x b
void vec3_cross(vec3 *out, vec3 *a, vec3 *b);
// normalize vector in
// out = in / |in|
void vec3_normalize(vec3 *out, vec3 *in);

void vec3_component_mult(vec3 *out, vec3 *a, vec3 *b);

void vec3_print(vec3 *v);

vec4 *vec4_alloc();
// out = a + b
void vec4_add(vec4 *out, vec4 *a, vec4 *b);
// out = a - b
void vec4_sub(vec4 *out, vec4 *a, vec4 *b);
// scalar product of vectors a and b
// returns a*b
vec4 vec4_scalar_mult(vec4 *a, vec4 *b);
// normalize vector in
// out = in / |in|
void vec4_normalize(vec4 *out, vec4 *in);

void vec4_component_mult(vec4 *out, vec4 *a, vec4 *b);

void vec4_print(vec4 *v);

// transpose matrix - write rows as columns
void mat4_transpose(mat4 *out, mat4 *in);
void mat3_transpose(mat3 *out, mat3 *in);
// out = a * b
void mat4_multiply(mat4 *out, mat4 *a, mat4 *b);
// write identity matrix to out - 1's on the diagonal, 0's everywhere else
void identity(mat4 *out);
// out = in * Translation_Matrix(v)
void translate(mat4 *out, mat4 *in, vec3 *v);
// out = in * Scale_Matrix(v)
void scale(mat4 *out, mat4 *in, vec3 *v);
// out = in * Rotation_Matrix_Z(angle_rad)
void rotatez(mat4 *out, mat4 *in, GLfloat angle_rad);
// out = in * Rotation_Matrix_Y(angle_rad)
void rotatey(mat4 *out, mat4 *in, GLfloat angle_rad);
// out = in * Rotation_Matrix_X(angle_rad)
void rotatex(mat4 *out, mat4 *in, GLfloat angle_rad);
// out = in * Rotation_Matrix_Around_Axis(axis, angle_rad)
void rotateaxis(mat4 *out, mat4 *in, vec3 *axis, GLfloat angle_rad);

// view matrix for given eye position, center(look) and up direction
// result is written to out
void lookAt(mat4 *out, vec3 *eye, vec3 *center, vec3 *up);

void perspective(mat4 *out, GLfloat fovy, GLfloat aspect, GLfloat near, GLfloat far);

#endif
