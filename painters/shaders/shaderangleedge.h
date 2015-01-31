#ifndef SHADERANGLEEDGE_H
#define SHADERANGLEEDGE_H
#include "shaderutils.h"

class ShaderAngleEdge : public ShaderUtils
{
    public:
        ShaderAngleEdge();
        GLuint position_buffer;
        GLuint height_buffer;
        GLuint angle_buffer;
        int data_buffer_size;
        void fillPositionBuffer(std::vector<glm::vec3>& data, std::vector<glm::vec3>& angle, std::vector<glm::vec3>& height);
        void render(glm::mat4 matrix, float exag_z,float min_angle, float max_angle, float width_line, glm::vec3 color_min, glm::vec3 color_max);
};

#endif // SHADERANGLEEDGE_H
