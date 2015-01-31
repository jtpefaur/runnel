#ifndef SHADERTERRAIN_H
#define SHADERTERRAIN_H
#include "shaderutils.h"

class ShaderTerrain : public ShaderUtils
{
    public:
        ShaderTerrain();
        GLuint position_buffer;
        int data_buffer_size;
        void fillPositionBuffer(std::vector<glm::vec3>& data);
        void render(glm::mat4 matrix, float exag_z, float max_bound_z, glm::vec3 color);
};

#endif // SHADERTERRAIN_H
