#include "BodyPart.hpp"

BodyPart::BodyPart( std::forward_list<BodyPart*> children, const std::string type, const vec3& pos, const vec3& scale, const vec3& rotation, const vec3& joint, const int64_t color ) : type(type), children(children) {
    this->model = new Model(pos, scale, rotation, joint, color);
}

BodyPart::BodyPart( const BodyPart& rhs ) {
    *this = rhs;
}

BodyPart&   BodyPart::operator=( const BodyPart& rhs ) {
    (void)rhs;
    return (*this);
}

BodyPart::~BodyPart( void ) {
    for (std::forward_list<BodyPart*>::iterator it = this->children.begin(); it != this->children.end(); ++it)
        delete *it;
    delete this->model;
}

void    BodyPart::rescale( const vec3& v, bool child ) {
    if (child == false) {
        // compute the position change for the new scale relative to the joint
        vec3 d = (v - this->model->getScale()) * 0.5f;
        vec3 n = d.multiply(mtls::sign(this->model->getJoint()));
        // update the pos/joint/scale values for the model
        this->model->setPos(this->model->getPos() - n);
        this->model->setJoint(this->model->getJoint() + n);
        this->model->setScale(v);
        // apply the changes to the children
        for (std::forward_list<BodyPart*>::iterator it = this->children.begin(); it != this->children.end(); ++it)
            if (*it) (*it)->rescale(d, true);
    } else {
        vec3 n = v.multiply(mtls::sign(this->model->getPos()));
        this->model->setPos(this->model->getPos() + n);
    }
}

void    BodyPart::update( const mat4& transform ) {
    this->model->update(transform); // update the transform of THIS model
    for (std::forward_list<BodyPart*>::iterator it = this->children.begin(); it != this->children.end(); ++it)
        if (*it) (*it)->update(this->model->getTransform()); // update the transform of the CHILDS models
}

void    BodyPart::render( Shader* shader ) {
    for (std::forward_list<BodyPart*>::iterator it = this->children.begin(); it != this->children.end(); ++it)
        if (*it) (*it)->render(shader);
    this->model->render(shader);
}
