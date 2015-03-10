#include "shaderdrainage.h"

ShaderDrainage::ShaderDrainage():
    ShaderUtils()
{
    this->readFilesShaders(
                ":shader_file/shaders/drainage.vert",
                ":shader_file/shaders/drainage.frag");
    this->InitializeProgram();
    this->saveAttribute("position");
    this->saveAttribute("valuecolor");
    position_buffer = 0;
    color_buffer = 0;

}
void ShaderDrainage::fillPositionBuffer(std::vector<glm::vec3>& data, std::vector<glm::vec3>& color){

    if(position_buffer != 0){
        glDeleteBuffers(1, &position_buffer );
        position_buffer = 0;
    }
    if(color_buffer != 0){
        glDeleteBuffers(1, &color_buffer );
        color_buffer = 0;
    }
    data_buffer_size = data.size();
    this->bufferCreate(position_buffer, data);
    this->bufferCreate(color_buffer, color);
}

void ShaderDrainage::render(glm::mat4 matrix, float exag_z, glm::vec3 color, int size_line){
    glDisable(GL_DEPTH_TEST);
    glLineWidth(size_line);
    glUseProgram(this->theProgram);
    this->setUniform("exag", exag_z);
    this->setUniform("mvp", matrix);
    this->setUniform("color_drainage", color);
    this->linkBufferWithAttr(position_buffer, "position", 3);
    this->linkBufferWithAttr(color_buffer, "valuecolor", 3);
    glDrawArrays(GL_LINES, 0, data_buffer_size);
    glLineWidth(1);
    glEnable(GL_DEPTH_TEST);
}
