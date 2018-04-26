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
    size_t frame = 0; // TMP
    // mat4    transform;

    for (std::vector<tBoneTransform>::iterator it = (*this->frames)[frame]->begin(); it != (*this->frames)[frame]->end(); it++) {
        const std::string boneId = it->boneId;
        std::cout << boneId << std::endl;
        // (*this->skeleton)[boneId]->getModel()->setTranslation(it->translation);
        // (*this->skeleton)[boneId]->getModel()->setRotation(it->rotation);
        this->skeleton->update();
    }

    // for (size_t bone = 0; bone < this->frames[frame]->size(); ++bone) {
        // const std::string boneId = (*this->frames[frame])[bone]->boneId;
        // this->skeleton[boneId]->getModel()->setTranslation((*this->frames[frame])[bone]->translation);
        // this->skeleton[boneId]->getModel()->setRotation((*this->frames[frame])[bone]->rotation);
    // }
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
