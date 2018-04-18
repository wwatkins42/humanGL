#pragma once

#include <glad/glad.h>
// #define GLFW_INCLUDE_GLCOREARB
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>

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
    Env     * env;

};
