#version 130
in vec3 color;
out vec4 fragmentColor;

void main(void)
{
    fragmentColor = vec4(color, 1.0f);
}
