#ifndef _DATA_STRUCTS_H_
#define _DATA_STRUCTS_H_

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



#endif
