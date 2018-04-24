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

class BodyPart {

public:
    BodyPart( std::forward_list<BodyPart*> children, const std::string type, const vec3& pos, const vec3& scale );
    BodyPart( const BodyPart& rhs );
    BodyPart& operator=( const BodyPart& rhs );
    ~BodyPart( void );

    // void    update( const vec3& pPos = vec3({0, 0, 0}), const vec3& pScale = vec3({0, 0, 0}) );
    void    update( const mat4& transform );
    void    render( Shader* shader );

    // void    resize( const vec3& v, bool child = false );

    const std::string   getType( void ) const { return (type); };
    Model*              getModel( void ) const { return (model); };

private:
    const std::string               type;
    Model*                          model;
    std::forward_list<BodyPart*>    children;

};
