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
    Shader( std::string const & vertexShader, std::string const & fragmentShader );
    Shader( Shader const & rhs );
    Shader & operator=( Shader const & rhs );
    ~Shader( void );

    static const std::string    getFromFile( std::string const & filename );
    static GLuint               create( std::string const & filename, GLenum shaderType );
    static GLuint               createProgram( std::forward_list<GLuint> const & shaders );
    static void                 isCompilationSuccess( GLint handle, GLint success, int shaderType );

    void                        use( void ) const;

    template<typename T>
    void    setUniformValue( std::string const & name, T x ) const;
    template<typename T>
    void    setUniformValue( std::string const & name, T x, T y ) const;
    template<typename T>
    void    setUniformValue( std::string const & name, T x, T y, T z ) const;
    template<typename T>
    void    setUniformValue( std::string const & name, T x, T y, T z, T w ) const;
    /* save the uniform location in a hashMap for efficiency */

    GLuint  id;

};
