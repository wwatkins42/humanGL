#pragma once

#include <iostream>
#include <string>
#include <array>
#include <forward_list>
#include <GLFW/glfw3.h>
#include "Exception.hpp"

// typedef struct  s_model {
//     std::array<GLfloat, 24> vertices;
//     std::array<GLfloat, 36> indices;
//     std::array<GLfloat, 24> colors;
// }               t_model;

class BodyPart {

public:
    BodyPart( std::forward_list<BodyPart *> children, std::string const type );
    BodyPart( BodyPart const & rhs );
    BodyPart & operator=( BodyPart const & rhs );
    ~BodyPart( void );

    void    update( void );
    void    render( void );

    std::string const getType( void ) const { return (type); };

private:
    std::string const               type; // good for debug
    std::forward_list<BodyPart *>   children;
    /* eventualy we could add colors and texture to individual body parts */
    static const GLfloat    vertices[];
    static const GLushort   indices[];
    static const GLfloat    colors[];

    void        initBufferObjects( int mode = GL_STATIC_DRAW );
    GLuint      vao; // vertex array object
    GLuint      vbo; // vertex buffer object
    GLuint      ibo; // index buffer object (or ebo sometimes)
};
