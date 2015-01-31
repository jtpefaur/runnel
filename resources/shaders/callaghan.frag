#version 400
in float color;
uniform float linewater;
uniform float maxwater;
uniform vec3 color_callaghan;

out vec4 outputColor;

void main(void)
{
    if(color/maxwater > linewater){
        outputColor = vec4(color_callaghan,1);
    }else{
        outputColor = vec4(0,0,0,0);
    }
}

