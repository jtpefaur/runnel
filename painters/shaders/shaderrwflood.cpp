#include "shaderrwflood.h"

ShaderRWFlood::ShaderRWFlood():
    ShaderUtils()
{
    this->readFilesShaders(
                ":shader_file/shaders/rwflood.vert",
                ":shader_file/shaders/rwflood.frag");
    this->InitializeProgram();
    this->saveAttribute("position");
    this->saveAttribute("inputColor");
    dataBufferSize = 0;
    positionBuffer = 0;
    colorBuffer = 0;
}

void ShaderRWFlood::fillBuffers(std::vector<glm::vec3> coords,
                                std::vector<glm::vec3> colors)
{
    dataBufferSize = coords.size();

    if (positionBuffer != 0) {
        glDeleteBuffers(1, &positionBuffer);
        positionBuffer = 0;
    }
    this->bufferCreate(positionBuffer, coords);

    if (colorBuffer != 0) {
        glDeleteBuffers(1, &colorBuffer);
        colorBuffer = 0;
    }
    this->bufferCreate(colorBuffer, colors);
}

void ShaderRWFlood::render(glm::mat4 matrix, float exag)
{
    glLineWidth(3);
    glUseProgram(this->theProgram);
    this->setUniform("exag", exag);
    this->setUniform("mvp", matrix);
    this->linkBufferWithAttr(positionBuffer, "position", 3);
    this->linkBufferWithAttr(colorBuffer, "inputColor", 3);
    glDrawArrays(GL_LINES, 0, dataBufferSize);
    glLineWidth(1);
}
