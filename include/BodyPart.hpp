#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <array>
#include <forward_list>

#include "Exception.hpp"
#include "Model.hpp"

class BodyPart {

public:
    BodyPart( std::forward_list<BodyPart*> children, const std::string type );
    BodyPart( const BodyPart& rhs );
    BodyPart& operator=( const BodyPart& rhs );
    ~BodyPart( void );

    void    update( void );
    void    render( void );

    const std::string   getType( void ) const { return (type); };
    Model*              getModel( void ) const { return (model); };

private:
    const std::string               type;
    Model*                          model;
    std::forward_list<BodyPart*>    children;

};
