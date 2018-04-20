#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <regex>

#include "Exception.hpp"
#include "Character.hpp"

typedef struct  s_window {
    GLFWwindow* ptr;
    int         width;
    int         height;
}               t_window;

class Env {

public:
    Env( void );
    Env( const Env& rhs );
    Env&    operator=( const Env& rhs );
    ~Env( void );

    const t_window& getWindow( void ) const { return (window); };
    Character*      getCharacter( void ) { return (character); };

private:
    t_window    window;
    Character*  character;

    void	       initGlfwEnvironment( const std::string& glVersion = "4.0" );
    void	       initGlfwWindow( size_t width, size_t height );
    // callback to be called each time the window is resized to update the viewport size as well
    static void    framebufferSizeCallback( GLFWwindow* window, int width, int height );
};
