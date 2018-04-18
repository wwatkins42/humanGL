#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <fstream>

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

private:
    Env     *env;
    GLuint  vertexShader;
    GLuint  fragmentShader;
    GLuint  shaderProgram;

    const std::string   getShaderSource( std::string const & filename );
    GLuint              createShader( std::string const & filename, GLenum shaderType );
    GLuint              createShaderProgram( GLuint vertexShader, GLuint fragmentShader );

    void                isShaderCompilationSuccess( GLint handle, GLint success, int shaderType );
};
