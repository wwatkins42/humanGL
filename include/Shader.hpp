#pragma once

#include <glad/glad.h>

#include <cmath>
#include <iostream>
#include <string>
#include <fstream>
#include <forward_list>

#include "Exception.hpp"

class Shader {

public:
    Shader( const std::string& vertexShader, const std::string& fragmentShader );
    Shader( const Shader& rhs );
    Shader& operator=( const Shader& rhs );
    ~Shader( void );

    static const std::string    getFromFile( const std::string& filename );
    static GLuint               create( const std::string& filename, GLenum shaderType );
    static GLuint               createProgram( const std::forward_list<GLuint>& shaders );
    static void                 isCompilationSuccess( GLint handle, GLint success, int shaderType );

    void                        use( void ) const;

    template<typename T>
    void    setUniformValue( const std::string& name, T x ) const;
    template<typename T>
    void    setUniformValue( const std::string& name, T x, T y ) const;
    template<typename T>
    void    setUniformValue( const std::string& name, T x, T y, T z ) const;
    template<typename T>
    void    setUniformValue( const std::string& name, T x, T y, T z, T w ) const;
    /* save the uniform location in a hashMap for efficiency */

    GLuint  id;

};
