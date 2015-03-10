#include "shadergradient.h"

ShaderGradient::ShaderGradient():
    ShaderUtils()
{
    this->readFilesShaders(
                ":shader_file/shaders/gradient.vert",
                ":shader_file/shaders/gradient.frag");
    this->InitializeProgram();
    this->saveAttribute("position");
    position_buffer = 0;
    data_buffer_size = 0;
}
void ShaderGradient::fillPositionBuffer(std::vector<glm::vec3>& data){
    if(position_buffer != 0){
        glDeleteBuffers(1, &position_buffer );
        position_buffer = 0;
    }
    data_buffer_size = data.size();
    this->bufferCreate(position_buffer, data);
}


void ShaderGradient::render(glm::mat4 matrix, float exag_z, glm::vec3 color, bool disable_depth_test ){
    if (data_buffer_size > 0){
        if ( disable_depth_test ){
            glDisable(GL_DEPTH_TEST);
        }
        glLineWidth(3);
        glUseProgram(this->theProgram);
        this->setUniform("exag", exag_z);
        this->setUniform("mvp", matrix);
        this->setUniform("valuecolor", color);
        this->linkBufferWithAttr(position_buffer, "position", 3);
        glDrawArrays(GL_LINES, 0, data_buffer_size);
        glLineWidth(1);
        if ( disable_depth_test ){
            glEnable(GL_DEPTH_TEST);
        }
    }
}
