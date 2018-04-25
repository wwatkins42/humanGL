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
    Model( const vec3& pos, const vec3& scale, const vec3& rotation, const vec3& joint, const int64_t color );
    Model( const Model& rhs );
    Model& operator=( const Model& rhs );
    ~Model( void );

    vec4    hex2vec( int64_t hex );

    void    update( const mat4& parentTransform );
    void    render( Shader* shader );

    void    setPos( const vec3& p ) { pos = p; };
    void    setScale( const vec3& s ) { scale = s; };
    void    setRotation( const vec3& r ) { rotation = r; };
    void    setJoint( const vec3& j ) { joint = j; };

    void    initBufferObjects( int mode = GL_STATIC_DRAW );

    const GLuint&   getVao( void ) const { return (vao); };
    const mat4&     getTransform( void ) const { return (nst); };

    const vec3&     getPos( void ) const { return (pos); };
    const vec3&     getScale( void ) const { return (scale); };
    const vec3&     getRotation( void ) const { return (rotation); };
    const vec3&     getJoint( void ) const { return (joint); };

private:
    GLuint      vao; // Vertex Array Object
    GLuint      vbo; // Vertex Buffer Object
    GLuint      ebo; // Element Buffer Object (or indices buffer object, ibo)

    mat4        nst;        // the non-scaled version of the transform matrix
    mat4        transform;  // model matrix (for object transformations)
    /* decomposed values for transformations */
    vec3        pos;
    vec3        scale;
    vec3        rotation;
    vec3        joint;      // the center of rotation and used for rescaling of parts

    vec4        color;

};
