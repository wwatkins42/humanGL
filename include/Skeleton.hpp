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
    Skeleton( const Skeleton& rhs );
    Skeleton& operator=( const Skeleton& rhs );
    ~Skeleton( void );

    void    update( void );
    void    render( Shader* shader );

    Bone*       getParentBone( void ) const { return (parentBone); };
    std::string getParentBoneId( void ) const { return (parentBoneId); };
    std::unordered_map<std::string, Bone*>  getBones( void ) const { return (bones); };

    Bone*   operator[]( const std::string& id );

private:
    Bone*                                   parentBone;
    std::string                             parentBoneId;
    std::unordered_map<std::string, Bone*>  bones;

};
