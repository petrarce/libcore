#version 440 core

layout (location = 0) in vec2 position;
layout (location = 1) in float color;

out float fs_Color;
void main() {
    gl_Position = vec4(position, 0, 1);
    fs_Color = color;
}
