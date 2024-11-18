#version 440 core

layout (location = 0) in vec3 position;

uniform mat4 vpmat;

void main() {
    gl_Position = vpmat * vec4(position, 1);
}
