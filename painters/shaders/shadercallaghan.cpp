#include "shadercallaghan.h"

ShaderCallaghan::ShaderCallaghan():
    ShaderUtils()
{
    this->readFilesShaders(
                ":shader_file/shaders/callaghan.vert",
                ":shader_file/shaders/callaghan.frag");
    this->InitializeProgram();
    this->saveAttribute("position");
    this->saveAttribute("watercount");
    position_buffer = 0;
    watercount_buffer = 0;
}


void ShaderCallaghan::fillPositionBuffer(std::vector<glm::vec3>& data, std::vector<float>& count_water){
    if(position_buffer != 0){
        glDeleteBuffers(1, &position_buffer );
        position_buffer = 0;
    }
    if(watercount_buffer != 0){
        glDeleteBuffers(1, &watercount_buffer );
        watercount_buffer = 0;
    }

    data_buffer_size = data.size();
    this->bufferCreate(position_buffer, data);
    this->bufferCreate(watercount_buffer, count_water);

}


void ShaderCallaghan::render(glm::mat4 matrix, float exag_z, float linewater, float max_water,  glm::vec3 color ){
    glLineWidth(3);
    glUseProgram(this->theProgram);
    this->setUniform("exag", exag_z);
    this->setUniform("mvp", matrix);
    this->setUniform("color_callaghan",color);

    this->setUniform("linewater", linewater);

    this->setUniform("maxwater", max_water);
    this->linkBufferWithAttr(position_buffer, "position", 3);
    this->linkBufferWithAttr(watercount_buffer, "watercount", 1);
    glDrawArrays(GL_LINES, 0, data_buffer_size);
    glLineWidth(1);
}

