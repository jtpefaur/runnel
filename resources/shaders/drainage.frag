#version 130
in vec3 color;
out vec4 outputColor;
uniform vec3 color_drainage;

void main(void)
{
    outputColor = vec4(color_drainage.x*color.x, color_drainage.y*color.y, color_drainage.z*color.z,1);
}
