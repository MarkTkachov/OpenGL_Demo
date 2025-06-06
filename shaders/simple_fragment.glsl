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

uniform float textureScale;

uniform float time;

void main() {
    vec3 LightVector = normalize(vec3(viewMatrix * vec4(lightPosition, 0.0)) - Position);
    vec3 nNormal = normalize(Normal);
    vec4 emmisive = emmisiveMaterialColor;

    vec4 texColor = texture(baseTexture, TexCoord);
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
