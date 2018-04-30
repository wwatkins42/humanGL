#include "Bone.hpp"

Bone::Bone( std::forward_list<Bone*> children, const std::string& name, const vec3& position, const vec3& orientation, const vec3& scale, const vec3& joint, const int64_t color ) : name(name), children(children) {
    this->model = new Model(position, orientation, scale, joint, color);
}

Bone::Bone( const Bone& rhs ) {
    *this = rhs;
}

Bone&   Bone::operator=( const Bone& rhs ) {
    this->name = rhs.getName();
    this->model = rhs.getModel();
    this->children = rhs.getChildren();
    return (*this);
}

Bone::~Bone( void ) {
    for (std::forward_list<Bone*>::iterator it = this->children.begin(); it != this->children.end(); ++it)
        delete *it;
    delete this->model;
}

void    Bone::rescale( const vec3& v, bool child ) {
    if (child == false) {
        // compute the position change for the new scale relative to the joint
        vec3 d = (v - this->model->getScaling()) * 0.5f;
        vec3 n = d.multiply(mtls::sign(this->model->getJoint()));
        // update the translation/joint/scale values for the model
        this->model->setPosition(this->model->getPosition() - n);
        this->model->setJoint(this->model->getJoint() + n);
        this->model->setScaling(v);
        // apply the changes to the children
        for (std::forward_list<Bone*>::iterator it = this->children.begin(); it != this->children.end(); ++it)
            if (*it) (*it)->rescale(d, true);
    } else {
        // the changes on the children are relative to their position (as it is in the parent local-space)
        vec3 n = v.multiply(mtls::sign(this->model->getPosition()));
        this->model->setPosition(this->model->getPosition() + n);
    }
}

void    Bone::update( const mat4& transform, Shader* shader ) {
    this->model->update(transform, shader);
    const mat4& parentTransform = this->model->getTransform();
    for (std::forward_list<Bone*>::iterator it = this->children.begin(); it != this->children.end(); ++it)
        if (*it) (*it)->update(parentTransform, shader);
    this->model->popMatrix(); /* revert to identity */
}
