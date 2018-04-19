#include "humanGL.hpp"
#include "BodyPart.hpp"

int main( void ) {
    Env         environment;
    Renderer    renderer(&environment);
    renderer.loop();
    return (0);
}
