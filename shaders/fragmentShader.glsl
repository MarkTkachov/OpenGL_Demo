#version 330 core
in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;

uniform vec3 lightPosition;
uniform mat4 viewMatrix;

uniform vec4 emmisiveMaterialColor;
uniform vec4 ambientLightColor;
uniform vec4 ambientMaterialColor;
uniform vec4 diffuseLightColor;
uniform vec4 diffuseMaterialColor;
uniform vec4 specularLightColor;
uniform vec4 specularMaterialColor;
uniform float shininess;

uniform sampler2D dayTexture;
uniform sampler2D nightTexture;
uniform sampler2D oceanMaskTexture;
uniform sampler2D cloudsTexture;

uniform float time;

void main() {
    vec3 LightVector = normalize(vec3(viewMatrix * vec4(lightPosition, 0.0)) - Position);
    vec3 nNormal = normalize(Normal);
    vec4 emmisive = emmisiveMaterialColor;

    vec3 dayColor = texture(dayTexture, TexCoord).xyz;
    float nightColor = texture(nightTexture, TexCoord).r;
    float oceanMaskColor = texture(oceanMaskTexture, TexCoord).r;
    vec3 cloudsColor = texture(cloudsTexture, TexCoord - vec2(time / 30, 0.0)).xyz;

    // vec4 ambient = ambientLightColor * ambientMaterialColor;
    vec4 ambient = ambientLightColor * vec4(dayColor, 0.0);

    vec3 reflectedLight = reflect(-LightVector, nNormal);
    vec4 diffuse =mix(vec4(vec3(nightColor), 0) * vec4(120.0/256.0, 60.0/256.0, 10.0/256.0, 0), vec4(dayColor, 0.0), max(dot(LightVector, nNormal), 0)) * diffuseLightColor;
    vec4 specular = mix(vec4(vec3(nightColor), 0), vec4(dayColor, 0.0), pow(max(dot(reflectedLight, nNormal), 0), shininess)) * specularLightColor * oceanMaskColor;
    vec4 color = 
    // emmisive
    // + ambient
    + diffuse
    + specular;
    color = mix(color, vec4(1.0) * max(dot(LightVector, nNormal), 0), vec4(cloudsColor, 0.0));

    // color = texture(ourTexture, TexCoord);
    gl_FragColor = vec4(color);
}
