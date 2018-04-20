#include "Model.hpp"

// NOTE: Change that to a parser of .obj, like in scop but cleaner
std::array<GLfloat, 24> vertices = {{
    // front
    -1.0, -1.0,  1.0,
     1.0, -1.0,  1.0,
     1.0,  1.0,  1.0,
    -1.0,  1.0,  1.0,
    // back
    -1.0, -1.0, -1.0,
     1.0, -1.0, -1.0,
     1.0,  1.0, -1.0,
    -1.0,  1.0, -1.0,
}};
std::array<unsigned int, 36> indices = {{
    // front
    0, 1, 2,
    2, 3, 0,
    // right
    1, 5, 6,
    6, 2, 1,
    // back
    7, 6, 5,
    5, 4, 7,
    // left
    4, 0, 3,
    3, 7, 4,
    // bottom
    4, 5, 1,
    1, 0, 4,
    // top
    3, 2, 6,
    6, 7, 3,
}};
// std::array<GLfloat, 24> colors = {{
//     // front colors
//     1.0, 0.0, 0.0,
//     0.0, 1.0, 0.0,
//     0.0, 0.0, 1.0,
//     1.0, 1.0, 1.0,
//     // back colors
//     1.0, 0.0, 0.0,
//     0.0, 1.0, 0.0,
//     0.0, 0.0, 1.0,
//     1.0, 1.0, 1.0,
// }};

// std::array<float, 12> vertices = {{
//      0.5f,  0.5f, 0.0f,  // top right
//      0.5f, -0.5f, 0.0f,  // bottom right
//     -0.5f, -0.5f, 0.0f,  // bottom left
//     -0.5f,  0.5f, 0.0f   // top left
// }};
// std::array<unsigned int, 6> indices = {{  // note that we start from 0!
//     0, 1, 3,  // first Triangle
//     1, 2, 3   // second Triangle
// }};

Model::Model( void ) {
    this->initBufferObjects(GL_STATIC_DRAW);
}

Model::Model( Model const & rhs ) {
    *this = rhs;
}

Model & Model::operator=( Model const & rhs ) {
    (void)rhs;
    return (*this);
}

Model::~Model( void ) {
    glDeleteVertexArrays(1, &this->vao);
    glDeleteBuffers(1, &this->vbo);
    glDeleteBuffers(1, &this->ebo);
}

void    Model::update( void ) {
    // code for update of this instance
}

void    Model::render( void ) {
    /* NOTE: will also contain the code for rendering relative to the updated position of the model */
    // glBindTexture(GL_TEXTURE_2D, env.buffer.texture);
    glBindVertexArray(this->vao);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}

void    Model::initBufferObjects( int mode ) {
    // gen buffers and vertex arrays
	glGenVertexArrays(1, &this->vao);
    glGenBuffers(1, &this->vbo);
	glGenBuffers(1, &this->ebo);
    // bind vertex array object, basically this is an object to allow us to not redo all of this process each time
    // we want to draw an object to screen, all the states we set are stored in the VAO
	glBindVertexArray(this->vao);
    // copy our vertices array in a buffer for OpenGL to use
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), mode);
    // copy our indices array in a buffer for OpenGL to use
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), mode);
    // set the vertex attribute pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), static_cast<GLvoid*>(0));
	glEnableVertexAttribArray(0);
	// glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), static_cast<GLvoid*>(0));
	// glEnableVertexAttribArray(0);
	// glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<GLvoid*>(3 * sizeof(GLfloat)));
	// glEnableVertexAttribArray(1); // CAREFUL WITH REINTERPRET_CAST
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
