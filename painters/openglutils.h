#ifndef OPENGLUTILS_H
#define OPENGLUTILS_H
#include <string>
#include <GL/glew.h>
#include <map>

class OpenGLUtils
{
	public:
        static bool printOpenGLError();

        static std::string errorToString(GLenum error);


};

#endif // OPENGLUTILS_H
