#ifndef _FILE_UTILS_H_
#define _FILE_UTILS_H_

#include <stdlib.h>
#include <GL/glew.h>

char *read_file(const char *filename, size_t *length);
void read_obj_file(const char *filename, GLuint *face_vertex_vbo_out, GLsizei *vertex_count_out);
GLuint load_texture(const char *filename);

#endif
