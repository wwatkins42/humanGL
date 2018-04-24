#include "BodyPart.hpp"

BodyPart::BodyPart( std::forward_list<BodyPart*> children, const std::string type, const vec3& pos, const vec3& scale ) : type(type), children(children) {
    this->model = new Model(pos, scale, vec3({0, 0, 0}));
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

// void    BodyPart::resize( const vec3& v, bool child ) {
// }
/*  we have a transformation matrix for the translation and scaling and rotation of the body parts.
    those transformations are relative to the world space, and if we want to create

    so we want to scale then translate then rotate
    but so each part has a world position that is different from it's local space, which is a specific pos a scale
    relative to the parent part pos and scale ??

    so what happens is that if we translate torso, we should translate all the attached parts as well, with
    their center of rotation shifted by that amount as well.


    /!\ If we modify the size of a body part, the bodyparts depending on it must adapt
    so maybe we could have a system that works with
*/
