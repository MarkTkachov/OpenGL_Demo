#define _XOPEN_SOURCE 500
#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
// #include <GL/glut.h>

#include "../inc/matrix_helper.h"
#include <file_utils.h>
#include <data_structs.h>
#include <uniforms.h>

#include <math.h>
#include <string.h>

Object3D earthObject;

GLint emmisiveMaterialColorUniformLocation;
GLint ambientLightColorUniformLocation;
GLint ambientMaterialColorUniformLocation;
GLint diffuseLightColorUniformLocation;
GLint diffuseMaterialColorUniformLocation;
GLint specularLightColorUniformLocation;
GLint specularMaterialColorUniformLocation;
GLint shininessMaterailUniformLocation;
GLint lightPositionUniformLocation;

GLint dayTextureUniformLocation;
GLint nightTextureUniformLocation;
GLint oceanMaskTextureUniformLocation;
GLint cloudsTextureUniformLocation;

GLuint dayTexture;
GLuint nightTexture;
GLuint oceanMaskTexture;
GLuint cloudsTexture;



GLfloat logoColor[] = {220.0f, 60.0f, 5.0f};
mat4 projMatrix[MAT4_SIZE];

void init(void)
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glDepthMask(GL_TRUE);
    // glutInitDisplayMode(GLUT_DEPTH);
    // create and compile vertex shader
    GLuint program = compile_program("shaders/vertexShader.glsl", "shaders/fragmentShader.glsl");

    emmisiveMaterialColorUniformLocation = glGetUniformLocation(program, "emmisiveMaterialColor");
    ambientLightColorUniformLocation = glGetUniformLocation(program, "ambientLightColor");
    ambientMaterialColorUniformLocation = glGetUniformLocation(program, "ambientMaterialColor");
    diffuseLightColorUniformLocation = glGetUniformLocation(program, "diffuseLightColor");
    diffuseMaterialColorUniformLocation = glGetUniformLocation(program, "diffuseMaterialColor");
    specularLightColorUniformLocation = glGetUniformLocation(program, "specularLightColor");
    specularMaterialColorUniformLocation = glGetUniformLocation(program, "specularMaterialColor");
    shininessMaterailUniformLocation = glGetUniformLocation(program, "shininess");
    lightPositionUniformLocation = glGetUniformLocation(program, "lightPosition");

    init_uniforms(program);

    dayTextureUniformLocation = glGetUniformLocation(program, "dayTexture");
    nightTextureUniformLocation = glGetUniformLocation(program, "nightTexture");
    oceanMaskTextureUniformLocation = glGetUniformLocation(program, "oceanMaskTexture");
    cloudsTextureUniformLocation = glGetUniformLocation(program, "cloudsTexture");
    
    dayTexture = load_texture("textures/earth_day.png");
    nightTexture = load_texture("textures/earth_night.png");
    oceanMaskTexture = load_texture("textures/earth_ocean_mask.png");
    cloudsTexture = load_texture("textures/earth_clouds.png");

    load_3d_object(&earthObject, "models/earth.obj", program);
    
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glViewport(0, 0, 800, 800);

    perspective(projMatrix, 90 / (180 / M_PI), 800 / 800, 0.01, 1000);
}

void draw(void)
{
    static mat4 modelMatrix[MAT4_SIZE];
    static mat4 viewMatrix[MAT4_SIZE];
    static float angle = 0;
    static int xRev = 1, yRev = 1;

    angle += 0.02;

    earthObject.rotation[1] = M_PI / 3.123 * angle;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    lookAt(viewMatrix, (vec3[]){2 * sin(angle/2), 1, 2 * cos(angle/2)}, (vec3[]){0, 0, 0}, (vec3[]){0, 1, 0});
    glUseProgram(earthObject.program);
    glUniformMatrix4fv(projectionMatrixUniformLocation, 1, GL_FALSE, projMatrix);
    glUniformMatrix4fv(viewMatrixUniformLocation, 1, GL_FALSE, viewMatrix);

    // material properties
    glUniform4f(emmisiveMaterialColorUniformLocation, 0, 0, 0, 1.0f);
    glUniform4f(ambientLightColorUniformLocation, 0.5f, 0.5f, 0.5f, 1.0f);
    glUniform4f(ambientMaterialColorUniformLocation, 0.23f, 0.09f, 0.03f, 1.0f);
    glUniform4f(diffuseLightColorUniformLocation, 1.0f, 1.0f, 1.0f, 1.0f);
    glUniform4f(diffuseMaterialColorUniformLocation, 0.55f, 0.21f, 0.07f, 1.0f);
    glUniform4f(specularLightColorUniformLocation, 1.0f, 1.0f, 1.0f, 1.0f);
    glUniform4f(specularMaterialColorUniformLocation, 0.58f, 0.22f, 0.07f, 1.0f);
    glUniform1f(shininessMaterailUniformLocation, 51.2f);
    //light position in scene coordinates
    glUniform3f(lightPositionUniformLocation, -3, 3, 3);
    glUniform1f(glGetUniformLocation(earthObject.program, "time"), angle);
    glBindVertexArray(earthObject.vao);

    // model matrix
    identity(modelMatrix);
    translate(modelMatrix, modelMatrix, earthObject.position);
    rotatey(modelMatrix, modelMatrix, earthObject.rotation[1]);
    scale(modelMatrix, modelMatrix, earthObject.scale);

    glUniformMatrix4fv(modelMatrixUniformLocation, 1, GL_FALSE, modelMatrix);

    //normal matrix
    mat4 normalMatrix4[MAT4_SIZE];
    identity(normalMatrix4);
    mat4_multiply(normalMatrix4, viewMatrix, modelMatrix);
    mat3 normalMatrix3[MAT3_SIZE];
    mat4_to_mat3(normalMatrix3, normalMatrix4);
    mat3_inverse(normalMatrix3, normalMatrix3); 
    mat3_transpose(normalMatrix3, normalMatrix3);
    glUniformMatrix3fv(normalMatrixUniformLocation, 1, GL_FALSE, normalMatrix3);

    //textures
    glUniform1i(dayTextureUniformLocation, 1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, dayTexture);

    glUniform1i(nightTextureUniformLocation, 2);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, nightTexture);

    glUniform1i(oceanMaskTextureUniformLocation, 3);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, oceanMaskTexture);

    glUniform1i(cloudsTextureUniformLocation, 4);
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, cloudsTexture);

    glDrawArrays(GL_TRIANGLES, 0, earthObject.vertex_count);

}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

int main(int argc, char *argv[])
{

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(800, 800, "LearnOpenGL", NULL, NULL);
    if (!window)
    {
        printf("Failed to create GLFW window\n");
        glfwTerminate();
        return 1;
    }
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwMakeContextCurrent(window);
    glewInit();
    init();

    while (!glfwWindowShouldClose(window))
    {
        draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
