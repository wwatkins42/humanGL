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
    Model( const vec3& pos, const vec3& scale, const vec3& rotation );
    Model( const Model& rhs );
    Model& operator=( const Model& rhs );
    ~Model( void );

    void    update( const mat4& parentTransform );
    void    render( Shader* shader );

    void    setScale( const vec3& s ) { scale = s; };
    void    setPos( const vec3& p ) { pos = p; };
    void    setRotation( const vec3& r ) { rotation = r; };

    void    initBufferObjects( int mode = GL_STATIC_DRAW );

    const GLuint&   getVao( void ) const { return (vao); };
    const vec3&     getPos( void ) const { return (pos); };
    const vec3&     getScale( void ) const { return (scale); };
    const mat4&     getTransform( void ) const { return (ut); };

private:
    GLuint      vao; // Vertex Array Object
    GLuint      vbo; // Vertex Buffer Object
    GLuint      ebo; // Element Buffer Object (or indices buffer object, ibo)

    mat4        ut;         // the non-scaled version of the transfornm matrix
    mat4        transform;  // model matrix (for object transformations)
    
    vec3        pos;
    vec3        scale;
    vec3        rotation;

};
