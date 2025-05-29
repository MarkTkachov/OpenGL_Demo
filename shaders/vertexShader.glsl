#version 330 core
layout (location = 0) in vec4 aPosition;
layout (location = 1) in vec3 aTexCoord;
layout (location = 2) in vec3 aNormal;

out vec3 Position;
out vec3 Normal;
out vec2 TexCoord;

uniform vec3 lightPosition;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat3 normalMatrix;
uniform mat4 projectionMatrix;

void main() {
    vec4 viewPosition = viewMatrix * modelMatrix * aPosition;
    Position = vec3(viewPosition);
    Normal = vec3((normalMatrix * aNormal));
    TexCoord = vec2(aTexCoord);
    
    gl_Position = projectionMatrix * viewPosition;
}
