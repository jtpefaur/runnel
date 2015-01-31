#version 400
in vec3 position;
in vec3 height_angle;
in vec3 angle_edge;
out vec3 height;
out vec3 angles;
uniform mat4 mvp;
uniform float exag;

void main(void)
{
    height = height_angle;
    angles = angle_edge;
    gl_Position  = mvp*vec4(position.x, position.y, position.z*exag, 1.0);
}
