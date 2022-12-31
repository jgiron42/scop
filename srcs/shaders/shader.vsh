#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
out vec3 Color;
uniform mat4 projection;

void main()
{
    vec4 tmp = vec4(vec4(aPos.xyz, 1.0) * projection);
    gl_Position = vec4(tmp.xyz, tmp.w);
    Color = aColor;
}