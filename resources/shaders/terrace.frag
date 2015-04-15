#version 400
out vec4 outputColor;
in vec3 color;

void main(void)
{
    outputColor = vec4(color, 1.0f);
}

