#include "Model.hpp"

// NOTE: Change that to a parser of .obj, like in scop but cleaner
// GLfloat vertices[] = { // 24
//     // front
//     -1.0, -1.0,  1.0,
//      1.0, -1.0,  1.0,
//      1.0,  1.0,  1.0,
//     -1.0,  1.0,  1.0,
//     // back
//     -1.0, -1.0, -1.0,
//      1.0, -1.0, -1.0,
//      1.0,  1.0, -1.0,
//     -1.0,  1.0, -1.0,
// };
// GLfloat colors[] = { // 24
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
// };
// GLushort indices[] = { // 36
//     // front
//     0, 1, 2,
//     2, 3, 0,
//     // right
//     1, 5, 6,
//     6, 2, 1,
//     // back
//     7, 6, 5,
//     5, 4, 7,
//     // left
//     4, 0, 3,
//     3, 7, 4,
//     // bottom
//     4, 5, 1,
//     1, 0, 4,
//     // top
//     3, 2, 6,
//     6, 7, 3,
// };

float vertices[] = {
     0.5f,  0.5f, 0.0f,  // top right
     0.5f, -0.5f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f,  // bottom left
    -0.5f,  0.5f, 0.0f   // top left
};
unsigned int indices[] = {  // note that we start from 0!
    0, 1, 3,  // first Triangle
    1, 2, 3   // second Triangle
};

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
    // code for render of this instance
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
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, mode);
    // copy our indices array in a buffer for OpenGL to use
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, mode);
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