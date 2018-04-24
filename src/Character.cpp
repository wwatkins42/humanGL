#include "Character.hpp"

Character::Character( void ) {
    /* not really modular, sorry */
    BodyPart* head = new BodyPart(
        (std::forward_list<BodyPart *>){{}},
        "head",
        vec3({0, 2, 0}),
        vec3({1, 1, 1})
    );
    BodyPart* lowerLegLeft = new BodyPart(
        (std::forward_list<BodyPart *>){{}},
        "lowerLegLeft",
        vec3({0, -1.5, 0}),
        vec3({0.75, 1.5, 0.75})
    );
    BodyPart* lowerLegRight = new BodyPart(
        (std::forward_list<BodyPart *>){{}},
        "lowerLegRight",
        vec3({0, -1.5, 0}),
        vec3({0.75, 1.5, 0.75})
    );
    BodyPart* upperLegLeft = new BodyPart(
        (std::forward_list<BodyPart *>){ lowerLegLeft },
        "upperLegLeft",
        vec3({-0.625, -2.25, 0}),
        vec3({0.75, 1.5, 0.75})
    );
    BodyPart* upperLegRight = new BodyPart(
        (std::forward_list<BodyPart *>){ lowerLegRight },
        "upperLegRight",
        vec3({0.625, -2.25, 0}),
        vec3({0.75, 1.5, 0.75})
    );
    BodyPart* lowerArmLeft = new BodyPart(
        (std::forward_list<BodyPart *>){{}},
        "lowerArmLeft",
        vec3({0, -1.5, 0}),
        vec3({0.6, 1.5, 0.6})
    );
    BodyPart* lowerArmRight = new BodyPart(
        (std::forward_list<BodyPart *>){{}},
        "lowerArmRight",
        vec3({0, -1.5, 0}),
        vec3({0.6, 1.5, 0.6})
    );
    BodyPart* upperArmLeft = new BodyPart(
        (std::forward_list<BodyPart *>){ lowerArmLeft },
        "upperArmLeft",
        vec3({-1.3, 0.75, 0}),
        vec3({0.6, 1.5, 0.6})
    );
    BodyPart* upperArmRight = new BodyPart(
        (std::forward_list<BodyPart *>){ lowerArmRight },
        "upperArmRight",
        vec3({1.3, 0.75, 0}),
        vec3({0.6, 1.5, 0.6})
    );
    BodyPart* torso = new BodyPart(
        (std::forward_list<BodyPart *>){{ upperArmLeft, head, upperArmRight, upperLegLeft, upperLegRight }},
        // (std::forward_list<BodyPart *>){{ head, upperArmLeft, upperArmRight, upperLegLeft, upperLegRight }},
        "torso",
        vec3({0, 0, 0}),
        vec3({2, 3, 0.9})
    );
    this->parentPart = torso;
    // this->parentPart->resize(vec3({1, 2, 1.1})); // TODO: we'll change that later

}

Character::Character( const Character& rhs ) {
    *this = rhs;
}

Character& Character::operator=( const Character& rhs ) {
    (void)rhs;
    return (*this);
}

Character::~Character( void ) {
    delete this->parentPart;
}

void    Character::update( void ) {
    mat4    identity;
    identity = identity.identity();
    this->parentPart->update(identity);

    this->parentPart->getModel()->setPos(vec3({(float)std::sin(glfwGetTime()*2)*3, 0, 0}));
    this->parentPart->getChildren().front()->getModel()->setRotation(vec3({(float)std::cos(glfwGetTime())*3, 0, 0}));
    this->parentPart->getModel()->setRotation(vec3({0, (float)std::cos(glfwGetTime())*3, 0}));
}

void    Character::render( Shader* shader ) {
    this->parentPart->render(shader);
}
