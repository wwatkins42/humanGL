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
    Model( const vec3& translation, const vec3& scale, const vec3& rotation, const vec3& joint, const int64_t color );
    Model( const Model& rhs );
    Model& operator=( const Model& rhs );
    ~Model( void );

    vec4    hex2vec( int64_t hex );

    void    update( const mat4& parentTransform );
    void    render( Shader* shader );

    void    setTranslation( const vec3& t ) { translation = t; };
    void    setScale( const vec3& s ) { scale = s; };
    void    setRotation( const vec3& r ) { rotation = r; };
    void    setJoint( const vec3& j ) { joint = j; };

    void    initBufferObjects( int mode = GL_STATIC_DRAW );

    const GLuint&   getVao( void ) const { return (vao); };
    const mat4&     getTransform( void ) const { return (nst); };

    const vec3&     getTranslation( void ) const { return (translation); };
    const vec3&     getScale( void ) const { return (scale); };
    const vec3&     getRotation( void ) const { return (rotation); };
    const vec3&     getJoint( void ) const { return (joint); };

    // NEW
    void            setExternalTransform( const mat4& transform ) { externalTransform = transform; };
    const mat4&     getExternalTransform( void ) const { return (externalTransform); };

private:
    GLuint      vao; // Vertex Array Object
    GLuint      vbo; // Vertex Buffer Object
    GLuint      ebo; // Element Buffer Object (or indices buffer object, ibo)

    mat4        nst;        // the non-scaled version of the transform matrix
    mat4        transform;  // model matrix (for object transformations)

    vec3        translation;
    vec3        scale;
    vec3        rotation;
    vec3        joint;

    mat4        externalTransform; // NOTE: name temporary

    /*  _those are base values for the model, they are updated relative to the parent local-space:
         | position
         | orientation
         | scale
         | joint
        _those are the transformations, they are updated relative to this model's local-space:
         | translation
         | rotation
         | scaling
         they could be only a simple mat4
         | mat4 externalTransform
    */

    vec4        color;

};
