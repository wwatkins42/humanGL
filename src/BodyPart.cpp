#include "BodyPart.hpp"

BodyPart::BodyPart( std::forward_list<BodyPart *> children, std::string const type ) : type(type), children(children) {
    this->model = new Model();
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
