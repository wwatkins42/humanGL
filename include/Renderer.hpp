#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cmath>
#include <iostream>
#include <string>
#include <fstream>
#include <forward_list>

#include "Exception.hpp"
#include "Env.hpp"
#include "Shader.hpp"
#include "Matrix.hpp"

class Renderer {

public:
    Renderer( Env* environment );
    Renderer( const Renderer& rhs );
    Renderer& operator=( const Renderer& rhs );
    ~Renderer( void );

    void	keyHandler( void );
    void	loop( void );

private:
    Env*    env;
    Shader  shader;

    void    updateShaderUniforms( void );
};
