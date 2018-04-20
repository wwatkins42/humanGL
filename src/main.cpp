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
    std::cout << A << std::endl;
    std::cout << B << std::endl << std::endl;
    std::cout << A * B << std::endl; // should be [[9, 12], [18, 24]]

    Mat2d<float, 4, 4>  D;
    std::cout << D.identity() << std::endl << std::endl;

    Mat2d<float, 4, 4>  E({
        1, 0, 1, 2,
        0, 0, 0, 1,
        3, 4, 1, 0,
        4, 3, 0, 1
    });
    auto E_T = E.transpose();
    std::cout << E_T << std::endl << std::endl;

    Mat2d<float, 3, 4>  F({
        1, 0, 1, 2,
        0, 0, 0, 1,
        3, 4, 1, 0
    });
    auto F_T = F.transpose();
    std::cout << F_T << std::endl << std::endl;

    vec4f    v4({ 4, 5, 6, 1 });
    std::cout << v4 << std::endl;

    Mat2d<float, 4, 4>  G;
    G.identity();
    G *= 2;

    std::cout << G * v4 << std::endl;

    std::exit(1);

    Env         environment;
    Renderer    renderer(&environment);
    renderer.loop();
    return (0);
}
