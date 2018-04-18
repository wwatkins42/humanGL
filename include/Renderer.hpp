#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <fstream>
#include <forward_list>

#include "Exception.hpp"
#include "Env.hpp"

class Renderer {

public:
    Renderer( Env * environment );
    Renderer( Renderer const & rhs );
    Renderer & operator=( Renderer const & rhs );
    ~Renderer( void );

    void	keyHandler( void );
    void	loop( void );

    static const std::string   getShaderSource( std::string const & filename );
    static GLuint              createShader( std::string const & filename, GLenum shaderType );
    static GLuint              createShaderProgram( std::forward_list<GLuint> const & shaders );
    static void                isCompilationSuccess( GLint handle, GLint success, int shaderType );

private:
    Env     *env;
    GLuint  shaderProgram;

};
