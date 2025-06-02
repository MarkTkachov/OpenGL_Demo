#ifndef _FILE_UTILS_H_
#define _FILE_UTILS_H_

#include <stdlib.h>
#include <GL/glew.h>
#include <data_structs.h>

char *read_file(const char *filename, size_t *length);

void read_obj_file(const char *filename, GLuint *face_vertex_vbo_out, GLsizei *vertex_count_out);

// Load a 3D object from an OBJ file into an Object3D structure
// @param out: pointer to the Object3D structure to fill, NOT NULL
// @param filename: path to the OBJ file, NOT NULL
// @param program: OpenGL shader program ID to use for the object
void load_3d_object(Object3D *out, const char *filename, GLuint program);

// Compile a shader from a file
// @param filename: path to the shader file, NOT NULL
// @param shader_type: type of the shader (GL_VERTEX_SHADER or GL_FRAGMENT_SHADER)
// @return the shader ID on success, 0 on failure
GLuint compile_shader(const char *filename, GLenum shader_type);

// Compile a shader program from vertex and fragment shader files
// @param vertex_shader_filename: path to the vertex shader file, NOT NULL
// @param fragment_shader_filename: path to the fragment shader file, NOT NULL
// @return the program ID on success, 0 on failure
GLuint compile_program(const char *vertex_shader_filename, const char *fragment_shader_filename);

GLuint load_texture(const char *filename);

/**
 * @brief  Load a cubemap texture from six image files.
 * Order of the faces:
 * 
 * 1. Positive X (right)
 * 
 * 2. Negative X (left)
 * 
 * 3. Positive Y (top)
 * 
 * 4. Negative Y (bottom)
 * 
 * 5. Positive Z (front)
 * 
 * 6. Negative Z (back)
 * 
 * @param faces  Array of six strings, each representing the path to an image file for the cubemap faces.
 * @return GLuint  The OpenGL texture ID of the loaded cubemap texture, or 0 on failure.
 */
GLuint load_cubemap(const char *faces[6]);

#endif
