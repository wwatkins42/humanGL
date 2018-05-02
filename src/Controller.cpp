#include "Controller.hpp"

Controller::Controller( GLFWwindow* window ) : window(window) {
    this->ref = std::chrono::steady_clock::now();
}

Controller::~Controller( void ) {
}

void    Controller::update( void ) {
    this->mouseHandler();
    this->keyHandler();
}

void    Controller::mouseHandler( void ) {
    glfwGetCursorPos(this->window, &(this->mouse.pos(0)), &(this->mouse.pos(1)));
    for (size_t b = 0; b < N_MOUSE_BUTTON; ++b)
        this->mouse.button[b] = (glfwGetMouseButton(this->window, b) == GLFW_PRESS);
}

void    Controller::keyHandler( void ) {
	if (glfwGetKey(this->window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(this->window, GL_TRUE);
    for (size_t k = 0; k < N_KEY; ++k)
        this->keyUpdate(k);
}

void	Controller::keyUpdate( int k ) {
    const short value = (glfwGetKey(this->window, k) == GLFW_PRESS);

    switch (this->key[k].type) {
        case eKeyMode::toggle: this->keyToggle(k, value); break;
        case eKeyMode::cooldown: this->keyCooldown(k, value); break;
        case eKeyMode::instant: this->keyInstant(k, value); break;
        default: this->key[k].value = value; break;
    };
}

void    Controller::keyToggle( int k, short value ) {
    if (value && getElapsedMilliseconds(this->key[k].last).count() > this->key[k].cooldown) {
        this->key[k].value = ~(this->key[k].value) & 0x1;
        this->key[k].last = std::chrono::steady_clock::now();
        this->key[k].stamp = std::chrono::steady_clock::now();
    }
    /* so that when we unpress the key we can switch immediatly */
    if (!value)
        this->key[k].last = this->ref;
}

void    Controller::keyCooldown( int k, short value ) {
    if (value && !this->key[k].value) {
        this->key[k].value = 1;
        this->key[k].last = std::chrono::steady_clock::now();
        this->key[k].stamp = std::chrono::steady_clock::now();
    }
    if (getElapsedMilliseconds(this->key[k].last).count() > this->key[k].cooldown)
        this->key[k].value = 0;
}

void    Controller::keyInstant( int k, short value ) {
    if (this->key[k].value && getElapsedMilliseconds(this->key[k].last).count() <= this->key[k].cooldown)
        this->key[k].value = 0;
    if (value && !this->key[k].value && getElapsedMilliseconds(this->key[k].last).count() > this->key[k].cooldown) {
        this->key[k].value = 1;
        this->key[k].last = std::chrono::steady_clock::now();
        this->key[k].stamp = std::chrono::steady_clock::now();
    }
}

void    Controller::setKeyProperties( int k, eKeyMode type, uint cooldown ) {
    this->key[k].type = type;
    this->key[k].cooldown = cooldown;
}

tMilliseconds   Controller::getElapsedMilliseconds( tTimePoint prev ) {
    return (std::chrono::steady_clock::now() - prev);
}
/*
          [w][s] : move forward/backward
          [a][d] : move left/right
  [space][shift] : move up/down

          [i][k] : scale z axis
          [j][l] : scale x axis
          [y][h] : scale y axis
          [-][=] : scale all axes

             [m] : switch bones model
             [c] : switch free/orbit camera mode
    [1][2][3][4] : select animation
           [esc] : quit
    (left click) : select bone
*/
