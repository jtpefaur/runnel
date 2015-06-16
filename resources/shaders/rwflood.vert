#version 400
in vec3 position;
in vec3 inputColor;
uniform float exagValue;
uniform mat4 mvp;
out vec3 color;

void main(void)
{
    gl_Position = mvp*vec4(position.x, 1.0-position.y, position.z*exagValue, 1.0);
    color = inputColor;
}

