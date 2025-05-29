#define _POSIX_C_SOURCE 200809L
#define STB_IMAGE_IMPLEMENTATION

#include <file_utils.h>
#include <data_structs.h>
#include <stdio.h>
#include <string.h>
#include <stb_image.h>

char *read_file(const char *filename, size_t *length)
{
    size_t file_length;
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        perror("Error opening file");
        printf(" %s\n", filename);
        return NULL;
    }
    fseek(file, 0, SEEK_END);
    file_length = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *buffer = malloc(file_length + 1);
    if (buffer == NULL)
    {
        perror("Error allocating memory");
        fclose(file);
        return NULL;
    }
    size_t read_length = fread(buffer, 1, file_length, file);
    if (read_length != file_length)
    {
        perror("Error reading file");
        printf(" %s\n", filename);
        free(buffer);
        fclose(file);
        return NULL;
    }
    buffer[file_length] = '\0';
    fclose(file);
    if (length != NULL)
    {
        *length = file_length;
    }
    return buffer;
}

void read_obj_file(const char *filename, GLuint *face_vertex_vbo_out, GLsizei *vertex_count_out)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        perror("Error opening file");
        printf(" %s\n", filename);
        return;
    }
    GLsizei vertex_read_count = 0;
    GLsizei texture_read_count = 0;
    GLsizei normal_read_count = 0;
    GLsizei face_read_count = 0;

    // count the number of vertices, texture coordinates, and normals
    char *line = NULL;
    size_t line_length = 0;
    while (getline(&line, &line_length, file) != -1)
    {
        // printf("%s", line);
        if (line[0] == 'v' && line[1] == ' ')
        {
            vertex_read_count++;
        }
        else if (line[0] == 'v' && line[1] == 't')
        {
            texture_read_count++;
        }
        else if (line[0] == 'v' && line[1] == 'n')
        {
            normal_read_count++;
        }
        else if (line[0] == 'f' && line[1] == ' ')
        {
            face_read_count++;
        }
    }
    fseek(file, 0, SEEK_SET);
    
    // printf("init arrays1\n");
    // allocate memory for the vertices, texture coordinates, and normals
    VerticeCoords *vertices = malloc(sizeof(VerticeCoords) * vertex_read_count);
    TextureCoords *textures = malloc(sizeof(TextureCoords) * texture_read_count);
    VertexNormal *normals = malloc(sizeof(VertexNormal) * normal_read_count);
    FaceVertex *facesVertices = malloc(sizeof(FaceVertex) * face_read_count * 3);
    if (vertices == NULL || textures == NULL || normals == NULL || facesVertices == NULL)
    {
        perror("Error allocating memory");
        free(vertices);
        free(textures);
        free(normals);
        free(facesVertices);
        fclose(file);
        return;
    }
    // printf("init arrays\n");

    memset(vertices, 0, sizeof(VerticeCoords) * vertex_read_count);
    memset(textures, 0, sizeof(TextureCoords) * texture_read_count);
    memset(normals, 0, sizeof(TextureCoords) * normal_read_count);
    memset(facesVertices, 0, sizeof(FaceVertex) * face_read_count * 3);

    // read the vertices, texture coordinates, and normals
    GLsizei vertex_index = 0;
    GLsizei texture_index = 0;
    GLsizei normal_index = 0;
    GLsizei face_index = 0;
    // read the file again and fill the arrays
    while (getline(&line, &line_length, file) != -1)
    {
        // printf("%s", line);
        // vertex coordinates
        if (line[0] == 'v' && line[1] == ' ')
        {
            vertices[vertex_index].w = 1.0f; // default value
            sscanf(line, "v %f %f %f %f", &vertices[vertex_index].x, &vertices[vertex_index].y, &vertices[vertex_index].z, &vertices[vertex_index].w);
            vertex_index++;
        }
        // texture coordinates
        else if (line[0] == 'v' && line[1] == 't')
        {
            sscanf(line, "vt %f %f %f", &textures[texture_index].u, &textures[texture_index].v, &textures[texture_index].w);
            texture_index++;
        }
        // vertex normals
        else if (line[0] == 'v' && line[1] == 'n')
        {
            sscanf(line, "vn %f %f %f", &normals[normal_index].x, &normals[normal_index].y, &normals[normal_index].z);
            normal_index++;
        }
        // faces
        else if (line[0] == 'f' && line[1] == ' ')
        {
            VerticeCoords vertex;
            TextureCoords texture;
            VertexNormal normal;
            int vertex_indices[3];
            int texture_indices[3];
            int normal_indices[3];
            memset(&vertex, 0, sizeof(vertex));
            memset(&texture, 0, sizeof(texture));
            memset(&normal, 0, sizeof(normal));
            memset(vertex_indices, 0, sizeof(vertex_indices));
            memset(texture_indices, 0, sizeof(texture_indices));
            memset(normal_indices, 0, sizeof(normal_indices));
            // possible formats:
            if (9 == sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d", &vertex_indices[0], &texture_indices[0], &normal_indices[0], &vertex_indices[1], &texture_indices[1], &normal_indices[1], &vertex_indices[2], &texture_indices[2], &normal_indices[2]))
            {
            }
            else if (6 == sscanf(line, "f %d//%d %d//%d %d//%d", &vertex_indices[0], &normal_indices[0], &vertex_indices[1], &normal_indices[1], &vertex_indices[2], &normal_indices[2]))
            {
            }
            else if (3 == sscanf(line, "f %d %d %d", &vertex_indices[0], &vertex_indices[1], &vertex_indices[2]))
            {
            }
            else
            {
                printf("Error: Cannot parse face: %s\n", line);
                continue;
            }
            // combine data from arrays into single FaceVertex
            for (int i = 0; i < 3; i++)
            {
                vertex_indices[i]--;
                texture_indices[i]--;
                normal_indices[i]--;
                if (vertex_indices[i] >= 0)
                    facesVertices[face_index].vertex = vertices[vertex_indices[i]];
                if (texture_indices[i] >= 0)
                    facesVertices[face_index].texture = textures[texture_indices[i]];
                if (normal_indices[i] >= 0)
                    facesVertices[face_index].normal = normals[normal_indices[i]];
                face_index++;
            }
        }
    }
    free(line);
    line = NULL;
    fclose(file);
    file = NULL;

    // create VBO
    GLuint face_vertex_vbo;
    glGenBuffers(1, &face_vertex_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, face_vertex_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(FaceVertex) * face_index, facesVertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    if (face_vertex_vbo_out != NULL)
    {
        *face_vertex_vbo_out = face_vertex_vbo;
    }
    if (vertex_count_out != NULL)
    {
        *vertex_count_out = face_index;
    }
    free(vertices);
    free(textures);
    free(normals);
    free(facesVertices);
}

GLuint load_texture(const char *filename) {
    int width, height, channels;
    stbi_set_flip_vertically_on_load(1);
    unsigned char *data = stbi_load(filename, &width, &height, &channels, 0);
    if (data == NULL) {
        fprintf(stderr, "Error loading texture: %s\n", filename);
        return 0;
    }
    GLint format = channels == 1 ? GL_RED : channels == 2 ? GL_RG : channels == 3 ? GL_RGB : GL_RGBA;
    // printf("Texture loaded: %s (%d x %d, %d channels)\n", filename, width, height, channels);
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);
    return texture;
}

void load_3d_object(Object3D *out, const char *filename, GLuint program) {
    assert(out != NULL);
    assert(filename != NULL);

    memset(out, 0, sizeof(Object3D));
    GLuint face_vertex_vbo;
    GLsizei vertex_count;
    read_obj_file(filename, &face_vertex_vbo, &vertex_count);
    if (face_vertex_vbo == 0 || vertex_count == 0) {
        fprintf(stderr, "Error loading 3D object from file: %s\n", filename);
        return;
    }

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, face_vertex_vbo);

    // vertex position
    glVertexAttribPointer(
        // 0 from location = 0 in vertex shader
        0,
        4,
        GL_FLOAT,
        GL_FALSE,
        sizeof(FaceVertex),
        (GLvoid *)(offsetof(FaceVertex, vertex)));
    glEnableVertexAttribArray(0);
    // texture coordinates
    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(FaceVertex),
        (GLvoid *)(offsetof(FaceVertex, texture)));
    glEnableVertexAttribArray(1);
    // normals coordinates
    glVertexAttribPointer(
        2,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(FaceVertex),
        (GLvoid *)(offsetof(FaceVertex, normal)));
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    out->vao = vao;
    out->vertex_count = vertex_count;
    out->program = program;
    memcpy(out->position, (vec3[]){0.0f, 0.0f, 0.0f}, VEC3_BYTESIZE);
    memcpy(out->rotation, (vec3[]){0.0f, 0.0f, 0.0f}, VEC3_BYTESIZE);
    memcpy(out->scale, (vec3[]){1.0f, 1.0f, 1.0f}, VEC3_BYTESIZE);
}
