#include <uniforms.h>

GLint modelMatrixUniformLocation;
GLint viewMatrixUniformLocation;
GLint projectionMatrixUniformLocation;
GLint normalMatrixUniformLocation;

void init_uniforms(GLuint program)
{
    modelMatrixUniformLocation = glGetUniformLocation(program, "modelMatrix");
    viewMatrixUniformLocation = glGetUniformLocation(program, "viewMatrix");
    projectionMatrixUniformLocation = glGetUniformLocation(program, "projectionMatrix");
    normalMatrixUniformLocation = glGetUniformLocation(program, "normalMatrix");
}
