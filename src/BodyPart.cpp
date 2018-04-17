#include "BodyPart.hpp"

const GLfloat BodyPart::vertices[] = { // 24
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
};
const GLfloat BodyPart::colors[] = { // 24
    // front colors
    1.0, 0.0, 0.0,
    0.0, 1.0, 0.0,
    0.0, 0.0, 1.0,
    1.0, 1.0, 1.0,
    // back colors
    1.0, 0.0, 0.0,
    0.0, 1.0, 0.0,
    0.0, 0.0, 1.0,
    1.0, 1.0, 1.0,
};
const GLushort BodyPart::indices[] = { // 36
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
};

BodyPart::BodyPart( std::forward_list<BodyPart *> children, std::string const type ) : type(type), children(children) {
    /* DEBUG */
    std::cout << type << " : ";
    for (std::forward_list<BodyPart *>::iterator it = children.begin(); it != children.end(); ++it)
        if (*it) std::cout << (*it)->getType() << ", ";
    std::cout << std::endl;
    // initBufferObjects(GL_STATIC_DRAW); // SEGFAULT
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

void    BodyPart::initBufferObjects( int mode ) {
    // (void)mode;
    // gen buffers and vertex arrays
    glGenBuffers(1, &this->vbo);
	glGenBuffers(1, &this->ibo);
	glGenVertexArrays(1, &this->vao);
    // bind vertex array object
	glBindVertexArray(this->vao);
    // bind vertex buffer object and copy
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
	glBufferData(GL_ARRAY_BUFFER, 24, this->vertices, mode);
    // bind index buffer object and copy
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36, this->indices, mode);
    // set the vertex attribute pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), static_cast<GLvoid*>(0));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
}
