#version 330 core
layout (location = 0) in vec4 aPosition;
layout (location = 1) in vec3 aTexCoord;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec3 aTangent;

out vec3 Position;
out vec3 Normal;
out vec2 TexCoord;
out mat3 TBN; // Tangent, Bitangent, Normal viewMatrix
out vec3 LightDir;

uniform vec3 lightPosition;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat3 normalMatrix;
uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;
uniform vec3 lightPosViewSpace;



void main() {
    vec4 viewPosition = modelViewMatrix * aPosition;
    Position = vec3(viewPosition);
    Normal = normalize(vec3(viewMatrix * vec4(aNormal, 0.0)));
    TexCoord = vec2(aTexCoord);

    vec3 T = normalize(vec3(modelMatrix * vec4(aTangent,   0.0)));
    vec3 N = normalize(vec3(modelMatrix * vec4(aNormal,    0.0)));
    vec3 B = normalize(cross(N, T));
    TBN = mat3(T, B, N);
    LightDir = normalize(lightPosViewSpace - Position);
    LightDir = lightDir; 
    
    gl_Position = projectionMatrix * viewPosition;
}
