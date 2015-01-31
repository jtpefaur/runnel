#include "shaderangleedge.h"

ShaderAngleEdge::ShaderAngleEdge():
    ShaderUtils()
{
    this->readFilesShaders(
                ":shader_file/shaders/angle_edge.vert",
                ":shader_file/shaders/angle_edge.frag");
    this->InitializeProgram();
    this->saveAttribute("position");
    this->saveAttribute("height_angle");
    this->saveAttribute("angle_edge");
    position_buffer = 0;
    angle_buffer = 0;
    height_buffer = 0;
}


void ShaderAngleEdge::fillPositionBuffer(std::vector<glm::vec3>& data, std::vector<glm::vec3>& angle, std::vector<glm::vec3>& height){

    if(position_buffer != 0){
        glDeleteBuffers(1, &position_buffer );
        position_buffer = 0;
    }
    if(angle_buffer != 0){
        glDeleteBuffers(1, &angle_buffer );
        angle_buffer = 0;
    }
    if(height_buffer != 0){
        glDeleteBuffers(1, &height_buffer );
        height_buffer = 0;
    }
    data_buffer_size = data.size();
    this->bufferCreate(position_buffer, data);
    this->bufferCreate(angle_buffer, angle);
    this->bufferCreate(height_buffer, height);
}

void ShaderAngleEdge::render(glm::mat4 matrix, float exag_z,float min_angle, float max_angle, float width_line, glm::vec3 color_min, glm::vec3 color_max){
    glDisable(GL_DEPTH_TEST);
    glLineWidth(3);
    glUseProgram(this->theProgram);
    this->setUniform("mvp", matrix);
    this->setUniform("exag", exag_z);
    this->setUniform("linewidth", width_line);
    this->setUniform("cotamaxangle", max_angle);
    this->setUniform("cotaminangle", min_angle);
    this->setUniform("color_min", color_min);
    this->setUniform("color_max", color_max);
    this->linkBufferWithAttr(position_buffer, "position", 3);
    this->linkBufferWithAttr(angle_buffer, "angle_edge", 3);
    this->linkBufferWithAttr(height_buffer, "height_angle", 3);
    glDrawArrays(GL_TRIANGLES, 0, data_buffer_size);
    glLineWidth(1);
    glEnable(GL_DEPTH_TEST);

}
