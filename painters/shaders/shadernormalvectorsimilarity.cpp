#include "shadernormalvectorsimilarity.h"

ShaderNormalVectorSimilarity::ShaderNormalVectorSimilarity():
    ShaderUtils()
{
    this->readFilesShaders(
                ":shader_file/shaders/terrace.vert",
                ":shader_file/shaders/terrace.frag");
    this->InitializeProgram();
    this->saveAttribute("position");
    position_buffer = 0;
    data_buffer_size = 0;
}

void ShaderNormalVectorSimilarity::fillPositionBuffer(std::vector<glm::vec3>& data){
    if(position_buffer != 0){
        glDeleteBuffers(1, &position_buffer );
        position_buffer = 0;
    }
    data_buffer_size = data.size();
    this->bufferCreate(position_buffer, data);
}

void ShaderNormalVectorSimilarity::render(glm::mat4 matrix, float exag_z, glm::vec3 color){
    if (data_buffer_size > 0){
        glDisable(GL_DEPTH_TEST);
        glUseProgram(this->theProgram);
        this->setUniform("exag", exag_z);
        this->setUniform("mvp", matrix);
        this->setUniform("valuecolor", color);
        this->linkBufferWithAttr(position_buffer, "position", 3);
        glDrawArrays(GL_TRIANGLES, 0, data_buffer_size);
        glEnable(GL_DEPTH_TEST);
    }
}
