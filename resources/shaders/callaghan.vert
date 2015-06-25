#version 400
in vec3 position;
in float watercount;
out float color;
uniform mat4 mvp;
uniform float exag;


void main(void)
{
    color = watercount;
    gl_Position  = mvp*vec4(position.x, position.y, position.z*exag, 1.0);
}
