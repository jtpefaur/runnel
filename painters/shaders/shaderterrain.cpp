#include "shaderterrain.h"

ShaderTerrain::ShaderTerrain():
    ShaderUtils()
{
    this->readFilesShaders(
                ":shader_file/shaders/terrain.vert",
                ":shader_file/shaders/terrain.frag");
    this->InitializeProgram();
    this->saveAttribute("position");
    position_buffer = 0;
}

void ShaderTerrain::fillPositionBuffer(std::vector<glm::vec3>& data){

    if(position_buffer != 0){
        glDeleteBuffers(1, &position_buffer );
        position_buffer = 0;
    }
    data_buffer_size = data.size();
    this->bufferCreate(position_buffer, data);
}

void ShaderTerrain::render(glm::mat4 matrix, float exag_z, float max_bound_z, glm::vec3 color){
    glUseProgram(this->theProgram);
    this->setUniform("mvp", matrix);
    this->setUniform("promz",max_bound_z);
    this->setUniform("exag",exag_z);
    this->setUniform("color_terrain", color );
    this->linkBufferWithAttr(position_buffer, "position", 3);
    glDrawArrays(GL_TRIANGLES, 0, data_buffer_size);
}
