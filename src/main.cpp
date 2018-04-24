#include "humanGL.hpp"
#include "Matrix.hpp"

int main( void ) {
    // mat4    mat({
    //     4, 0, 0, 3,
    //     0, 4, 0, 5,
    //     0, 0, 4, 2,
    //     0, 0, 0, 1
    // });
    // mat = mtls::inverse(mat);
    // std::cout << mat << std::endl;

    // mat3    m1({
    //     4, 0, 3,
    //     0, 4, 5,
    //     2, 0, 4,
    // });
    // mat3    m2({
    //     1, 1, 1,
    //     7, 2, 0,
    //     0, 0, 1,
    // });
    // std::cout << m1 * m2 << std::endl;
    // std::exit(1);

    Env         environment;
    Renderer    renderer(&environment);
    renderer.loop();
    return (0);
}
