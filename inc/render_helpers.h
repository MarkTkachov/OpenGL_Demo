#ifndef _RENDER_HELPERS_H
#define _RENDER_HELPERS_H

#include <data_structs.h>
#include <matrix_helper.h>

extern mat4 projMatrix[MAT4_SIZE];
extern mat4 viewMatrix[MAT4_SIZE];

void use_object_program(Object3D *object);
void render_object(Object3D *object);
void calculate_projection_matrix(float fov, float aspect, float near, float far);
void calculate_view_matrix(vec3 eye[VEC3_SIZE], vec3 center[VEC3_SIZE], vec3 up[VEC3_SIZE]);

#endif
