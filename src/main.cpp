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

    vec4    v({ 4, 5, 6, 1 });
    std::cout << v << std::endl;

    Mat2d<float, 4, 4>  G;
    G.identity();
    G *= 2;
    std::cout << G * v << std::endl;

    /* MATRIX MULTIPLICATION IS NOT COMMUTATIVE */
    Mat2d<float, 4, 4>  scale({
        6, 0, 0, 0,
        0, 6, 0, 0,
        0, 0, 6, 0,
        0, 0, 0, 1
    });
    Mat2d<float, 4, 4>  translate({
        1, 0, 0, 1,
        0, 1, 0, 2,
        0, 0, 1, 3,
        0, 0, 0, 1
    });
    std::cout << scale * translate << std::endl; // those two are different
    std::cout << translate * scale << std::endl; // the order is important

    std::cout << translate * scale * vec4({0.5, 0.1, 0.4, 1}) << std::endl; // should be [[4], [2.6], [5.4], [1]]


    vec4    test({1, 0, 0, 1});
    mat4    trans;

    trans = mtls::translate(trans, vec3({ 1, 1, 0 }));
    trans = mtls::rotate(trans, mtls::radians(90), vec3({ 0, 0, 1 }));
    trans = mtls::scale(trans, vec3({ 0.5, 0.5, 0.5 }));

    std::cout << trans * test << std::endl;

    // matrix::rotate(vec4({0.5, 0.1, 0.4, 1}), matrix::radians(90), vec3());

    std::exit(1);
    //===========================================

    Env         environment;
    Renderer    renderer(&environment);
    renderer.loop();
    return (0);
}
