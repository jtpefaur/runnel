#version 400
out vec4 outputColor;
uniform vec3 color_terrain;
in float heigthColor;

void main()
{
    outputColor = vec4(color_terrain.x*heigthColor,color_terrain.y*heigthColor,color_terrain.z*heigthColor,1);
}
