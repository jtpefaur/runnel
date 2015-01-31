#version 400
in vec3 position;
uniform vec3 valuecolor;
uniform mat4 mvp;
uniform float exag;
out vec3 color;

void main(void)
{
    color = valuecolor;
    gl_Position  = mvp*vec4(position.x, position.y, position.z*exag, 1.0);
}
