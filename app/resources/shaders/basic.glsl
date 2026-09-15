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

// Spot Light (baterija iz kamere)
uniform vec3 spotLightPos;
uniform vec3 spotLightDir;
uniform vec3 spotLightColor;
uniform float spotCutOff;
uniform float spotOuterCutOff;

uniform float darkness;

void main() {
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(spotLightPos - FragPos);

    float ambientStrength = 1.0f - darkness + 0.15f;
    vec3 ambient = ambientStrength * vec3(1.0f);

    float diff = max(dot(norm, lightDir), 0.0f);
    vec3 diffuse = diff * spotLightColor;

    float theta = dot(lightDir, normalize(-spotLightDir));
    float epsilon = spotCutOff - spotOuterCutOff;
    float intensity = clamp((theta - spotOuterCutOff) / epsilon, 0.0f, 1.0f);

    vec3 spotContribution = diffuse * intensity * darkness;

    vec4 texColor = texture(texture_diffuse1, TexCords);
    vec3 result = (ambient + spotContribution) * texColor.rgb;
    FragColor = vec4(result, 1.0f);
}