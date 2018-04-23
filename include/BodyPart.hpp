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

    void    update( void );
    void    render( Shader* shader );

    const std::string   getType( void ) const { return (type); };
    Model*              getModel( void ) const { return (model); };

private:
    const std::string               type;
    Model*                          model;
    std::forward_list<BodyPart*>    children;

};
