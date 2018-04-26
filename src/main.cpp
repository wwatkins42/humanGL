#include "humanGL.hpp"
#include "Matrix.hpp"

int main( void ) {
    // mat4    mat({ // this is a classical tranlation matrix in openGL
    //     1, 0, 0, 0,
    //     0, 1, 0, 0,
    //     0, 0, 1, 0,
    //     3, 3, 0, 1
    // });
    // vec4    vec({
    //     1, 2, 3, 1
    // });
    // std::cout << mat.transpose() * vec << std::endl; // [4, 5, 3, 1]

    // vec4    vec({
    //     1, 2, 3, 1
    // });
    // vec3    t = static_cast<vec3>(vec);
    // std::cout << t << std::endl;
    //
    // vec4    b({
    //     4, 5, 2, 1
    // });
    // std::cout << mtls::multiply(static_cast<vec4>(t), b) << std::endl;
    // std::cout << b.multiply(static_cast<vec4>(t)) << std::endl;

    Env         environment;
    Renderer    renderer(&environment);
    renderer.loop();
    return (0);
}
