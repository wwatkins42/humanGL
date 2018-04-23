#include "Character.hpp"

Character::Character( void ) {
    /* not really modular, sorry */
    BodyPart* head = new BodyPart(
        (std::forward_list<BodyPart *>){{}},
        "head",
        vec3({0, 1, 0}),
        vec3({1, 1, 1})
    );
    BodyPart* lowerLegLeft = new BodyPart(
        (std::forward_list<BodyPart *>){{}},
        "lowerLegLeft",
        vec3({0, 0, 0}),
        vec3({1, 1, 1})
    );
    BodyPart* lowerLegRight = new BodyPart(
        (std::forward_list<BodyPart *>){{}},
        "lowerLegRight",
        vec3({0, 0, 0}),
        vec3({1, 1, 1})
    );
    BodyPart* lowerArmLeft = new BodyPart(
        (std::forward_list<BodyPart *>){{}},
        "lowerArmLeft",
        vec3({-1, -1, 0}),
        vec3({1, 1, 1})
    );
    BodyPart* lowerArmRight = new BodyPart(
        (std::forward_list<BodyPart *>){{}},
        "lowerArmRight",
        vec3({1, -1, 0}),
        vec3({1, 1, 1})
    );
    BodyPart* upperArmLeft = new BodyPart(
        (std::forward_list<BodyPart *>){ lowerArmLeft },
        "upperArmLeft",
        vec3({-1, 0, 0}),
        vec3({1, 1, 1})
    );
    BodyPart* upperArmRight = new BodyPart(
        (std::forward_list<BodyPart *>){ lowerArmRight },
        "upperArmRight",
        vec3({1, 0, 0}),
        vec3({1, 1, 1})
    );
    BodyPart* upperLegLeft = new BodyPart(
        (std::forward_list<BodyPart *>){ lowerLegLeft },
        "upperLegLeft",
        vec3({0, 0, 0}),
        vec3({1, 1, 1})
    );
    BodyPart* upperLegRight = new BodyPart(
        (std::forward_list<BodyPart *>){ lowerLegRight },
        "upperLegRight",
        vec3({0, 0, 0}),
        vec3({1, 1, 1})
    );
    BodyPart* torso = new BodyPart(
        (std::forward_list<BodyPart *>){{ head, upperArmLeft, upperArmRight, upperLegLeft, upperLegRight }},
        "torso",
        vec3({0, 0, 0}),
        vec3({1, 1, 1})
    );
    this->parentPart = torso;
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
    this->parentPart->update();
}

void    Character::render( Shader* shader ) {
    this->parentPart->render(shader);
}

/*

idx, children, pos, dims, color, str

dims are defined as : width, height, depth (from a front view)

0: [1, 2, 3, 4, 5], { 0, 0, 0}, { 2 ,   2 , 1.25}, 0x0, "torso"
1: [],              { 0, 1, 0}, { 1 ,   1 ,   1 }, 0x0, "head"
2: [6],             {-1, 1, 0}, {0.5, 1.25,   1 }, 0x0, "upper_arm_left"
3: [7],             { 1, 1, 0}, {0.5, 1.25,   1 }, 0x0, "upper_arm_right"
6: [],              {-1, 0, 0}, {0.5, 1.25,   1 }, 0x0, "lower_arm_left"
7: [],              { 1, 0, 0}, {0.5, 1.25,   1 }, 0x0, "lower_arm_right"
4: [8],             {-1,-1, 0}, {0.5,   1 ,   1 }, 0x0, "upper_leg_left"
5: [9],             { 1,-1, 0}, {0.5,   1 ,   1 }, 0x0, "upper_leg_right"
8: [],              {-1,-2, 0}, {0.5,   1 ,   1 }, 0x0, "lower_leg_left"
9: [],              { 1,-2, 0}, {0.5,   1 ,   1 }, 0x0, "lower_leg_right"

pos is not correct, it's something like that

    y
    |__ x
   /
  z

        (0, 1, 0)           head
(-1, 0, 0)     (1, 0, 0)    upper arms
        (0, 0, 0)           torso
(-1,-1, 0)     (1,-1, 0)    lower arms
  (0, 0, 0)   (0, 0, 0)     upper legs
  (0, 0, 0)   (0, 0, 0)     lower legs


*/
