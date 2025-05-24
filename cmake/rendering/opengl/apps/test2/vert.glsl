#version 440 core

layout (location = 0) in vec2 position;

uniform mat4 view;
uniform mat4 persp;
void main() {
    gl_Position = persp * view * vec4(position, 0, 1);
}
