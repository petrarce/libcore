#version 440 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 inTexCoord;

uniform mat4 vpmat;

layout(location = 0) out vec2 texCoord;

void main()
{
    gl_Position = vpmat * vec4(pos, 1);
    texCoord = inTexCoord;
}
