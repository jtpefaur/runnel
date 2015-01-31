#version 400
in float color;
uniform vec3 color_angle_point;
out vec4 outputColor;

void main(void)
{
    if(color == 0.0f){
        outputColor = vec4(color_angle_point.x,color_angle_point.y,color_angle_point.z*color,0.0f);
    }else{
        outputColor = vec4(color_angle_point.x,color_angle_point.y,color_angle_point.z*color,1.0f);
    }
}
