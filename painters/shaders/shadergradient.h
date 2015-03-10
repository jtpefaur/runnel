#ifndef SHADERGRADIENT_H
#define SHADERGRADIENT_H
#include "shaderutils.h"

class ShaderGradient : public ShaderUtils
{
    public:
        ShaderGradient();
        GLuint position_buffer;
        int data_buffer_size;
        void fillPositionBuffer(std::vector<glm::vec3>& data);
        void render(glm::mat4 matrix, float exag_z, glm::vec3 color, bool disable_depth_test = true);
};

#endif // SHADERGRADIENT_H
