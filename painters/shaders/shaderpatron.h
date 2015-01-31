#ifndef SHADERPATRON_H
#define SHADERPATRON_H
#include "shaderutils.h"
#include "../../drainageAlgorithms/arbol.h"

class ShaderPatron : public ShaderUtils
{
    public:
        ShaderPatron();
        int data_buffer_size;
        std::vector<GLuint> buffer_position_arbolitos;
        std::vector<GLuint> buffer_color_arbolitos;
        std::vector<int> tamano_arbol;
        void fillPositionBuffer(std::vector<arbol*> arbolines,std::vector<std::string> names_type);
        void render(glm::mat4 matrix, float exag_z);
};

#endif // SHADERPATRON_H
