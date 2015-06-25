
#version 400
in vec3 position;
uniform mat4 mvp;
uniform float promz;
uniform float exag;
out float heigthColor;

void main()
{
    heigthColor = position.z/promz;
    gl_Position  = mvp*vec4(position.x, position.y, position.z*exag, 1.0);

}
