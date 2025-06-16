#define _XOPEN_SOURCE 500
#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
// #include <GL/glut.h>

#include "../inc/matrix_helper.h"
#include <file_utils.h>
#include <data_structs.h>
#include <uniforms.h>
#include <render_helpers.h>
#include <assert.h>
#include <stb_image.h>

#include <math.h>
#include <string.h>

int screenWidth = 800;
int screenHeight = 800;

Object3D skybox;
Object3D waterSurface;
Object3D balloon;

GLint emmisiveMaterialColorUniformLocation;
GLint ambientLightColorUniformLocation;
GLint diffuseLightColorUniformLocation;
GLint specularLightColorUniformLocation;
GLint shininessMaterialUniformLocation;
GLint lightPositionUniformLocation;

GLuint skyTexture;
GLint skyTextureUniformLocation;

GLuint waterTexture;
GLuint waterNormalTexture;
GLint waterTextureUniformLocation;
GLint waterNormalTextureUniformLocation;

GLuint balloonTexture;
GLint balloonTextureUniformLocation;


void init(void)
{ 
    glEnable(GL_DEPTH_TEST);
    // glEnable(GL_CULL_FACE);
    glDepthMask(GL_TRUE);
    // glutInitDisplayMode(GLUT_DEPTH);
    // create and compile vertex shader
    GLuint skyProgram = compile_program("shaders/skybox_vertex.glsl", "shaders/skybox_fragment.glsl");
    GLuint waterProgram = compile_program("shaders/vertexShader.glsl", "shaders/normal_mapped_fragment.glsl");
    GLuint simpleProgram = compile_program("shaders/vertexShader.glsl", "shaders/simple_fragment.glsl");
    
    skyTextureUniformLocation = glGetUniformLocation(skyProgram, "skyTexture");
    waterTextureUniformLocation = glGetUniformLocation(waterProgram, "baseTexture");
    waterNormalTextureUniformLocation = glGetUniformLocation(waterProgram, "normalTexture");
    
    skyTexture = load_cubemap((const char *[6]){
        "textures/spiaggia_cubemap/px.png",
        "textures/spiaggia_cubemap/nx.png",
        "textures/spiaggia_cubemap/py.png",
        "textures/spiaggia_cubemap/ny.png",
        "textures/spiaggia_cubemap/pz.png",
        "textures/spiaggia_cubemap/nz.png"
    });
    waterTexture = load_texture("textures/water.jpg");
    waterNormalTexture = load_texture("textures/water_normal.jpg");
    balloonTexture = load_texture("textures/combined_texture_balloon.png");


    load_3d_object(&skybox, "models/cube.obj", skyProgram);
    load_3d_object(&waterSurface, "models/flat_panel.obj", waterProgram);
    load_3d_object(&balloon, "models/balloon.obj", simpleProgram);
    
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glViewport(0, 0, 800, 800);

    calculate_projection_matrix(90 / (180 / M_PI), (float)screenWidth / (float)screenHeight, 0.01, 1000);
}

void draw(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    static float angle = 0;
    static int xRev = 1, yRev = 1;

    vec3 lightPosition[VEC3_SIZE] = {10000 * sin(angle/5), 10000 * cos(angle/5), 0};
    angle += 0.02;


    calculate_view_matrix((vec3[]){2, 1, 10}, (vec3[]){0, 0, 0}, (vec3[]){0, 1, 0});

    glDepthMask(GL_FALSE);
    glDisable(GL_CULL_FACE);
    use_object_program(&skybox);
    init_uniforms(skybox.program);

    glUniform1i(skyTextureUniformLocation, 10);
    glActiveTexture(GL_TEXTURE10);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyTexture);

    render_object(&skybox);
    glDepthMask(GL_TRUE);

    waterSurface.position[1] = -10;
    waterSurface.scale[0] = 1000;
    waterSurface.scale[2] = 1000;
    use_object_program(&waterSurface);
    init_uniforms(waterSurface.program);

    glUniform1i(waterTextureUniformLocation, 5);
    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_2D, waterTexture);
    glUniform1i(waterNormalTextureUniformLocation, 6);
    glActiveTexture(GL_TEXTURE6);
    glBindTexture(GL_TEXTURE_2D, waterNormalTexture);

    glUniform4f(glGetUniformLocation(waterSurface.program, "ambientLightColor"), 0.2f, 0.2f, 0.2f, 1.0f);
    glUniform4f(glGetUniformLocation(waterSurface.program, "diffuseLightColor"), 0.8f, 0.8f, 0.8f, 1.0f);
    glUniform4f(glGetUniformLocation(waterSurface.program, "specularLightColor"), 0.8f, 0.8f, 0.8f, 1.0f);
    glUniform1f(glGetUniformLocation(waterSurface.program, "shininess"), 100.0f);
    glUniform3f(glGetUniformLocation(waterSurface.program, "lightPosition"), lightPosition[0], lightPosition[1], lightPosition[2]);
    glUniform1f(glGetUniformLocation(waterSurface.program, "time"), angle);
    glUniform1f(glGetUniformLocation(waterSurface.program, "textureScale"), 100.0f);
    render_object(&waterSurface);


    glEnable(GL_CULL_FACE);
    use_object_program(&balloon);
    init_uniforms(balloon.program);
    balloon.rotation[1] = M_PI / 2.0f * angle;

    emmisiveMaterialColorUniformLocation = glGetUniformLocation(balloon.program, "emmisiveMaterialColor");
    ambientLightColorUniformLocation = glGetUniformLocation(balloon.program, "ambientLightColor");
    diffuseLightColorUniformLocation = glGetUniformLocation(balloon.program, "diffuseLightColor");
    specularLightColorUniformLocation = glGetUniformLocation(balloon.program, "specularLightColor");
    shininessMaterialUniformLocation = glGetUniformLocation(balloon.program, "shininess");
    lightPositionUniformLocation = glGetUniformLocation(balloon.program, "lightPosition");

    // material properties
    glUniform4f(emmisiveMaterialColorUniformLocation, 0, 0, 0, 1.0f);
    glUniform4f(ambientLightColorUniformLocation, 0.2f, 0.2f, 0.2f, 1.0f);
    glUniform4f(diffuseLightColorUniformLocation, 0.8f, 0.8f, 0.8f, 1.0f);
    glUniform4f(specularLightColorUniformLocation, 0.8f, 0.8f, 0.8f, 1.0f);
    glUniform1f(shininessMaterialUniformLocation, 51.2f);
    //light position in scene coordinates
    glUniform3f(lightPositionUniformLocation, lightPosition[0], lightPosition[1], lightPosition[2]);
    glUniform1f(glGetUniformLocation(balloon.program, "time"), angle);

    //textures
    balloonTextureUniformLocation = glGetUniformLocation(balloon.program, "baseTexture");
    glUniform1i(balloonTextureUniformLocation, 1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, balloonTexture);

    render_object(&balloon);

}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
    screenWidth = width;
    screenHeight = height;
    calculate_projection_matrix(90 / (180 / M_PI), (float)screenWidth / (float)screenHeight, 0.01, 1000);
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
