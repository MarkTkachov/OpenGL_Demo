#include <uniforms.h>
#include <render_helpers.h>
#include <matrix_helper.h>
#include <data_structs.h>

mat4 projMatrix[MAT4_SIZE];
mat4 viewMatrix[MAT4_SIZE];


void calculate_projection_matrix(float fov, float aspect, float near, float far) {
    perspective(projMatrix, fov, aspect, near, far);
}

void calculate_view_matrix(vec3 eye[VEC3_SIZE], vec3 center[VEC3_SIZE], vec3 up[VEC3_SIZE]) {
    assert(eye != NULL && center != NULL && up != NULL);\
    
    lookAt(viewMatrix, eye, center, up);
}


void use_object_program(Object3D *object) {
    assert(object != NULL);

    glUseProgram(object->program);
}


void render_object(Object3D *object) {
    assert(object != NULL);

    mat4 modelMatrix[MAT4_SIZE];
    identity(modelMatrix);
    translate(modelMatrix, modelMatrix, object->position);
    rotatex(modelMatrix, modelMatrix, object->rotation[0]);
    rotatey(modelMatrix, modelMatrix, object->rotation[1]);
    rotatez(modelMatrix, modelMatrix, object->rotation[2]);
    scale(modelMatrix, modelMatrix, object->scale);

    mat4 normalMatrix4[MAT4_SIZE];
    identity(normalMatrix4);
    mat4_multiply(normalMatrix4, viewMatrix, modelMatrix);
    mat3 normalMatrix3[MAT3_SIZE];
    mat4_to_mat3(normalMatrix3, normalMatrix4);
    mat3_inverse(normalMatrix3, normalMatrix3); 
    mat3_transpose(normalMatrix3, normalMatrix3);

    glUniformMatrix4fv(projectionMatrixUniformLocation, 1, GL_FALSE, projMatrix);
    glUniformMatrix4fv(viewMatrixUniformLocation, 1, GL_FALSE, viewMatrix);
    glUniformMatrix3fv(normalMatrixUniformLocation, 1, GL_FALSE, normalMatrix3);
    glUniformMatrix4fv(modelMatrixUniformLocation, 1, GL_FALSE, modelMatrix);
    glBindVertexArray(object->vao);
    glDrawArrays(GL_TRIANGLES, 0, object->vertex_count);
    glBindVertexArray(0);
}
