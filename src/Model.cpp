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

Model::Model( const vec3& translation, const vec3& scale, const vec3& rotation, const vec3& joint, const int64_t color ) : translation(translation), scale(scale), rotation(rotation), joint(joint), color(hex2vec(color)) {
    this->initBufferObjects(GL_STATIC_DRAW);
    this->externalTransform.identity();
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
    this->nst.identity();
    this->nst = mtls::translate(this->nst, this->translation);
    this->nst = mtls::rotate(this->nst, this->rotation, this->joint);
    // this->nst = this->nst *  parentTransform;
    this->nst = this->externalTransform * this->nst * parentTransform;
    /* the transformation matrix used to display the model */
    this->transform = this->nst;
    this->transform = mtls::scale(this->transform, this->scale);
}

void    Model::render( Shader* shader ) {
    shader->setVec4UniformValue("customColor", this->color);
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
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

vec4    Model::hex2vec( int64_t hex ) {
    return vec4({
        ((hex >> 16) & 0xFF) / 255.0f,
        ((hex >>  8) & 0xFF) / 255.0f,
        ((hex      ) & 0xFF) / 255.0f,
        1
    });
}
