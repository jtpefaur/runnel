#include "shaderpatron.h"
#include <iostream>

ShaderPatron::ShaderPatron():
    ShaderUtils()
{
    this->readFilesShaders(
                ":shader_file/shaders/arbol_drenaje.vert",
                ":shader_file/shaders/arbol_drenaje.frag");
    this->InitializeProgram();
    this->saveAttribute("position");
    this->saveAttribute("valuecolor");
    buffer_position_arbolitos.clear();
    buffer_color_arbolitos.clear();
    tamano_arbol.clear();
    std::cout << "Initialize ShaderPatron" << std::endl;
}

void ShaderPatron::fillPositionBuffer(std::vector<arbol*> arbolines, std::vector<std::string> names_type) {
    fillPositionBuffer(arbolines, names_type, 1);
}

void ShaderPatron::fillPositionBuffer(std::vector<arbol*> arbolines, std::vector<std::string> names_type, int orderThreshold){
    for (auto buf: buffer_position_arbolitos){
        if(buf != 0){
            glDeleteBuffers(1, &buf );
            buf = 0;
        }
    }

    for (auto buf: buffer_color_arbolitos){
        if(buf != 0){
            glDeleteBuffers(1, &buf );
            buf = 0;
        }
    }
    buffer_position_arbolitos.clear();
    buffer_color_arbolitos.clear();
    tamano_arbol.clear();
    if(arbolines.size() > 0){
        int i = 0;
        for(arbol* aa : arbolines){
            GLuint buffer_arbolito;
            std::vector<runnel::Point*> networkPoints;
            aa->getArbolEdges(networkPoints, orderThreshold);
            std::vector<glm::vec3> ed_ar;
            for (runnel::Point* point : networkPoints) {
                ed_ar.push_back(point->coord);
            }
            this->bufferCreate(buffer_arbolito, ed_ar);
            tamano_arbol.push_back(ed_ar.size());
            buffer_position_arbolitos.push_back(buffer_arbolito);

            GLuint buffer_arbolito_color;
            std::vector<glm::vec3> ed_ar_col;
            if(names_type.size()>0){
                aa->getColorEdgesType(ed_ar_col, names_type[i]);
            }else{
                aa->getColorEdges(ed_ar_col, orderThreshold);
            }
            this->bufferCreate(buffer_arbolito_color, ed_ar_col);
            buffer_color_arbolitos.push_back(buffer_arbolito_color);
            ++i;
        }

    }
}

void ShaderPatron::render(glm::mat4 matrix, float exag_z){
    int i = 0;
    for( GLuint buffer_id: buffer_position_arbolitos){
        if(tamano_arbol[i] > 4){
            glDisable(GL_DEPTH_TEST);
            glLineWidth(3);
            glUseProgram(this->theProgram);
            this->setUniform("mvp", matrix);
            this->setUniform("exag", exag_z);
            this->linkBufferWithAttr(buffer_position_arbolitos[i], "position", 3);
            this->linkBufferWithAttr(buffer_color_arbolitos[i], "valuecolor", 3);
            glDrawArrays(GL_LINES, 0, tamano_arbol[i]);
            glLineWidth(1);
            glEnable(GL_DEPTH_TEST);
        }
        ++i;
    }
}



