#pragma once

#include <GLFW/glfw3.h>
#include "Env.hpp"
#include "Renderer.hpp"

/*  Project architecture:
    [MANDATORY]
    | class Env:
      -> will init the GLFW and OpenGL environment/windows

    | class Renderer:
      -> will handle the runtime rendering (will contain a Env instance)

    | class Matrix:
      -> will implement a Matrix class which will contains all the necessary methods for matrix manipulation
         in the context of this project

    | class Character:
      -> will contain the hierarchies of body parts

    | class BodyPart:
      -> A body part

    | class Animator:
      -> will animate the body parts defined in Character

    [BONUS]
    | class Camera:
      -> will be used to move around in the scene


    Animations will be sequence of frames that define rotations and translations for the bones of the skeletal model
    and with interpolations between those frames so we can modify the playback speed of the animation.
*/
