#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <forward_list>

#include "Exception.hpp"
#include "BodyPart.hpp"
#include "Shader.hpp"

class Character {

public:
    Character( void );
    Character( const Character& rhs );
    Character& operator=( const Character& rhs );
    ~Character( void );

    void        update( void );
    void        render( Shader* shader );
    BodyPart*   getParentPart( void ) const { return (parentPart); };

private:
    BodyPart*   parentPart;

};

/*
        0000
        0000
                               ______________________ Torso _______________________
    66 111111 88              |              ___________|___________               |
    66 111111 88        UpperArmLeft        |           |           |        UpperArmRight
    66 111111 88              |        UpperLegLeft     |     UpperLegRight        |
       111111                 |             |          Head         |              |
    77        99        LowerArmLeft        |                       |        LowerArmRight
    77 22  33 99                      LowerLegLeft           LowerLegRight
       22  33

       44  55
       44  55

each body part may contain children, which may also contain children and so on

        Torso : [ Head, UpperArmLeft, UpperArmRight, UpperLegLeft, UpperLegRight ]
         Head : [ ]
 UpperArmLeft : [ LowerArmLeft ]
UpperArmRight : [ LowerArmRight ]
 LowerArmLeft : [ ]
LowerArmRight : [ ]
 UpperLegLeft : [ LowerLegLeft ]
UpperLegRight : [ LowerLegRight ]
 LowerLegLeft : [ ]
LowerLegRight : [ ]

BodyPart will contain methods to update the position, render the body part, then call the childrens and do the necessary updates too
so basically, we will call the Torso which is the highest instance, and in turn it will call the childrens one by one

When we populate our characters body parts, we must begin from the extremities and go up
*/
