#ifndef _DATA_STRUCTS_H_
#define _DATA_STRUCTS_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <matrix_helper.h>

typedef struct
{
    float x;
    float y;
    float z;
    float w;
} VerticeCoords;

typedef struct
{
    float u;
    float v;
    float w;
} TextureCoords;

typedef struct
{
    float x;
    float y;
    float z;
} VertexNormal;

typedef struct {
    VerticeCoords vertex;
    TextureCoords texture;
    VertexNormal normal;
} FaceVertex;

typedef struct {
    vec3 position[VEC3_SIZE];
    vec3 rotation[VEC3_SIZE];
    vec3 scale[VEC3_SIZE];
    GLuint vao;
    GLuint vertex_count;
    GLuint program;
} Object3D;



#endif
