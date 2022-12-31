#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec3 aCol;
layout (location = 3) in vec2 aTexCoord;
out float luminance;
out vec3 color;
out vec2 TexCoord;
uniform mat4 projection;
uniform float textureSwitch;

void main()
{
    gl_Position = projection * vec4(aPos.xyz, 1.0);

    mat3 rotation = mat3(projection[0].xyz, projection[1].xyz, projection[2].xyz); // extract rotation from the projection matrix TODO: change to uniform
    luminance = dot(normalize(rotation * aNorm), vec3(0, 0, -1));

    color = aCol;
    TexCoord = aTexCoord;
}