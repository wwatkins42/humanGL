#include "humanGL.hpp"
#include "Matrix.hpp"

int main( void ) {
    Mat2d<float, 2, 3>  A({
        1, 1, 1,
        2, 2, 2
    });
    Mat2d<float, 3, 2>  B({
        1, 2,
        3, 4,
        5, 6
    });
    // std::cout << B(1, 2) << std::endl;
    std::cout << A << std::endl;
    std::cout << B << std::endl << std::endl;
    // std::cout << A * B << std::endl;
    std::exit(1);

    Env         environment;
    Renderer    renderer(&environment);
    renderer.loop();
    return (0);
}
