#include "Controller.hpp"

Controller::Controller( GLFWwindow* window ) : window(window) {
    this->ref = std::chrono::steady_clock::now();
    std::cout << "[w][s] : move forward/backward\n[a][d] : move left/right\n\
[space][shift] : move up/down\n\n[i][k] : scale z axis\n[j][l] : scale x axis\n\
[y][h] : scale y axis\n[-][=] : scale all axes\n\n[m] : switch bones model\n\
[c] : switch free/orbit camera mode\n[1][2][3][4][5][6] : select animation\n\
[esc] : quit\n(left click) : select bone\n";
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
        case eKeyMode::cycle: this->keyCycle(k, value); break;
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

void    Controller::keyCycle( int k, short value ) {
    if (value && getElapsedMilliseconds(this->key[k].last).count() > this->key[k].cooldown) {
        this->key[k].value = (this->key[k].value + 1 >= this->key[k].cycles ? 0 : this->key[k].value + 1);
        this->key[k].last = std::chrono::steady_clock::now();
        this->key[k].stamp = std::chrono::steady_clock::now();
    }
    if (!value)
        this->key[k].last = this->ref;
}

void    Controller::setKeyProperties( int k, eKeyMode type, short sval, uint cooldown, uint cycles ) {
    this->key[k].value = sval;
    this->key[k].type = type;
    this->key[k].cooldown = cooldown;
    this->key[k].cycles = cycles;
}

tMilliseconds   Controller::getElapsedMilliseconds( tTimePoint prev ) {
    return (std::chrono::steady_clock::now() - prev);
}
