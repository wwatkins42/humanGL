#pragma once

#include <iostream>
#include <string>
#include <regex>
#include <GLFW/glfw3.h>
#include "Exception.hpp"

typedef struct  s_window {
    GLFWwindow  *ptr;
    int         width;
    int         height;
}               t_window;

class Env {

public:
    Env( void );
    Env( Env const & rhs );
    Env & operator=( Env const & rhs );
    ~Env( void );

    void	initGlfwEnvironment( std::string const & glVersion = "4.0" );
    void	initGlfwWindow( size_t width, size_t height );

private:
    t_window    window;
};
