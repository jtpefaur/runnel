#version 130
in vec3 color;
out vec4 outputColor;


void main(void)
{
    outputColor = vec4(color,1);
}
