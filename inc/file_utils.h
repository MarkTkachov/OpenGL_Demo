#ifndef _FILE_UTILS_H_
#define _FILE_UTILS_H_

#include <stdlib.h>
#include <GL/glew.h>

char *read_file(const char *filename, size_t *length);

void read_obj_file(const char *filename, GLuint *face_vertex_vbo_out, GLsizei *vertex_count_out);

// Load a 3D object from an OBJ file into an Object3D structure
// out: pointer to the Object3D structure to fill, NOT NULL
// filename: path to the OBJ file, NOT NULL
// program: OpenGL shader program ID to use for the object
void load_3d_object(Object3D *out, const char *filename, GLuint program);

GLuint load_texture(const char *filename);

#endif
