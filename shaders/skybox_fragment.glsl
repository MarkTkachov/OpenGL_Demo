#version 330 core
in vec3 Position;

#define M_PI 3.1415926535897932384626433832795

uniform samplerCube skyTexture;

uniform vec3 colorMultiplier;

void main() {
    vec3 color = texture(skyTexture, Position).xyz * colorMultiplier;
    gl_FragColor = vec4(color, 1.0);
}
