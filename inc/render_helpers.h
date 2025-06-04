#ifndef _RENDER_HELPERS_H
#define _RENDER_HELPERS_H

#include <data_structs.h>
#include <matrix_helper.h>

extern mat4 projMatrix[MAT4_SIZE];
extern mat4 viewMatrix[MAT4_SIZE];

/**
 * @brief Bind the OpenGL program associated with the Object3D structure to set uniforms and render the object.
 * 
 * @param object Pointer to the Object3D structure containing the object's data.
 */
void use_object_program(Object3D *object);

/**
 * @brief Render the Object3D using the current OpenGL context.
 * 
 * This function sets up the model matrix, normal matrix, projection matrix and view matrix uniforms.
 * But does not bind the program, which should be done before calling this function., e.g. by use_object_program().
 * 
 * @param object Pointer to the Object3D structure containing the object's data.
 */
void render_object(Object3D *object);

/**
 * @brief calculates the projection matrix into the global projMatrix variable.
 * 
 * @param fov  field of view in radians
 * @param aspect  aspect ratio of the viewport (width/height)
 * @param near  near clipping plane distance
 * @param far  far clipping plane distance
 */
void calculate_projection_matrix(float fov, float aspect, float near, float far);

/**
 * @brief  calculates the view matrix into the global viewMatrix variable.
 * 
 * @param eye  the position of the camera in world coordinates
 * @param center  the point the camera is looking at in world coordinates
 * @param up  the up vector of the camera in world coordinates
 */
void calculate_view_matrix(vec3 eye[VEC3_SIZE], vec3 center[VEC3_SIZE], vec3 up[VEC3_SIZE]);

#endif
