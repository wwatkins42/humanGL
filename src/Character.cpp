#include "Character.hpp"

Character::Character( void ) {
    /* not really modular, sorry */
    BodyPart* head = new BodyPart(
        (std::forward_list<BodyPart *>){{}},
        "head",
        vec3({0, 2, 0}),
        vec3({1, 1, 1}),
        vec3({0, 0, 0}), vec3({0, -0.5, 0}),
        0xEEAD7E
    );
    BodyPart* lowerLegLeft = new BodyPart(
        (std::forward_list<BodyPart *>){{}},
        "lowerLegLeft",
        vec3({0, -1.5, 0}),
        vec3({0.75, 1.5, 0.75}),
        vec3({0, 0, 0}), vec3({0, 0.75, 0}),
        0x3F5D6A
    );
    BodyPart* lowerLegRight = new BodyPart(
        (std::forward_list<BodyPart *>){{}},
        "lowerLegRight",
        vec3({0, -1.5, 0}),
        vec3({0.75, 1.5, 0.75}),
        vec3({0, 0, 0}), vec3({0, 0.75, 0}),
        0x3F5D6A
    );
    BodyPart* upperLegLeft = new BodyPart(
        (std::forward_list<BodyPart *>){ lowerLegLeft },
        "upperLegLeft",
        vec3({-0.625, -2.25, 0}),
        vec3({0.75, 1.5, 0.75}),
        vec3({0, 0, 0}), vec3({-0.375, 0.75, 0}),
        0x3F5D6A
    );
    BodyPart* upperLegRight = new BodyPart(
        (std::forward_list<BodyPart *>){ lowerLegRight },
        "upperLegRight",
        vec3({0.625, -2.25, 0}),
        vec3({0.75, 1.5, 0.75}),
        vec3({0, 0, 0}), vec3({0.375, 0.75, 0}),
        0x3F5D6A
    );
    BodyPart* lowerArmLeft = new BodyPart(
        (std::forward_list<BodyPart *>){{}},
        "lowerArmLeft",
        vec3({0, -1.5, 0}),
        vec3({0.6, 1.5, 0.6}),
        vec3({1.9, 0, 0}), vec3({0, 0.75, 0}),
        0xEEAD7E
    );
    BodyPart* lowerArmRight = new BodyPart(
        (std::forward_list<BodyPart *>){{}},
        "lowerArmRight",
        vec3({0, -1.5, 0}),
        vec3({0.6, 1.5, 0.6}),
        vec3({1.9, 0, 0}), vec3({0, 0.75, 0}),
        0xEEAD7E
    );
    BodyPart* upperArmLeft = new BodyPart(
        (std::forward_list<BodyPart *>){ lowerArmLeft },
        "upperArmLeft",
        vec3({-1.3, 0.75, 0}),
        vec3({0.6, 1.5, 0.6}),
        vec3({0, 0, 0}), vec3({0.3, 0.75, 0}),
        0x408467
    );
    BodyPart* upperArmRight = new BodyPart(
        (std::forward_list<BodyPart *>){ lowerArmRight },
        "upperArmRight",
        vec3({1.3, 0.75, 0}),
        vec3({0.6, 1.5, 0.6}),
        vec3({0, 0, 0}), vec3({-0.3, 0.75, 0}),
        0x408467
    );
    BodyPart* torso = new BodyPart(
        // (std::forward_list<BodyPart *>){{ head, upperArmLeft, upperArmRight, upperLegLeft, upperLegRight }},
        (std::forward_list<BodyPart *>){{ upperArmLeft, head, upperArmRight, upperLegLeft, upperLegRight }},
        // (std::forward_list<BodyPart *>){{ upperLegLeft, upperArmLeft, head, upperArmRight, upperLegRight }},
        // (std::forward_list<BodyPart *>){{ upperLegRight, upperArmLeft, head, upperArmRight, upperLegLeft }},
        "torso",
        vec3({0, 0, 0}),
        vec3({2, 3, 0.9}),
        vec3({0, 0, 0}), vec3({0, 0, 0}),
        0x43876A
    );
    this->parentPart = torso;
    // this->parentPart->getChildren().front()->rescale(vec3({1, 1.5, 0.95}));
    // this->parentPart->getChildren().front()->getChildren().front()->resize(vec3({1.2, 1.5, 0.6}));

    this->parentPart->getModel()->setRotation(vec3({0, -0.8f, 0}));
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

    // this->parentPart->getModel()->setPos(vec3({(float)std::sin(glfwGetTime()*2)*3, 0, 0}));
    // this->parentPart->getChildren().front()->getModel()->setRotation(vec3({(float)std::cos(glfwGetTime())*3, 0, 0}));
    this->parentPart->getChildren().front()->getModel()->setRotation(vec3({0, 0, (float)std::cos(glfwGetTime()*4)*0.99f+0.99f}));
    this->parentPart->getModel()->setRotation(vec3({0, (float)std::cos(glfwGetTime())*0.25f, 0}));
    this->parentPart->getChildren().front()->rescale(vec3({
        0.6f+(float)std::cos(glfwGetTime())*0.5f+0.5f,
        1.5f+(float)std::cos(glfwGetTime())*1.5f+1.5f,
        0.6f+(float)std::cos(glfwGetTime())*0.5f+0.5f
    }));
}

void    Character::render( Shader* shader ) {
    this->parentPart->render(shader);
}
