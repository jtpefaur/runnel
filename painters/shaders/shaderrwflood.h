#ifndef SHADERRWFLOOD_H
#define SHADERRWFLOOD_H

#include "shaderutils.h"

class ShaderRWFlood : public ShaderUtils
{
public:
    ShaderRWFlood();

    std::size_t dataBufferSize;
    GLuint positionBuffer;
    GLuint colorBuffer;

    void fillBuffers(std::vector<glm::vec3>, std::vector<glm::vec3>);
    void render(glm::mat4, float);
};

#endif // SHADERRWFLOOD_H
