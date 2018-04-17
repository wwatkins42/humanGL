#pragma once

#include <iostream>
#include <string>
#include <forward_list>
#include <GLFW/glfw3.h>
#include "Exception.hpp"

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
    std::string const               type; // good for debug
    std::forward_list<BodyPart *>   children;
    /* eventualy we could add colors and texture to individual body parts */
};
