#include "Model.hpp"

// NOTE: Change that to a parser of .obj, like in scop but cleaner
std::array<GLfloat, 24> vertices = {{
    // front
    -0.5, -0.5,  0.5,
     0.5, -0.5,  0.5,
     0.5,  0.5,  0.5,
    -0.5,  0.5,  0.5,
    // back
    -0.5, -0.5, -0.5,
     0.5, -0.5, -0.5,
     0.5,  0.5, -0.5,
    -0.5,  0.5, -0.5,
}};
std::array<unsigned int, 36> indices = {{
    0, 1, 2,  2, 3, 0, // front
    1, 5, 6,  6, 2, 1, // right
    7, 6, 5,  5, 4, 7, // back
    4, 0, 3,  3, 7, 4, // left
    4, 5, 1,  1, 0, 4, // bottom
    3, 2, 6,  6, 7, 3, // top
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

Model::Model( const vec3& pos, const vec3& scale, const vec3& rotation ) : pos(pos), scale(scale), rotation(rotation) {
    this->initBufferObjects(GL_STATIC_DRAW);
    // this->ut.identity();
    // this->ut = mtls::translate(this->ut, this->pos);
    // this->ut = mtls::rotate(this->ut, this->rotation);
    // this->transform = this->ut;
    // this->transform = mtls::scale(this->transform, this->scale);
}

Model::Model( const Model& rhs ) {
    *this = rhs;
}

Model& Model::operator=( const Model& rhs ) {
    (void)rhs;
    return (*this);
}

Model::~Model( void ) {
    glDeleteVertexArrays(1, &this->vao);
    glDeleteBuffers(1, &this->vbo);
    glDeleteBuffers(1, &this->ebo);
}

void    Model::update( const mat4& parentTransform ) {
    /* this is the non-scaled transform passed as parentTransform for children */
    this->ut.identity();
    this->ut = mtls::translate(this->ut, this->pos);
    this->ut = mtls::rotate(this->ut, this->rotation, vec3({0, this->scale[1]/2, 0})); // this offset value should be modular
    this->ut = this->ut * parentTransform;
    /* the transformation matrix used to display the model */
    this->transform.identity();
    this->transform = mtls::translate(this->transform, this->pos);
    this->transform = mtls::rotate(this->transform, this->rotation, vec3({0, this->scale[1]/2, 0}) );
    this->transform = mtls::scale(this->transform, this->scale);
    this->transform = this->transform * parentTransform;
}

void    Model::render( Shader* shader ) {
    shader->setMat4UniformValue("model", this->transform);
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
