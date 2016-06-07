#include "shaderutils.h"
#include <iostream>
#include <algorithm>
#include <fstream>
#include "lib/glm/ext.hpp"
#include <QFile>
#include <QByteArray>



ShaderUtils::ShaderUtils()
{

}

void ShaderUtils::readFilesShaders(std::string vertex_shader, std::string fragment_shader){
    QFile data(vertex_shader.data());
    if (data.open(QIODevice::ReadOnly)) {
        QByteArray dump = data.readAll();
        strVertexShader = dump.data();
    }
    QFile data2(fragment_shader.data());
    if (data2.open(QIODevice::ReadOnly)) {
        QByteArray dump2 = data2.readAll();
        strFragmentShader = dump2.data();
    }
}

void ShaderUtils::InitializeProgram()
{
    std::vector<GLuint> shaderList;
    /*std::cout << strVertexShader << std::endl;
    std::cout << strFragmentShader << std::endl;*/
    GLuint vertexShader, fragmentShader;
    vertexShader = CreateShader(GL_VERTEX_SHADER, strVertexShader);
    fragmentShader = CreateShader(GL_FRAGMENT_SHADER, strFragmentShader);

    if(!vertexShader || !fragmentShader){
        std::cout << "Unable to initialize shaders!" << std::endl;
        exit(-1);
    }

    shaderList.push_back(vertexShader);
    shaderList.push_back(fragmentShader);

    theProgram = CreateProgram(shaderList);

    std::for_each(shaderList.begin(), shaderList.end(), glDeleteShader);
}


GLuint ShaderUtils::CreateShader(GLenum eShaderType, const std::string &strShaderFile)
{
    if (!glCreateShader){
        std::cout << "Error: glCreateShader is undefined!" << std::endl;
        std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
        return 0;
    }
    GLuint shader = glCreateShader(eShaderType);
    const char *strFileData = strShaderFile.c_str();
    glShaderSource(shader, 1, &strFileData, NULL);
    glCompileShader(shader);
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE)
    {
        GLint infoLogLength;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

        GLchar *strInfoLog = new GLchar[infoLogLength + 1];
        glGetShaderInfoLog(shader, infoLogLength, NULL, strInfoLog);

        const char *strShaderType = NULL;
        switch(eShaderType)
        {
        case GL_VERTEX_SHADER: strShaderType = "vertex"; break;
        case GL_GEOMETRY_SHADER: strShaderType = "geometry"; break;
        case GL_FRAGMENT_SHADER: strShaderType = "fragment"; break;
        }

        fprintf(stderr, "Compile failure in %s shader:\n%s\n", strShaderType, strInfoLog);
        delete[] strInfoLog;
    }

    return shader;
}

GLuint ShaderUtils::CreateProgram(const std::vector<GLuint> &shaderList)
{
    GLuint program = glCreateProgram();

    for(size_t iLoop = 0; iLoop < shaderList.size(); iLoop++)
        glAttachShader(program, shaderList[iLoop]);

    glLinkProgram(program);

    GLint status;
    glGetProgramiv (program, GL_LINK_STATUS, &status);
    if (status == GL_FALSE)
    {
        GLint infoLogLength;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);

        GLchar *strInfoLog = new GLchar[infoLogLength + 1];
        glGetProgramInfoLog(program, infoLogLength, NULL, strInfoLog);
        fprintf(stderr, "Linker failure: %s\n", strInfoLog);
        delete[] strInfoLog;
    }

    for(size_t iLoop = 0; iLoop < shaderList.size(); iLoop++)
        glDetachShader(program, shaderList[iLoop]);

    return program;
}

bool ShaderUtils::setUniform(const char* uniformName, glm::mat4 val){
    GLint loc = glGetUniformLocation(theProgram,uniformName);
    if(loc >= 0){
        glUniformMatrix4fv(loc, 1, GL_FALSE,glm::value_ptr(val));
        return true;
    }
    return false;
}
bool ShaderUtils::setUniform(const char* uniformName, float val){
    GLint loc = glGetUniformLocation(theProgram,uniformName);
    if(loc >= 0){
        glUniform1f(loc, val);
        return true;
    }
    return false;
}
bool ShaderUtils::setUniform(const char* uniformName, glm::vec3 val){
    GLint loc = glGetUniformLocation(theProgram,uniformName);
    if(loc >= 0){
        glUniform3fv(loc, 1, glm::value_ptr(val));
        return true;
    }
    return false;
}

void ShaderUtils::bufferCreate(GLuint& number, std::vector<glm::vec3>& content){
    glGenBuffers(1, &number);
    glBindBuffer(GL_ARRAY_BUFFER, number);
    glBufferData(GL_ARRAY_BUFFER, content.size()*sizeof(glm::vec3), &content[0].x, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void ShaderUtils::bufferCreate(GLuint& number, std::vector<float>& content){
    glGenBuffers(1, &number);
    glBindBuffer(GL_ARRAY_BUFFER, number);
    glBufferData(GL_ARRAY_BUFFER, content.size()*sizeof(float), &content[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ShaderUtils::bufferCreate(GLuint& number, glm::vec3* content, int contentSize){
    glGenBuffers(1, &number);
    glBindBuffer(GL_ARRAY_BUFFER, number);
    glBufferData(GL_ARRAY_BUFFER, contentSize*sizeof(glm::vec3), content, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ShaderUtils::saveAttribute(std::string name){
    GLuint number;
    number = glGetAttribLocation (theProgram, name.c_str());
    std::cout << "saveattribute " << name << " " << number << std::endl;
    attr[name] = number;
}

void ShaderUtils::linkBufferWithAttr(GLuint& buffer, std::string attr_name, int large){
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glEnableVertexAttribArray(attr[attr_name]);
    glVertexAttribPointer(attr[attr_name], large, GL_FLOAT, GL_FALSE, 0, 0);
}
