#version 400

in vec3 height;
in vec3 angles;
uniform float linewidth;
uniform float cotamaxangle;
uniform float cotaminangle;
uniform vec3 color_min;
uniform vec3 color_max;
out vec4 outputColor;

void main(void)
{
    outputColor = vec4(height, 1);
    int minIndex = 0;
    float minHeight = 100000.0f;
    for(int i = 0;i<3;i++){
        if(height[i] < minHeight){
            minHeight = height[i];
            minIndex = i;
        }
    }
    float closestEdgeAngle = angles[minIndex];
    if(minHeight < linewidth){
        if(closestEdgeAngle > cotamaxangle){
            outputColor = vec4(color_max,1);
        }else if(closestEdgeAngle < cotaminangle){
            outputColor = vec4(color_min,1);
        }else{
            outputColor = vec4(0,0,0,0);
        }
    }else{
        outputColor = vec4(0,0,0,0);
    }
}
