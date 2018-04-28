#include "Controller.hpp"

Controller::Controller( Env* env ) : env(env) {
    this->ref = std::chrono::steady_clock::now();
    this->setKeyProperties(GLFW_KEY_SPACE, keyMode::toggle, 250);
}

Controller::~Controller( void ) {
}

void    Controller::mouseHandler( void ) {
    glfwGetCursorPos(this->env->getWindow().ptr, &(this->mouse.pos(0)), &(this->mouse.pos(1)));
    for (size_t b = 0; b < N_MOUSE_BUTTON; ++b)
        this->mouse.button[b] = (glfwGetMouseButton(this->env->getWindow().ptr, b) == GLFW_PRESS);
}

void    Controller::keyHandler( void ) {
	if (glfwGetKey(this->env->getWindow().ptr, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(this->env->getWindow().ptr, GL_TRUE);
    for (size_t k = 0; k < N_KEY; ++k)
        this->keyUpdate(k);
}

void	Controller::keyUpdate( int k ) {
    const short value = (glfwGetKey(this->env->getWindow().ptr, k) == GLFW_PRESS);
    switch (this->key[k].type) {
        case keyMode::toggle: this->keyToggle(k, value); break;
        case keyMode::cooldown: this->keyCooldown(k, value); break;
        default: this->key[k].value = value; break;
    };
}

void    Controller::keyToggle( int k, int value ) {
    if (value && getElapsedMilliseconds(this->key[k].last).count() > this->key[k].cooldown) {
        this->key[k].value = ~(this->key[k].value) & 0x1;
        this->key[k].last = std::chrono::steady_clock::now();
    }
    if (!value)
        this->key[k].last = this->ref;
}

void    Controller::keyCooldown( int k, int value ) {
    if (value && !this->key[k].value) {
        this->key[k].value = 1;
        this->key[k].last = std::chrono::steady_clock::now();
    }
    if (getElapsedMilliseconds(this->key[k].last).count() > this->key[k].cooldown)
        this->key[k].value = 0;
}

void    Controller::setKeyProperties( int k, keyMode::eKeyMode type, uint cooldown ) {
    this->key[k].type = type;
    this->key[k].cooldown = cooldown;
}

tMilliseconds   Controller::getElapsedMilliseconds( tTimePoint prev ) {
    return (std::chrono::steady_clock::now() - prev);
}
