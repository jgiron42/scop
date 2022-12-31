
#version 330 core
out vec3 FragColor;
in vec2 TexCoord;
in float luminance;
uniform sampler2D uTex;
uniform float textureSwitch;
in vec3 color;

void main()
{
    FragColor = (textureSwitch * texture(uTex, TexCoord).rgb + (1. - textureSwitch) * color / 255) * luminance;
//    FragColor = texture(uTex, TexCoord).rgb;
}