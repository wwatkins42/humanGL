#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cmath>
#include <iostream>
#include <string>
#include <array>
#include <chrono>

#include "Exception.hpp"
#include "Env.hpp"
#include "Animator.hpp"
#include "Camera.hpp"
#include "Matrix.hpp"

#define N_KEY GLFW_KEY_LAST + 1
#define N_MOUSE_BUTTON GLFW_MOUSE_BUTTON_LAST + 1

typedef std::chrono::duration<double,std::milli> tMilliseconds;
typedef std::chrono::steady_clock::time_point tTimePoint;

namespace keyMode {
    enum eKeyMode {
        press,   /* default */
        toggle,  /* press to switch the key state */
        cooldown /* press and the state remains on for so long */
    };
}

typedef struct  sKey {
    short               value = 0;
    keyMode::eKeyMode   type = keyMode::press;
    uint                cooldown = 250;
    tTimePoint          last = std::chrono::steady_clock::now();
}               tKey;

typedef struct  sMouse {
    vec2d                               pos;
    std::array<short, N_MOUSE_BUTTON>   button;
}               tMouse;

class Controller {

public:
    Controller( Env* environment );
    ~Controller( void );

    void    mouseHandler( void );
    void	keyHandler( void );

    void    keyUpdate( int k );
    void    keyToggle( int k, int value );
    void    keyCooldown( int k, int value );

    void            setKeyProperties( int k, keyMode::eKeyMode type, uint cooldown );
    tMilliseconds   getElapsedMilliseconds( tTimePoint prev );

    short           getKeyValue( int k ) const { return (key[k].value); };
    short           getMouseButtonValue( int b ) const { return (mouse.button[b]); };
    const vec2d&    getMousePosition( void ) const { return (mouse.pos); };

private:
    Env*                    env;
    std::array<tKey, N_KEY> key;
    tTimePoint              ref;
    tMouse                  mouse;

};
