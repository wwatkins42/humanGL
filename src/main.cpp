#include "Renderer.hpp"
#include "Env.hpp"

int main( void ) {
    Env         environment;
    Renderer    renderer(&environment);
    renderer.loop();
    return (0);
}
