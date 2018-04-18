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
    BodyPart( std::forward_list<BodyPart *> children, std::string const type );
    BodyPart( BodyPart const & rhs );
    BodyPart & operator=( BodyPart const & rhs );
    ~BodyPart( void );

    void    update( void );
    void    render( void );

    std::string const getType( void ) const { return (type); };

private:
    std::string const               type;
    Model                           *model;
    std::forward_list<BodyPart *>   children;

};
