#include "BodyPart.hpp"

BodyPart::BodyPart( std::forward_list<BodyPart *> children, std::string const type ) : type(type), children(children) {
    this->model = new Model();
    model->initBufferObjects();
}

BodyPart::BodyPart( BodyPart const & rhs ) {
    *this = rhs;
}

BodyPart & BodyPart::operator=( BodyPart const & rhs ) {
    (void)rhs;
    return (*this);
}

BodyPart::~BodyPart( void ) {
    for (std::forward_list<BodyPart *>::iterator it = this->children.begin(); it != this->children.end(); ++it)
        delete *it; // something like that
    // delete this->model;
}

void    BodyPart::update( void ) {
    for (std::forward_list<BodyPart *>::iterator it = this->children.begin(); it != this->children.end(); ++it)
        if (*it) (*it)->update();
    // code for update of this instance
}

void    BodyPart::render( void ) {
    for (std::forward_list<BodyPart *>::iterator it = this->children.begin(); it != this->children.end(); ++it)
        if (*it) (*it)->render();
    // code for render of this instance
}

// void    BodyPart::initBufferObjects( int mode ) {
//     GLuint      vao = 0; // vertex array object
//     GLuint      vbo = 0; // vertex buffer object
//     GLuint      ibo = 0; // index buffer object (or ebo sometimes)
//
//     // gen buffers and vertex arrays
//     glGenBuffers(1, &vbo);
// 	glGenBuffers(1, &ibo);
// 	glGenVertexArrays(1, &vao);
//     // bind vertex array object, basically this is an object to allow us to not redo all of this process each time
//     // we want to draw an object to screen, all the states we set are stored in the VAO
// 	glBindVertexArray(vao);
//     // copy our vertices array in a buffer for OpenGL to use
// 	glBindBuffer(GL_ARRAY_BUFFER, vbo);
// 	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, mode);
//     // copy our indices array in a buffer for OpenGL to use
// 	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
// 	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, mode);
//     // set the vertex attribute pointers
// 	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), static_cast<GLvoid*>(0));
// 	glEnableVertexAttribArray(0);
// 	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<GLvoid*>(3 * sizeof(GLfloat)));
// 	glEnableVertexAttribArray(1); // CAREFUL WITH REINTERPRET_CAST
// }
