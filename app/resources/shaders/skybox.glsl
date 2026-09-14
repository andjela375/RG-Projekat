//#shader vertex
#version 400 core
layout (location = 0) in vec3 aPos;

out vec3 TexCords;

uniform mat4 projection;
uniform mat4 view;

void main() {
    TexCords = aPos;
    vec4 pos = projection * view * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
}

//#shader fragment
#version 400 core
out vec4 FragColor;

in vec3 TexCords;

uniform samplerCube skybox;

void main() {
    FragColor = texture(skybox, TexCords);
}