#include "BodyPart.hpp"

BodyPart::BodyPart( std::forward_list<BodyPart*> children, const std::string type ) : type(type), children(children) {
    this->model = new Model();
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

void    BodyPart::update( void ) {
    for (std::forward_list<BodyPart*>::iterator it = this->children.begin(); it != this->children.end(); ++it)
        if (*it) (*it)->update();
    // code for update of this instance
}

void    BodyPart::render( void ) {
    // render all the children parts
    for (std::forward_list<BodyPart*>::iterator it = this->children.begin(); it != this->children.end(); ++it)
        if (*it) (*it)->render();
    // then render this part
    this->model->render();
}
