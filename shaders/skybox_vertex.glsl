#version 330 core
layout (location = 0) in vec4 aPosition;
layout (location = 1) in vec3 aTexCoord;
layout (location = 2) in vec3 aNormal;

out vec3 Position;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

void main() {
    Position = aPosition.xyz;
    gl_Position = projectionMatrix * mat4(mat3(viewMatrix)) * aPosition;
}
