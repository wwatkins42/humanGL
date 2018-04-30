#pragma once

#include <glad/glad.h>

#include <cmath>
#include <iostream>
#include <string>
#include <fstream>

#include "Exception.hpp"
#include "Matrix.hpp"
#include "Controller.hpp"

class Camera {

public:
    Camera( float fov, float aspect );
    Camera( const Camera& rhs );
    Camera& operator=( const Camera& rhs );
    ~Camera( void );

    void        handleKeys( const std::array<tKey, N_KEY>& keys, const vec3& lockPos );
    /* Setters */
    void        setFov( float fov );
    void        setAspect( float aspect );
    /* Getters */
    const mat4& getProjectionMatrix( void ) const { return (projectionMatrix); };
    const mat4& getViewMatrix( void ) const { return (viewMatrix); };
    const float getFov( void ) const { return (fov); };
    const float getAspect( void ) const { return (aspect); };
    const vec3& getPosition( void ) const { return (position); };
    const vec3& getTarget( void ) const { return (target); };

    static mat4 createPerspectiveProjectionMatrix( float fov, float aspect, float near = 0.1, float far = 100.0 );

private:
    mat4    projectionMatrix;
    mat4    viewMatrix;
    float   fov;
    float   aspect;
    vec3    position;
    vec3    target;

    vec3    translation;

};
