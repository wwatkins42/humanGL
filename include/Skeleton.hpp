#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <unordered_map>

#include "Exception.hpp"
#include "Bone.hpp"
#include "Shader.hpp"

class Skeleton {

public:
    Skeleton( std::unordered_map<std::string, Bone*> bones, const std::string& parentBoneId );
    ~Skeleton( void );

    void        update( void );

    /* setters */
    void                                            setShader( Shader* s ) { shader = s; };
    /* getters */
    const Bone*                                     getParentBone( void ) const { return (parentBone); };
    const std::string&                              getParentBoneId( void ) const { return (parentBoneId); };
    const std::unordered_map<std::string, Bone*>&   getBones( void ) const { return (bones); };
    Shader*                                         getShader( void ) const { return (shader); };

    Bone*   operator[]( const std::string& id );

private:
    Bone*                                   parentBone;
    std::string                             parentBoneId;
    std::unordered_map<std::string, Bone*>  bones;
    Shader*                                 shader;

};
