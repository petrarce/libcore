#version 440 core
uniform float color;
layout(location = 0) out vec4 diffuseColor;
void main() { diffuseColor = vec4(0, color, 0, 0.5); }
