#pragma once

#include <iostream>
#include <string>
#include <GLFW/glfw3.h>
#include "Exception.hpp"
#include "Env.hpp"

class Renderer {

public:
    Renderer( Env * environment );
    Renderer( Renderer const & rhs );
    Renderer & operator=( Renderer const & rhs );
    ~Renderer( void );

    void	keyHandler( void );
    void	loop( void );

private:
    Env     * env;

};
