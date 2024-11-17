#version 440 core
in float fs_Color;
layout(location = 0) out vec4 diffuseColor;
void main() { diffuseColor = vec4(0, 0, fs_Color, 0.5); }
