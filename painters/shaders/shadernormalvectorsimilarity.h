#ifndef SHADERNORMALVECTORSIMILARITY_H
#define SHADERNORMALVECTORSIMILARITY_H
#include "shaderutils.h"

class ShaderNormalVectorSimilarity : public ShaderUtils
{
public:
    ShaderNormalVectorSimilarity();
    GLuint position_buffer;
    int data_buffer_size;
    void fillPositionBuffer(std::vector<glm::vec3>& data);
    void render(glm::mat4 matrix, float exag_z, glm::vec3 color);
};

#endif // SHADERNORMALVECTORSIMILARITY_H
