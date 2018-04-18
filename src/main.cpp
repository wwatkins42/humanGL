// #define GLEW_STATIC
// #include <GL/glew.h>
#include "humanGL.hpp"

int main( void ) {
    Env         environment;
    // glewInit();
    Renderer    renderer(&environment);
    renderer.loop();
    return (0);
}
