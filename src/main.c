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
Object3D fire;

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

GLuint fireTexture;
GLint fireTextureUniformLocation;

vec3 cameraPosition[VEC3_SIZE] = {2, 1, 2};
vec3 cameraTarget[VEC3_SIZE] = {0, 0, 0};


void init(void)
{ 
    glEnable(GL_DEPTH_TEST);
    // glEnable(GL_CULL_FACE);
    glDepthMask(GL_TRUE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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
    fireTexture = load_texture("textures/Fire_texture.png");


    load_3d_object(&skybox, "models/cube.obj", skyProgram);
    load_3d_object(&waterSurface, "models/flat_panel.obj", waterProgram);
    load_3d_object(&balloon, "models/balloon.obj", simpleProgram);
    load_3d_object(&fire, "models/Fire.obj", simpleProgram);
    
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glViewport(0, 0, 800, 800);

    calculate_projection_matrix(90 / (180 / M_PI), (float)screenWidth / (float)screenHeight, 0.01, 1000);
}

void draw(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    static float angle = 0;
    static int xRev = 1, yRev = 1;

    vec3 lightPosition[VEC3_SIZE] = {1000 * sin(angle/20), 1000 * cos(angle/20), 0};
    vec4 lightColor[VEC4_SIZE] = {0.8f, 0.8f, 0.8f, 1.0f};
    angle += 0.01;
    if (lightPosition[1] < 0) {
        lightColor[0] = 0.0f;
        lightColor[1] = 0.0f;
        lightColor[2] = 0.0f;
        // high night speed
        angle += 0.03;
    } else {
        lightColor[0] = 0.8f;
        lightColor[1] = 0.8f;
        lightColor[2] = 0.8f;
    }


    calculate_view_matrix(cameraPosition, cameraTarget, (vec3[]){0, 100, 0});

    glDepthMask(GL_FALSE);
    glDisable(GL_CULL_FACE);
    use_object_program(&skybox);
    init_uniforms(skybox.program);

    glUniform1i(skyTextureUniformLocation, 10);
    glActiveTexture(GL_TEXTURE10);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyTexture);

    if (lightPosition[1] < 0) {
        glUniform3f(glGetUniformLocation(skybox.program, "colorMultiplier"), 0.2f, 0.2f, 0.2f);
    } else {
        glUniform3f(glGetUniformLocation(skybox.program, "colorMultiplier"), 1.0f, 1.0f, 1.0f);
    }

    render_object(&skybox);
    glDepthMask(GL_TRUE);

    waterSurface.position[1] = -20;
    waterSurface.scale[0] = 10000;
    waterSurface.scale[2] = 10000;
    use_object_program(&waterSurface);
    init_uniforms(waterSurface.program);

    glUniform1i(waterTextureUniformLocation, 5);
    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_2D, waterTexture);
    glUniform1i(waterNormalTextureUniformLocation, 6);
    glActiveTexture(GL_TEXTURE6);
    glBindTexture(GL_TEXTURE_2D, waterNormalTexture);

    glUniform4f(glGetUniformLocation(waterSurface.program, "ambientLightColor"), 0.2f, 0.2f, 0.2f, 1.0f);
    glUniform4f(glGetUniformLocation(waterSurface.program, "diffuseLightColor"), lightColor[0], lightColor[1], lightColor[2], 1.0f);
    glUniform4f(glGetUniformLocation(waterSurface.program, "specularLightColor"), lightColor[0], lightColor[1], lightColor[2], 1.0f);
    glUniform1f(glGetUniformLocation(waterSurface.program, "shininess"), 50.0f);
    glUniform3f(glGetUniformLocation(waterSurface.program, "lightPosition"), lightPosition[0], lightPosition[1], lightPosition[2]);
    glUniform1f(glGetUniformLocation(waterSurface.program, "time"), angle);
    glUniform1f(glGetUniformLocation(waterSurface.program, "textureScale"), 1000.0f);
    render_object(&waterSurface);


    glEnable(GL_CULL_FACE);
    use_object_program(&balloon);
    init_uniforms(balloon.program);
    balloon.rotation[1] = M_PI / 2.0f * angle/100;

    emmisiveMaterialColorUniformLocation = glGetUniformLocation(balloon.program, "emmisiveMaterialColor");
    ambientLightColorUniformLocation = glGetUniformLocation(balloon.program, "ambientLightColor");
    diffuseLightColorUniformLocation = glGetUniformLocation(balloon.program, "diffuseLightColor");
    specularLightColorUniformLocation = glGetUniformLocation(balloon.program, "specularLightColor");
    shininessMaterialUniformLocation = glGetUniformLocation(balloon.program, "shininess");
    lightPositionUniformLocation = glGetUniformLocation(balloon.program, "lightPosition");

    // material properties
    glUniform4f(emmisiveMaterialColorUniformLocation, 0, 0, 0, 1.0f);
    glUniform4f(ambientLightColorUniformLocation, 0.2f, 0.2f, 0.2f, 1.0f);
    glUniform4f(diffuseLightColorUniformLocation, lightColor[0], lightColor[1], lightColor[2], 1.0f);
    glUniform4f(specularLightColorUniformLocation, lightColor[0], lightColor[1], lightColor[2], 1.0f);
    glUniform1f(shininessMaterialUniformLocation, 51.2f);
    //light position in scene coordinates
    glUniform3f(lightPositionUniformLocation, lightPosition[0], lightPosition[1], lightPosition[2]);
    glUniform1f(glGetUniformLocation(balloon.program, "time"), angle);

    //textures
    balloonTextureUniformLocation = glGetUniformLocation(balloon.program, "baseTexture");
    glUniform1i(balloonTextureUniformLocation, 1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, balloonTexture);

    glUniform1f(glGetUniformLocation(balloon.program, "texXOffset"), 0.0f);
    glUniform1f(glGetUniformLocation(balloon.program, "texYOffset"), 0.0f);

    render_object(&balloon);


    glDisable(GL_CULL_FACE);
    use_object_program(&fire);
    init_uniforms(fire.program);
    fire.rotation[1] = M_PI / 2.0f * angle/100;
    fire.scale[0] = 0.1f;
    fire.scale[1] = 0.1f;
    fire.scale[2] = 0.05f;
    fire.position[0] = 0.0f;
    fire.position[1] = 1.65f;
    fire.position[2] = 0.0f;


    emmisiveMaterialColorUniformLocation = glGetUniformLocation(fire.program, "emmisiveMaterialColor");
    ambientLightColorUniformLocation = glGetUniformLocation(fire.program, "ambientLightColor");
    diffuseLightColorUniformLocation = glGetUniformLocation(fire.program, "diffuseLightColor");
    specularLightColorUniformLocation = glGetUniformLocation(fire.program, "specularLightColor");
    shininessMaterialUniformLocation = glGetUniformLocation(fire.program, "shininess");
    lightPositionUniformLocation = glGetUniformLocation(fire.program, "lightPosition");

    glUniform4f(emmisiveMaterialColorUniformLocation, 1.0f, 1.0f, 1.0f, 1.0f);
    glUniform4f(ambientLightColorUniformLocation, 0.2f, 0.2f, 0.2f, 1.0f);
    glUniform4f(diffuseLightColorUniformLocation, lightColor[0], lightColor[1], lightColor[2], 1.0f);
    glUniform4f(specularLightColorUniformLocation, lightColor[0], lightColor[1], lightColor[2], 1.0f);
    glUniform1f(shininessMaterialUniformLocation, 51.2f);
    glUniform3f(lightPositionUniformLocation, lightPosition[0], lightPosition[1], lightPosition[2]);
    glUniform1f(glGetUniformLocation(fire.program, "time"), angle);
    glUniform1f(glGetUniformLocation(fire.program, "texXOffset"), 0.0f);
    glUniform1f(glGetUniformLocation(fire.program, "texYOffset"), (-1/32.0f) * (int)(angle * 10));
    fireTextureUniformLocation = glGetUniformLocation(fire.program, "baseTexture");
    glUniform1i(fireTextureUniformLocation, 2);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, fireTexture);
    render_object(&fire);

    fire.position[0] = lightPosition[0];
    fire.position[1] = lightPosition[1];
    fire.position[2] = lightPosition[2];
    fire.scale[0] = 100;
    fire.scale[1] = 100;
    fire.scale[2] = 100;
    render_object(&fire);
    glDisable(GL_CULL_FACE);

}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
    screenWidth = width;
    screenHeight = height;
    calculate_projection_matrix(90 / (180 / M_PI), (float)screenWidth / (float)screenHeight, 0.01, 1000);
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    // Move camera with WASD keys
    if (key == GLFW_KEY_W && (action == GLFW_REPEAT || action == GLFW_PRESS)){
        vec3 forward[VEC3_SIZE] = {0, 0, 0};
        vec3_sub(forward, cameraTarget, cameraPosition);
        vec3_normalize(forward, forward);
        forward[0] = forward[0] * 0.1f;
        forward[1] = forward[1] * 0.1f;
        forward[2] = forward[2] * 0.1f;
        vec3_add(cameraPosition, cameraPosition, forward);
        vec3_add(cameraTarget, cameraTarget, forward);
    }
    if (key == GLFW_KEY_S && (action == GLFW_REPEAT || action == GLFW_PRESS)){
        vec3 forward[VEC3_SIZE] = {0, 0, 0};
        vec3_sub(forward, cameraPosition, cameraTarget);
        vec3_normalize(forward, forward);
        forward[0] = forward[0] * 0.1f;
        forward[1] = forward[1] * 0.1f;
        forward[2] = forward[2] * 0.1f;
        vec3_add(cameraPosition, cameraPosition, forward);
        vec3_add(cameraTarget, cameraTarget, forward);
    }
    if (key == GLFW_KEY_A && (action == GLFW_REPEAT || action == GLFW_PRESS)){
        vec3 forward[VEC3_SIZE] = {0, 0, 0};
        vec3 tangent[VEC3_SIZE] = {0, 0, 0};
        vec3_sub(forward, cameraPosition, cameraTarget);
        vec3_normalize(forward, forward);
        vec3_cross(tangent, forward, (vec3[]){0, 1, 0});
        vec3_normalize(tangent, tangent);
        tangent[0] = tangent[0] * 0.1f;
        tangent[1] = tangent[1] * 0.1f;
        tangent[2] = tangent[2] * 0.1f;
        vec3_add(cameraPosition, cameraPosition, tangent);
        vec3_add(cameraTarget, cameraTarget, tangent);
    }
    if (key == GLFW_KEY_D && (action == GLFW_REPEAT || action == GLFW_PRESS)){
        vec3 forward[VEC3_SIZE] = {0, 0, 0};
        vec3 tangent[VEC3_SIZE] = {0, 0, 0};
        vec3_sub(forward, cameraPosition, cameraTarget);
        vec3_normalize(forward, forward);
        vec3_cross(tangent, (vec3[]){0, 1, 0}, forward);
        vec3_normalize(tangent, tangent);
        tangent[0] = tangent[0] * 0.1f;
        tangent[1] = tangent[1] * 0.1f;
        tangent[2] = tangent[2] * 0.1f;
        vec3_add(cameraPosition, cameraPosition, tangent);
        vec3_add(cameraTarget, cameraTarget, tangent);
    }

    // Move camera target with arrow keys
    if (key == GLFW_KEY_RIGHT && (action == GLFW_REPEAT || action == GLFW_PRESS)){
        mat4 rotationMatrix[MAT4_SIZE];
        identity(rotationMatrix);
        rotatey(rotationMatrix, rotationMatrix, -0.1f);
        mat3 rotationMatrix3[MAT3_SIZE];
        mat4_to_mat3(rotationMatrix3, rotationMatrix);
        vec3 newTarget[VEC3_SIZE] = {0, 0, 0};
        vec3_sub(newTarget, cameraTarget, cameraPosition);
        mat3_vec3_multiply(newTarget, rotationMatrix3, newTarget);
        vec3_add(newTarget, newTarget, cameraPosition);
        
        memcpy(cameraTarget, newTarget, VEC3_BYTESIZE);
    }
    if (key == GLFW_KEY_LEFT && (action == GLFW_REPEAT || action == GLFW_PRESS)){
        mat4 rotationMatrix[MAT4_SIZE];
        identity(rotationMatrix);
        rotatey(rotationMatrix, rotationMatrix, 0.1f);
        mat3 rotationMatrix3[MAT3_SIZE];
        mat4_to_mat3(rotationMatrix3, rotationMatrix);
        vec3 newTarget[VEC3_SIZE] = {0, 0, 0};
        vec3_sub(newTarget, cameraTarget, cameraPosition);
        mat3_vec3_multiply(newTarget, rotationMatrix3, newTarget);
        vec3_add(newTarget, newTarget, cameraPosition);
        
        memcpy(cameraTarget, newTarget, VEC3_BYTESIZE);
    }
    if (key == GLFW_KEY_UP && (action == GLFW_REPEAT || action == GLFW_PRESS)){
        vec3 forward[VEC3_SIZE] = {0, 0, 0};
        vec3 tangent[VEC3_SIZE] = {0, 0, 0};
        vec3_sub(forward, cameraPosition, cameraTarget);
        vec3_normalize(forward, forward);
        vec3_cross(tangent, (vec3[]){0, 1, 0}, forward);
        vec3_normalize(tangent, tangent);

        mat4 rotationMatrix[MAT4_SIZE];
        identity(rotationMatrix);
        rotateaxis(rotationMatrix, rotationMatrix, tangent, 0.1f);
        mat3 rotationMatrix3[MAT3_SIZE];
        mat4_to_mat3(rotationMatrix3, rotationMatrix);
        vec3 newTarget[VEC3_SIZE] = {0, 0, 0};
        vec3_sub(newTarget, cameraTarget, cameraPosition);
        mat3_vec3_multiply(newTarget, rotationMatrix3, newTarget);
        vec3_add(newTarget, newTarget, cameraPosition);
        
        memcpy(cameraTarget, newTarget, VEC3_BYTESIZE);
    }

     if (key == GLFW_KEY_DOWN && (action == GLFW_REPEAT || action == GLFW_PRESS)){
        vec3 forward[VEC3_SIZE] = {0, 0, 0};
        vec3 tangent[VEC3_SIZE] = {0, 0, 0};
        vec3_sub(forward, cameraPosition, cameraTarget);
        vec3_normalize(forward, forward);
        vec3_cross(tangent, (vec3[]){0, 1, 0}, forward);
        vec3_normalize(tangent, tangent);

        mat4 rotationMatrix[MAT4_SIZE];
        identity(rotationMatrix);
        rotateaxis(rotationMatrix, rotationMatrix, tangent, -0.1f);
        mat3 rotationMatrix3[MAT3_SIZE];
        mat4_to_mat3(rotationMatrix3, rotationMatrix);
        vec3 newTarget[VEC3_SIZE] = {0, 0, 0};
        vec3_sub(newTarget, cameraTarget, cameraPosition);
        mat3_vec3_multiply(newTarget, rotationMatrix3, newTarget);
        
        vec3_add(newTarget, newTarget, cameraPosition);
        
        memcpy(cameraTarget, newTarget, VEC3_BYTESIZE);
    }

        
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
    glfwSetKeyCallback(window, key_callback);
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
