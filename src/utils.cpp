#include "utils.hpp"

inline void push_indices(std::vector<GLuint>& indices, int sectors, int r, int s) {
    const int currRow = r * sectors;
    const int nextRow = (r + 1) * sectors;
    const int nextS = (s + 1) % sectors;
    indices.push_back(currRow + s);
    indices.push_back(nextRow + s);
    indices.push_back(nextRow + nextS);
    indices.push_back(currRow + s);
    indices.push_back(nextRow + nextS);
    indices.push_back(currRow + nextS);
}

void    createSphere(std::vector<GLfloat>& vertices, std::vector<GLuint>& indices, float radius, unsigned int rings, unsigned int sectors) {
    float const R = 1.0f / static_cast<float>(rings - 1);
    float const S = 1.0f / static_cast<float>(sectors - 1);
    vec3    v;
    for (size_t r = 0; r < rings; ++r) {
        for (size_t s = 0; s < sectors; ++s) {
            v = vec3({
                static_cast<float>(std::cos(2 * M_PI * s * S) * std::sin(M_PI * r * R)),
                static_cast<float>(std::sin(-M_PI_2 + M_PI * r * R)),
                static_cast<float>(std::sin(2 * M_PI * s * S) * std::sin(M_PI * r * R))
            });
            v = v * radius;
            vertices.push_back(v[0]);
            vertices.push_back(v[1]);
            vertices.push_back(v[2]);
            if (r < rings-1)
                push_indices(indices, sectors, r, s);
        }
    }
}

void    createCube(std::vector<GLfloat>& vertices, std::vector<GLuint>& indices) {
    vertices = {{
        -0.5, -0.5,  0.5,
         0.5, -0.5,  0.5,
         0.5,  0.5,  0.5,
        -0.5,  0.5,  0.5,
        -0.5, -0.5, -0.5,
         0.5, -0.5, -0.5,
         0.5,  0.5, -0.5,
        -0.5,  0.5, -0.5,
    }};
    indices = {{
        0, 1, 2,  2, 3, 0,
        1, 5, 6,  6, 2, 1,
        7, 6, 5,  5, 4, 7,
        4, 0, 3,  3, 7, 4,
        4, 5, 1,  1, 0, 4,
        3, 2, 6,  6, 7, 3,
    }};
}

vec4    hex2vec( int64_t hex ) {
    return vec4({
        ((hex >> 16) & 0xFF) / 255.0f,
        ((hex >>  8) & 0xFF) / 255.0f,
        ((hex      ) & 0xFF) / 255.0f,
        1
    });
}

vec2    mousePosToClipSpace( const vec2d& pos, int winWidth, int winHeight ) {
    vec2    mouse = vec2({(float)pos[0] / winWidth, (float)pos[1] / winHeight}) * 2.0f - 1.0f;
    mouse(1) = -mouse[1];
    return (mouse);
}
