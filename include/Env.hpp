#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <unordered_map>
#include <regex>

#include "Exception.hpp"
#include "Animator.hpp"
#include "Skeleton.hpp"
#include "Bone.hpp"

typedef struct  s_window {
    GLFWwindow* ptr;
    int         width;
    int         height;
}               t_window;

class Env {

public:
    Env( void );
    Env( const Env& rhs );
    Env&    operator=( const Env& rhs );
    ~Env( void );

    const t_window& getWindow( void ) const { return (window); };
    Skeleton*       getCharacter( void ) { return (character); };
    Animator*       getAnimator( void ) { return (animator); };

private:
    t_window    window;
    Skeleton*   character;
    Animator*   animator;

    void        initGlfwEnvironment( const std::string& glVersion = "4.0" );
    void        initGlfwWindow( size_t width, size_t height );
    // callback to be called each time the window is resized to update the viewport size as well
    static void framebufferSizeCallback( GLFWwindow* window, int width, int height );

    std::unordered_map<std::string, Bone*>  createBetterCharacterSkeleton( void );
    std::unordered_map<std::string, Bone*>  createCharacterSkeleton( void );
    tAnimationFrames*                       createIdleAnimation( void );
    tAnimationFrames*                       createWalkingAnimation( void );
    tAnimationFrames*                       createBetterWalkingAnimation( void );
    tAnimationFrames*                       createJumpingAnimation( void );
};
