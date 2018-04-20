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
    Model( const Model& rhs );
    Model& operator=( const Model& rhs );
    ~Model( void );

    void    update( void );
    void    render( void );

    void    initBufferObjects( int mode = GL_STATIC_DRAW );

    const GLuint& getVao( void ) const { return (vao); };

private:
    GLuint  vao; // Vertex Array Object
    GLuint  vbo; // Vertex Buffer Object
    GLuint  ebo; // Element Buffer Object (or indices buffer object, ibo)

};
