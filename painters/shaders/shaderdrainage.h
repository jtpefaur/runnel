#ifndef SHADERDRAINAGE_H
#define SHADERDRAINAGE_H
#include "shaderutils.h"

class ShaderDrainage : public ShaderUtils
{
    public:
        ShaderDrainage();
        int data_buffer_size;
        GLuint position_buffer;
        GLuint color_buffer;
        void fillPositionBuffer(std::vector<glm::vec3>& data, std::vector<glm::vec3>& color);
        void render(glm::mat4 matrix, float exag_z, glm::vec3 color);
};

#endif // SHADERDRAINAGE_H
