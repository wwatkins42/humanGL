// #define GLEW_STATIC
// #include <GL/glew.h>
#include "humanGL.hpp"
#include "BodyPart.hpp"

int main( void ) {
    Env         environment;
    Renderer    renderer(&environment);

    environment.getCharacter().getParentPart()->initBufferObjects(GL_STATIC_DRAW); // NOTE: this doesn't segfault

    // renderer.loop();
    return (0);
}
