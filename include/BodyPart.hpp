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
    BodyPart( std::forward_list<BodyPart*> children, const std::string type, const vec3& pos, const vec3& scale, const vec3& rotation = vec3({0, 0, 0}), const vec3& joint = vec3({0, 0, 0}), const int64_t color = 0xFFFFFF );
    BodyPart( const BodyPart& rhs );
    BodyPart& operator=( const BodyPart& rhs );
    ~BodyPart( void );

    void    update( const mat4& transform );
    void    render( Shader* shader );
    void    rescale( const vec3& v, bool child = false );

    const std::string               getType( void ) const { return (type); };
    Model*                          getModel( void ) const { return (model); };
    std::forward_list<BodyPart*>&   getChildren( void ) { return (children); };

private:
    const std::string               type;
    Model*                          model;
    std::forward_list<BodyPart*>    children;

};
/* the joints are as follow :

        0000
        0000
         --
    66/111111\88
    66 111111 88
    66 111111 88
    -- 111111 --
    77 /    \ 99
    77 22  33 99
       22  33
       --  --
       44  55
       44  55
*/
