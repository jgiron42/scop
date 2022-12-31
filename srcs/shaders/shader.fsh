
#version 330 core
out vec4 FragColor;
in vec3 Color;

void main()
{
    FragColor = (1 - gl_FragCoord.z) * vec4((Color.x / 255), (Color.y / 255), (Color.z / 255), 1.0f);
}