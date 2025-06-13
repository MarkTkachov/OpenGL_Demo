#ifndef _UNIFORMS_H_
#define _UNIFORMS_H_

#include <GL/glew.h>
#include <assert.h>

extern GLint modelMatrixUniformLocation;
extern GLint viewMatrixUniformLocation;
extern GLint projectionMatrixUniformLocation;
extern GLint normalMatrixUniformLocation;

void init_uniforms(GLuint program);

#endif
