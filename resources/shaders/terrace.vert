#version 400
in vec3 position;
uniform mat4 mvp;
uniform float exag;
uniform vec3 valuecolor;
out vec3 color;

void main(void)
{
    gl_Position  = mvp*vec4(position.x, 1.0-position.y, position.z*exag, 1.0);
    color = valuecolor;
}

