#version 400
attribute vec3 position;
attribute float normalizedWaterCount;
uniform float exagValue;
uniform mat4 mvp;
out vec3 color;

void main(void)
{
    gl_Position = mvp*vec4(position.x, 1.0-position.y, position.z*exagValue, 1.0);
    color = vec3(1.0-normalizedWaterCount, 1.0-normalizedWaterCount, 1.0);
}

