#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <array>
#include <forward_list>

#include "Exception.hpp"
#include "Model.hpp"
#include "Matrix.hpp"
#include "Shader.hpp"

class Bone {

public:
    Bone( std::forward_list<Bone*> children, const std::string name, const vec3& position, const vec3& orientation, const vec3& scale, const vec3& joint = vec3({0, 0, 0}), const int64_t color = 0xFFFFFF );
    Bone( const Bone& rhs );
    Bone& operator=( const Bone& rhs );
    ~Bone( void );

    void    update( const mat4& transform );
    void    render( Shader* shader );
    void    rescale( const vec3& v, bool child = false );

    const std::string               getName( void ) const { return (name); };
    Model*                          getModel( void ) const { return (model); };
    const std::forward_list<Bone*>& getChildren( void ) const { return (children); };

private:
    const std::string           name;
    Model*                      model;
    std::forward_list<Bone*>    children;

};
