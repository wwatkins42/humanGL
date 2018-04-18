#pragma once

#include <glad/glad.h>
// #define GLFW_INCLUDE_GLCOREARB
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <regex>

#include "Exception.hpp"
#include "Character.hpp"

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

    t_window const  & getWindow( void ) const { return (window); };
    Character       & getCharacter( void ) { return (character); };

private:
    t_window    window;
    Character   character;

};
