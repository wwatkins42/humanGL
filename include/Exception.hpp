#pragma once

#include <sstream>
#include <string>

namespace Exception {
    class InitError : public std::exception {

    public:
        InitError( std::string const & str ) {
            std::stringstream ss;
            ss << "InitError : " << str;
            msg = ss.str();
        }
        virtual const char* what() const noexcept {
            return (msg.c_str());
        }

    private:
        std::string msg;
    };

    class ShaderError : public std::exception {

    public:
        ShaderError( int type, std::string const & err ) {
            std::stringstream ss;
            if (type == GL_VERTEX_SHADER)
                ss << "SHADER::VERTEX::COMPILATION_FAILED_";
            else if (type == GL_FRAGMENT_SHADER)
                ss << "SHADER::FRAGMENT::COMPILATION_FAILED_";
            else
                ss << "SHADER::PROGRAM::COMPILATION_FAILED_";
            ss << err;
            msg = ss.str();
        }
        virtual const char* what() const noexcept {
            return (msg.c_str());
        }

    private:
        std::string msg;
    };

}
