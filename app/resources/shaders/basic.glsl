//#shader vertex
#version 400 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCords;

out vec2 TexCords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    TexCords = aTexCords;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}

//#shader fragment
#version 400 core

out vec4 FragColor;

in vec2 TexCords;

uniform sampler2D texture_diffuse1;

void main() {
    FragColor = texture(texture_diffuse1, TexCords);
}