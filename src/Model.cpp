#include "Model.hpp"

Model::Model( const vec3& position, const vec3& orientation, const vec3& scale, const vec3& joint, const int64_t color ) : position(position), orientation(orientation), scale(scale), joint(joint), color(hex2vec(color)) {
    this->nIndices = 0;
    this->initBufferObjects(GL_STATIC_DRAW, eModelType::sphere);
    this->pushMatrix(mtls::mat4identity);
    this->externalTransform.identity();

    this->worldPosition = vec3({0, 0, 0}); // NEW
}

Model::~Model( void ) {
    glDeleteVertexArrays(1, &this->vao);
    glDeleteBuffers(1, &this->vbo);
    glDeleteBuffers(1, &this->ebo);
    while (!this->stack.empty())
        this->stack.pop();
}

mat4    Model::popMatrix( void ) {
    mat4 top = this->stack.top();
    this->stack.pop();
    return (top);
};

void    Model::update( const mat4& parentTransform, Shader* shader ) {
    this->pushMatrix();
    mtls::translate(this->stack.top(), this->position);
    mtls::rotate(this->stack.top(), this->orientation, this->joint);
    this->stack.top() = this->externalTransform * this->stack.top() * parentTransform;
    this->pushMatrix();
    this->updateWorldPosition(parentTransform); // NEW: needed for raycast
    mtls::scale(this->stack.top(), this->scale + this->scaling);
    this->render(shader);
    this->popMatrix();
}

void    Model::render( Shader* shader ) {
    shader->setVec4UniformValue("customColor", this->color);
    shader->setMat4UniformValue("model", this->stack.top());
    glBindVertexArray(this->vao);
    glDrawElements(GL_TRIANGLES, this->nIndices, GL_UNSIGNED_INT, 0);
}

void    Model::updateWorldPosition( const mat4& parentTransform ) {
    vec4    tmp = static_cast<vec4>(this->position);
    tmp(3) = 1;
    this->worldPosition = static_cast<vec3>(parentTransform.transpose() * tmp);
}

void    Model::initBufferObjects( int mode, eModelType modelType ) {
    std::vector<GLfloat>    vertices;
    std::vector<GLuint>     indices;

    switch (modelType) {
        case eModelType::cube: createCube(vertices, indices); break;
        case eModelType::sphere: createSphere(vertices, indices, 1.0f, 40, 40); break;
        case eModelType::cylinder: createSphere(vertices, indices, 1.2f, 4, 40); break;
        default: break;
    };
    this->nIndices = indices.size();
    // gen buffers and vertex arrays
	glGenVertexArrays(1, &this->vao);
    glGenBuffers(1, &this->vbo);
	glGenBuffers(1, &this->ebo);
    // bind vertex array object, basically this is an object to allow us to not redo all of this process each time
    // we want to draw an object to screen, all the states we set are stored in the VAO
	glBindVertexArray(this->vao);
    // copy our vertices array in a buffer for OpenGL to use
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.size(), vertices.data(), mode);
    // copy our indices array in a buffer for OpenGL to use
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), indices.data(), mode);
    // set the vertex attribute pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), static_cast<GLvoid*>(0));
	glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
