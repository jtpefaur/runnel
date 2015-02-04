#include "openglutils.h"
#include <iostream>
#include <fstream>
//#include "Utils/fileutils.h"
//#include "Common/Constants.h"
#include <QFileInfo>

bool OpenGLUtils::printOpenGLError(){
    GLenum errorCode = glGetError();
    if(errorCode!=GL_NO_ERROR){
        std::cout << OpenGLUtils::errorToString(errorCode) << std::endl;
        return false;//ERRORS
    }
    return true;
}
std::string OpenGLUtils::errorToString(GLenum error){
    switch(error){
        case GL_NO_ERROR:{
            return "No error has been recorded. The value of this symbolic constant is guaranteed to be 0.";
        }

        case GL_INVALID_ENUM:{
            return "An unacceptable value is specified for an enumerated argument. The offending command is ignored and has no other side effect than to set the error flag.";
        }

        case GL_INVALID_VALUE:{
            return "A numeric argument is out of range. The offending command is ignored and has no other side effect than to set the error flag.";
        }

        case GL_INVALID_OPERATION:{
            return "The specified operation is not allowed in the current state. The offending command is ignored and has no other side effect than to set the error flag.";
        }

        case GL_STACK_OVERFLOW:{
            return "This command would cause a stack overflow. The offending command is ignored and has no other side effect than to set the error flag.";
        }

        case GL_STACK_UNDERFLOW:{
            return "This command would cause a stack underflow. The offending command is ignored and has no other side effect than to set the error flag.";
        }

        case GL_OUT_OF_MEMORY:{
            return "There is not enough memory left to execute the command. The state of the GL is undefined, except for the state of the error flags, after this error is recorded.";
        }

        case GL_TABLE_TOO_LARGE:{
            return "The specified table exceeds the implementation's maximum supported table size. The offending command is ignored and has no other side effect than to set the error flag.:";
        }
        default:{
            return "Unknown Error.";
        }
    }
}
