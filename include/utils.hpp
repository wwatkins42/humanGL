#pragma once

#include "Matrix.hpp"
#include <vector>

void    createSphere(std::vector<GLfloat>& vertices, std::vector<GLuint>& indices, float radius, unsigned int rings, unsigned int sectors);
void    createCube(std::vector<GLfloat>& vertices, std::vector<GLuint>& indices);
vec4    hex2vec( int64_t hex );
