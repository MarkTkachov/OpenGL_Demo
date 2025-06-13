#version 330 core
in vec3 Position;

#define M_PI 3.1415926535897932384626433832795

uniform samplerCube skyTexture;

void main() {
    vec3 color = texture(skyTexture, Position).xyz;
    gl_FragColor = vec4(color, 1.0);
}
