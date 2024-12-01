#version 440 core

layout(location = 0) in vec3 position;

uniform mat4 vpmat;

layout(location = 0)out vec3 vcolor;

void main() {
    gl_Position = vpmat * vec4(position, 1);
    vcolor = (vec3(gl_Position) + vec3(1)) * 0.5;
}
