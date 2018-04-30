#include "Model.hpp"

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

Model::Model( const vec3& position, const vec3& orientation, const vec3& scale, const vec3& joint, const int64_t color ) : position(position), orientation(orientation), scale(scale), joint(joint), color(hex2vec(color)) {
    this->nIndices = 0;
    this->initBufferObjects(GL_STATIC_DRAW);
    /* 1st element: identity */
    this->pushMatrix(mtls::mat4identity);
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

void    Model::update( const mat4& parentTransform, Shader* shader ) {
    /* 2nd element: translate/rotate/parent/external */
    this->pushMatrix();
    mtls::translate(this->stack.top(), this->position);
    mtls::rotate(this->stack.top(), this->orientation, this->joint);
    this->stack.top() = this->externalTransform * this->stack.top() * parentTransform;
    /* 3rd element: scaling */
    this->pushMatrix();
    mtls::scale(this->stack.top(), this->scale + this->scaling);

    this->render(shader);
}

void    Model::render( Shader* shader ) {
    shader->setVec4UniformValue("customColor", this->color);
    shader->setMat4UniformValue("model", this->stack.top());
    glBindVertexArray(this->vao);
    glDrawElements(GL_TRIANGLES, this->nIndices, GL_UNSIGNED_INT, 0);
    this->popMatrix(); /* revert to 2nd element */
}

// void    Model::update( const mat4& parentTransform ) {
//     /* this is the non-scaled transform passed as parentTransform for children */
//     this->nst.identity();
//     mtls::translate(this->nst, this->position);
//     mtls::rotate(this->nst, this->orientation, this->joint);
//     this->nst = this->externalTransform * this->nst * parentTransform;
//     /* the transformation matrix used to display the model */
//     this->transform = this->nst;
//     mtls::scale(this->transform, this->scale + this->scaling);
// }

// void    Model::render( Shader* shader ) {
//     shader->setVec4UniformValue("customColor", this->color);
//     shader->setMat4UniformValue("model", this->transform);
//     glBindVertexArray(this->vao);
//     glDrawElements(GL_TRIANGLES, this->nIndices, GL_UNSIGNED_INT, 0);
// }

void    Model::initBufferObjects( int mode ) {
    std::vector<GLfloat>    vertices;
    std::vector<GLuint>     indices;

    // createCube(vertices, indices);
    createSphere(vertices, indices, 1.2f, 4, 40);
    // createSphere(vertices, indices, 1.25f, 40, 40);

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

vec4    Model::hex2vec( int64_t hex ) {
    return vec4({
        ((hex >> 16) & 0xFF) / 255.0f,
        ((hex >>  8) & 0xFF) / 255.0f,
        ((hex      ) & 0xFF) / 255.0f,
        1
    });
}

/*  for the matrix stack we could do as follow:
    | in the constructor:
    * identity() // base state
    * pushMatrix()

        | in the update method:
        * popMatrix()
        * perform translation and rotation
        * pushMatrix()
        * perform scaling
        * render()

    | in the Bone update method:
    * popMatrix() to get the non-scaled matrix
    * we do the same all over again for the children

*/
