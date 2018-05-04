#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <unordered_map>
#include <regex>

#include "Exception.hpp"
#include "Controller.hpp"
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
    ~Env( void );

    const t_window& getWindow( void ) const { return (window); };
    Skeleton*       getCharacter( void ) { return (character); };
    Animator*       getAnimator( void ) { return (animator); };
    Controller*     getController( void ) { return (controller); };

private:
    t_window    window;
    Controller* controller;
    Animator*   animator;
    Skeleton*   character;

    void        initGlfwEnvironment( const std::string& glVersion = "4.0" );
    void        initGlfwWindow( size_t width, size_t height );
    void        setupController( void );
    // callback to be called each time the window is resized to update the viewport size as well
    static void framebufferSizeCallback( GLFWwindow* window, int width, int height );

    std::unordered_map<std::string, Bone*>  createCharacterSkeleton( void );
    tAnimationFrames*                       createIdleAnimation( void );
    tAnimationFrames*                       createWalkingAnimation( void );
    tAnimationFrames*                       createJumpingAnimation( void );
    tAnimationFrames*                       createBackflipAnimation( void );
    tAnimationFrames*                       createBackflipConnectedAnimation( void );
};
