#include "Character.hpp"

Character::Character( void ) {
    /* not really modular, sorry */
    BodyPart    *head = new BodyPart((std::forward_list<BodyPart *>){{}}, "head");
    BodyPart    *lowerLegLeft = new BodyPart((std::forward_list<BodyPart *>){{}}, "lowerLegLeft");
    BodyPart    *lowerLegRight = new BodyPart((std::forward_list<BodyPart *>){{}}, "lowerLegRight");
    BodyPart    *lowerArmLeft = new BodyPart((std::forward_list<BodyPart *>){{}}, "lowerArmLeft");
    BodyPart    *lowerArmRight = new BodyPart((std::forward_list<BodyPart *>){{}}, "lowerArmRight");
    BodyPart    *upperArmLeft = new BodyPart((std::forward_list<BodyPart *>){ lowerArmLeft }, "upperArmLeft");
    BodyPart    *upperArmRight = new BodyPart((std::forward_list<BodyPart *>){ lowerArmRight }, "upperArmRight");
    BodyPart    *upperLegLeft = new BodyPart((std::forward_list<BodyPart *>){ lowerLegLeft }, "upperLegLeft");
    BodyPart    *upperLegRight = new BodyPart((std::forward_list<BodyPart *>){ lowerLegRight }, "upperLegRight");
    BodyPart    *torso = new BodyPart((std::forward_list<BodyPart *>){{ head, upperArmLeft, upperArmRight, upperLegLeft, upperLegRight }}, "torso");
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

void    Character::render( void ) {
    this->parentPart->render();
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

*/
