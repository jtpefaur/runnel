#ifndef SHADERCALLAGHAN_H
#define SHADERCALLAGHAN_H
#include "shaderutils.h"

class ShaderCallaghan : public ShaderUtils
{
    public:
        ShaderCallaghan();
        GLuint position_buffer;
        GLuint watercount_buffer;
        int data_buffer_size;
        void fillPositionBuffer(std::vector<glm::vec3>& data, std::vector<float>& count_water);
        void render(glm::mat4 matrix, float exag_z, float linewater, float max_water, glm::vec3 color);
};

#endif // SHADERCALLAGHAN_H
