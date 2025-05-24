#version 440 core
layout(location = 0) in vec3 vcolor;

uniform float color;

layout(location = 0) out vec4 diffuseColor;
void main() {
    diffuseColor = vec4(vec3(vcolor * color), 1);
}
