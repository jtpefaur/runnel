#ifndef SHADERUTILS_H
#define SHADERUTILS_H
#include <GL/glew.h>
#include <vector>
#include <string>
#include "lib/glm/glm.hpp"
#include <unordered_map>
#include <QString>

class ShaderUtils
{
    public:
        std::string strVertexShader;
        std::string strFragmentShader;
        std::unordered_map<std::string, GLuint> attr;
        void saveAttribute(std::string name);
        GLuint theProgram;
        ShaderUtils();
        void InitializeProgram();
        GLuint CreateProgram(const std::vector<GLuint> &shaderList);
        void readFilesShaders(std::string vertex_shader, std::string fragment_shader);
        GLuint CreateShader(GLenum eShaderType, const std::string &strShaderFile);
        bool setUniform(const char* uniformName, glm::mat4 val);
        bool setUniform(const char* uniformName, float val);
        bool setUniform(const char* uniformName, glm::vec3 val);
        void bufferCreate(GLuint& number, std::vector<glm::vec3>& content);
        void bufferCreate(GLuint& number, std::vector<float>& content);
        void bufferCreate(GLuint& number, glm::vec3* content, int contentSize);
        void linkBufferWithAttr(GLuint& buffer, std::string attr_name, int large);
};

#endif // SHADERUTILS_H
