#ifndef _DATA_STRUCTS_H_
#define _DATA_STRUCTS_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <matrix_helper.h>

typedef struct s_VerticeCoords
{
    float x;
    float y;
    float z;
    float w;
} VerticeCoords;

typedef struct s_TextureCoords
{
    float u;
    float v;
    float w;
} TextureCoords;

typedef struct s_VertexNormal
{
    float x;
    float y;
    float z;
} VertexNormal;

typedef struct s_FaceVertex {
    VerticeCoords vertex;
    TextureCoords texture;
    VertexNormal normal;
} FaceVertex;

typedef struct s_FaceVertexWithTangent {
    VerticeCoords vertex;
    TextureCoords texture;
    VertexNormal normal;
    VertexNormal tangent;
} FaceVertexWithTangent;

typedef struct s_Object3D {
    vec3 position[VEC3_SIZE];
    vec3 rotation[VEC3_SIZE];
    vec3 scale[VEC3_SIZE];
    GLuint vao;
    GLuint vertex_count;
    GLuint program;
} Object3D;



#endif
