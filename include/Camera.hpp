#pragma once

#include <glad/glad.h>

#include <cmath>
#include <iostream>
#include <string>
#include <fstream>
#include <chrono>

#include "Exception.hpp"
#include "Matrix.hpp"
#include "Controller.hpp"

typedef std::chrono::duration<double,std::milli> tMilliseconds;
typedef std::chrono::steady_clock::time_point tTimePoint;

class Camera {

public:
    Camera( float fov, float aspect );
    Camera( const Camera& rhs );
    Camera& operator=( const Camera& rhs );
    ~Camera( void );

    vec3            interpolate( const vec3& v0, const vec3& v1, tTimePoint last, size_t duration = 250 );
    tMilliseconds   getElapsedMilliseconds( tTimePoint last );

    void            handleKeys( const std::array<tKey, N_KEY>& keys, const vec3& lockPos );
    /* Setters */
    void            setFov( float fov );
    void            setAspect( float aspect );
    /* Getters */
    const mat4&     getProjectionMatrix( void ) const { return (projectionMatrix); };
    const mat4&     getViewMatrix( void ) const { return (viewMatrix); };
    const float     getFov( void ) const { return (fov); };
    const float     getAspect( void ) const { return (aspect); };
    const vec3&     getPosition( void ) const { return (position); };
    const vec3&     getTarget( void ) const { return (target); };

    static mat4     createPerspectiveProjectionMatrix( float fov, float aspect, float near = 0.1, float far = 100.0 );

private:
    mat4    projectionMatrix;
    mat4    viewMatrix;
    float   fov;
    float   aspect;
    vec3    position;
    vec3    target;

};
