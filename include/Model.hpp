#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <array>
#include <forward_list>

#include "Exception.hpp"
#include "Matrix.hpp"
#include "Shader.hpp"

class Model {

public:
    Model( const vec3& pos, const vec3& scale );
    Model( const Model& rhs );
    Model& operator=( const Model& rhs );
    ~Model( void );

    void    update( void );
    void    render( Shader* shader );

    void    initBufferObjects( int mode = GL_STATIC_DRAW );

    const GLuint& getVao( void ) const { return (vao); };

private:
    GLuint  vao; // Vertex Array Object
    GLuint  vbo; // Vertex Buffer Object
    GLuint  ebo; // Element Buffer Object (or indices buffer object, ibo)

    // maybe we also keep the local matrix ?
    mat4    matrix; // model matrix (for object translation)

};
