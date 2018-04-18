#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <array>
#include <forward_list>

#include "Exception.hpp"

class Model {

public:
    Model( void );
    Model( Model const & rhs );
    Model & operator=( Model const & rhs );
    ~Model( void );

    void    update( void );
    void    render( void );

    void    initBufferObjects( int mode = GL_STATIC_DRAW );

private:
    GLuint  vao; // Vertex Array Object
    GLuint  vbo; // Vertex Buffer Object
    GLuint  ibo; // Indice Buffer Object

};
