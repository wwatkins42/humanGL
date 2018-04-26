#include "Animator.hpp"

Animator::Animator( Skeleton* skeleton, tAnimationFrames* animation, size_t frameDuration ) : skeleton(skeleton), frames(animation), frameDuration(frameDuration) {
}

Animator::Animator( const Animator& rhs ) {
    *this = rhs;
}

Animator& Animator::operator=( const Animator& rhs ) {
    (void)rhs;
    return (*this);
}

Animator::~Animator( void ) {
    for (size_t i = 0; i < this->frames->size(); ++i)
        delete (*this->frames)[i];
    delete this->frames;
}

void    Animator::update( void ) {
    // this->parentBone->update(mtls::mat4identity);
    //
    // this->bones["upperArmLeft"]->getModel()->setRotation(vec3({0, 0, (float)std::cos(glfwGetTime()*4)*0.99f+0.99f}));
    // this->bones["torso"]->getModel()->setRotation(vec3({0, (float)std::cos(glfwGetTime()*5)*0.25f, 0}));
    // /* dynamic rescale, I'll do that with switch during animation */
    // this->bones["torso"]->rescale(vec3({
    //     2.0f+(float)std::cos(glfwGetTime()*0.5)*1.5f+1.5f,
    //     3.0f+(float)std::cos(glfwGetTime()*5)*1.5f+1.5f,
    //     0.9f+(float)std::cos(glfwGetTime()*3)*0.5f+0.5f
    // }));
    // this->bones["upperArmLeft"]->rescale(vec3({
    //     0.6f+(float)std::cos(glfwGetTime())*0.5f+0.5f,
    //     1.5f+(float)std::cos(glfwGetTime()*5.0f)*1.5f+1.5f,
    //     0.6f+(float)std::cos(glfwGetTime())*0.5f+0.5f
    // }));


}

/*  Animator could also be above Animation which are object in themselves

    an animation is a list of list of frames containing member to transform:
    [
    [["upperArmLeft", vec3(), vec3()], ["upperArmRight", vec3(), vec3()]],
    [["upperArmLeft", vec3(), vec3()], ["upperArmRight", vec3(), vec3()]],
    [["upperArmLeft", vec3(), vec3()], ["upperArmRight", vec3(), vec3()]],
    ]

    std::string boneId = this->animation[frame].boneId;

    this->skeleton[boneId]->update(this->frames[frame].pos);
    this->skeleton[boneId]->update(this->frames[frame].rotation);

    this->animator->playback()

    walkingAnimation = new std::array<std::array<tBoneTransform>* >({{
        // 1st frame
        new std::array<tBoneTransform>({{
            static_cast<tBoneTransform>({"upperArmRight", vec3({0, 0, 0}), vec3({0, 0, 0})})),
            static_cast<tBoneTransform>({"upperArmLeft",  vec3({0, 0, 0}), vec3({0, 0, 0})})),
            static_cast<tBoneTransform>({"lowerArmRight", vec3({0, 0, 0}), vec3({0, 0, 0})})),
            static_cast<tBoneTransform>({"lowerArmLeft",  vec3({0, 0, 0}), vec3({0, 0, 0})})),
        }},
        // 2nd frame
        new std::array<tBoneTransform>({{
            static_cast<tBoneTransform>({"upperArmRight", vec3({0, 0, 0}), vec3({0, 0, 0})})),
            static_cast<tBoneTransform>({"upperArmLeft",  vec3({0, 0, 0}), vec3({0, 0, 0})})),
            static_cast<tBoneTransform>({"lowerArmRight", vec3({0, 0, 0}), vec3({0, 0, 0})})),
            static_cast<tBoneTransform>({"lowerArmLeft",  vec3({0, 0, 0}), vec3({0, 0, 0})})),
        }},
        // ...
    }});
    this->animator = new Animator(character, walkingAnimation, 50);

    list of frames -> list of tBoneTransform
*/
