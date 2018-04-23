#pragma once

#include <glad/glad.h>
// #include <GLFW/glfw3.h>

#include <cmath>
#include <iostream>
#include <string>
#include <fstream>

#include "Exception.hpp"
#include "Matrix.hpp"

namespace projection {
    enum eProjectionType {
        orthographic,
        perspective,
    };
}

class Camera {

public:
    Camera( float fov, float aspect, short projectionType = projection::perspective );
    Camera( const Camera& rhs );
    Camera& operator=( const Camera& rhs );
    ~Camera( void );

    /* Setters */
    void    setFov( float fov );
    void    setAspect( float aspect );
    /* Getters */
    mat4&   getProjectionMatrix( void ) { return (projectionMatrix); };
    mat4&   getViewMatrix( void ) { return (viewMatrix); };
    float   getFov( void ) const { return (fov); };
    float   getAspect( void ) const { return (aspect); };

    static mat4 createPerspectiveProjectionMatrix( float fov, float aspect, float near = 0.1, float far = 100.0 );
    static mat4 createOrthographicProjectionMatrix( float aspect, float near = 0.1, float far = 100.0 );

private:
    mat4    projectionMatrix;
    mat4    viewMatrix;

    float   fov;
    float   aspect;

};
