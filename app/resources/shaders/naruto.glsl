//#shader vertex
#version 400 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCords;

out vec2 TexCords;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;
    TexCords = aTexCords;
    gl_Position = projection * view * vec4(FragPos, 1.0);
}

//#shader fragment
#version 400 core

out vec4 FragColor;

in vec2 TexCords;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D texture_diffuse1;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

uniform vec3 spotLightPos;
uniform vec3 spotLightDir;
uniform vec3 spotLightColor;
uniform float spotCutOff;
uniform float spotOuterCutOff;

uniform float darkness;

vec3 calculatePointLight(vec3 lColor, vec3 lPos, vec3 norm, vec3 viewDir) {
    float ambientStrength = 0.4f;
    vec3 ambient = ambientStrength * lColor;

    vec3 lightDir = normalize(lPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0f);
    vec3 diffuse = diff * lColor;

    float specularStrength = 0.5f;
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 32);
    vec3 specular = specularStrength * spec * lColor;

    return ambient + diffuse + specular;
}

vec3 calculateSpotLight(vec3 norm, vec3 viewDir) {
    vec3 lightDir = normalize(spotLightPos - FragPos);

    vec3 ambient = 0.05f * spotLightColor;

    float diff = max(dot(norm, lightDir), 0.0f);
    vec3 diffuse = diff * spotLightColor;

    float specularStrength = 0.5f;
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 32);
    vec3 specular = specularStrength * spec * spotLightColor;

    float theta = dot(lightDir, normalize(-spotLightDir));
    float epsilon = spotCutOff - spotOuterCutOff;
    float intensity = clamp((theta - spotOuterCutOff)/epsilon, 0.0f, 1.0f);

    return (ambient + diffuse + specular) * intensity;
}

void main() {
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 pointResult = calculatePointLight(lightColor, lightPos, norm, viewDir) * (1.0f - darkness);

    vec3 spotResult = calculateSpotLight(norm, viewDir) * darkness;

    vec4 texColor = texture(texture_diffuse1, TexCords);
    vec3 result = (pointResult + spotResult) * texColor.rgb;
    FragColor = vec4(result, 1.0f);
}