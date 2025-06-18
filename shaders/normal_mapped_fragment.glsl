#version 330 core
in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;
in mat3 TBN;

uniform mat4 viewMatrix;

uniform vec3 lightPosition;

uniform vec4 emmisiveMaterialColor;
uniform vec4 ambientLightColor;
uniform vec4 diffuseLightColor;
uniform vec4 specularLightColor;
uniform float shininess;

uniform sampler2D baseTexture;
uniform sampler2D normalTexture;

uniform float textureScale;

uniform float time;

void main() {
    vec3 LightVector = normalize(vec3(viewMatrix * vec4(lightPosition, 0.0)) - Position);
    vec3 nNormal = texture(normalTexture, TexCoord * textureScale + vec2(0, time / 10)).xyz;
    nNormal = nNormal * 2.0 - 1.0; // Convert from [0,1] to [-1,1]
    nNormal = normalize(TBN * nNormal); // Transform normal from tangent space to world space
    vec4 emmisive = emmisiveMaterialColor;

    vec4 texColor = texture(baseTexture, TexCoord * textureScale + vec2(0, time / 10));
    vec4 ambient = ambientLightColor * texColor;

    vec3 reflectedLight = reflect(-LightVector, nNormal);
    vec4 diffuse = texColor * max(dot(LightVector, nNormal), 0) * diffuseLightColor;
    vec4 specular = texColor * pow(max(dot(reflectedLight, nNormal), 0), shininess) * specularLightColor;
    vec4 color = 
    emmisive
    + ambient
    + diffuse
    + specular;

    gl_FragColor = color;
}
