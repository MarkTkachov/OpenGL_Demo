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

Object3D earthObject;
Object3D skybox;
Object3D waterSurface;
Object3D balloon;

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

GLuint skyTexture;
GLint skyTextureUniformLocation;

GLuint waterTexture;
GLuint waterNormalTexture;
GLint waterTextureUniformLocation;
GLint waterNormalTextureUniformLocation;

GLuint balloonTexture;
GLint balloonTextureUniformLocation;


GLfloat logoColor[] = {220.0f, 60.0f, 5.0f};



void init(void)
{ 
    glEnable(GL_DEPTH_TEST);
    // glEnable(GL_CULL_FACE);
    glDepthMask(GL_TRUE);
    // glutInitDisplayMode(GLUT_DEPTH);
    // create and compile vertex shader
    GLuint program = compile_program("shaders/vertexShader.glsl", "shaders/fragmentShader.glsl");
    GLuint skyProgram = compile_program("shaders/skybox_vertex.glsl", "shaders/skybox_fragment.glsl");
    GLuint waterProgram = compile_program("shaders/vertexShader.glsl", "shaders/normal_mapped_fragment.glsl");
    GLuint simpleProgram = compile_program("shaders/vertexShader.glsl", "shaders/simple_fragment.glsl");

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
    
    skyTextureUniformLocation = glGetUniformLocation(skyProgram, "skyTexture");
    waterTextureUniformLocation = glGetUniformLocation(waterProgram, "baseTexture");
    waterNormalTextureUniformLocation = glGetUniformLocation(waterProgram, "normalTexture");
    
    dayTexture = load_texture("textures/earth_day.png");
    nightTexture = load_texture("textures/earth_night.png");
    oceanMaskTexture = load_texture("textures/earth_ocean_mask.png");
    cloudsTexture = load_texture("textures/earth_clouds.png");
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

    load_3d_object(&earthObject, "models/earth.obj", program);
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

    vec3 lightPosition[VEC3_SIZE] = {100 * sin(angle/2), 100 * cos(angle/2), 0};
    angle += 0.02;

    earthObject.rotation[1] = M_PI / 3.123 * angle;


    calculate_view_matrix((vec3[]){2, 1, 2 * cos(angle/2)}, (vec3[]){0, 0, 0}, (vec3[]){0, 1, 0});

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

    glUniform4f(glGetUniformLocation(waterSurface.program, "ambientLightColor"), 0.1f, 0.1f, 0.1f, 0.1f);
    glUniform4f(glGetUniformLocation(waterSurface.program, "diffuseLightColor"), 1.0f, 1.0f, 1.0f, 1.0f);
    glUniform4f(glGetUniformLocation(waterSurface.program, "specularLightColor"), 1.0f, 1.0f, 1.0f, 1.0f);
    glUniform1f(glGetUniformLocation(waterSurface.program, "shininess"), 10.0f);
    glUniform3f(glGetUniformLocation(waterSurface.program, "lightPosition"), lightPosition[0], lightPosition[1], lightPosition[2]);
    glUniform1f(glGetUniformLocation(waterSurface.program, "time"), angle);
    glUniform1f(glGetUniformLocation(waterSurface.program, "textureScale"), 100.0f);
    render_object(&waterSurface);


    use_object_program(&balloon);
    init_uniforms(balloon.program);
    balloon.rotation[1] = M_PI / 2.0f * angle;

    emmisiveMaterialColorUniformLocation = glGetUniformLocation(balloon.program, "emmisiveMaterialColor");
    ambientLightColorUniformLocation = glGetUniformLocation(balloon.program, "ambientLightColor");
    ambientMaterialColorUniformLocation = glGetUniformLocation(balloon.program, "ambientMaterialColor");
    diffuseLightColorUniformLocation = glGetUniformLocation(balloon.program, "diffuseLightColor");
    diffuseMaterialColorUniformLocation = glGetUniformLocation(balloon.program, "diffuseMaterialColor");
    specularLightColorUniformLocation = glGetUniformLocation(balloon.program, "specularLightColor");
    specularMaterialColorUniformLocation = glGetUniformLocation(balloon.program, "specularMaterialColor");
    shininessMaterailUniformLocation = glGetUniformLocation(balloon.program, "shininess");
    lightPositionUniformLocation = glGetUniformLocation(balloon.program, "lightPosition");

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
