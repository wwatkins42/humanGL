#include "Animator.hpp"

Animator::Animator( Skeleton* skeleton, tAnimationFrames* animation, size_t frameDuration ) : skeleton(skeleton), frames(animation), frameDuration(frameDuration) {
    this->pTimepoint = std::chrono::steady_clock::now();
    this->cFrame = 0;
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
    mat4    transform;

    if (this->getElapsedMilliseconds() > this->frameDuration) {
        this->pTimepoint = std::chrono::steady_clock::now();
        this->cFrame = (this->cFrame + 1 >= this->frames->size() ? 0 : this->cFrame + 1);
    }

    // float t = 1.0f - ((this->frameDuration - this->getElapsedMilliseconds()) / (float)this->frameDuration);
    float t = 1.0f - ((this->frameDuration - static_cast<double>(this->getElapsed().count())) / (float)this->frameDuration);
    std::cout << t << std::endl;

    // for (std::vector<tBoneTransform>::iterator it = (*this->frames)[this->cFrame]->begin(); it != (*this->frames)[this->cFrame]->end(); it++) {
    for (size_t i = 0; i < (*this->frames)[this->cFrame]->size(); ++i) {

        tBoneTransform curr = (*(*this->frames)[this->cFrame])[i];
        tBoneTransform next = (*(*this->frames)[this->cFrame+1])[i];
        const std::string boneId = curr.boneId;

        vec3    translation = mtls::lerp(curr.translation, next.translation, t);
        vec3    rotation = mtls::lerp(curr.rotation, next.rotation, t);

        transform.identity();
        transform = mtls::translate(transform, translation);
        transform = mtls::rotate(transform, rotation, (*this->skeleton)[boneId]->getModel()->getJoint());
        (*this->skeleton)[boneId]->getModel()->setExternalTransform(transform);
        this->skeleton->update();
    }
}

size_t  Animator::getElapsedMilliseconds( void ) {
    std::chrono::duration<double, std::milli> elapsed = (std::chrono::steady_clock::now() - this->pTimepoint);
    return (std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count());
}

std::chrono::duration<double, std::milli>  Animator::getElapsed( void ) {
    return (std::chrono::steady_clock::now() - this->pTimepoint);
}

// void    Animator::update( void ) {
//     mat4    transform;
//     if (this->getElapsedMilliseconds() > this->frameDuration) {
//         this->pTimepoint = std::chrono::steady_clock::now();
//         this->cFrame = (this->cFrame + 1 >= this->frames->size() ? 0 : this->cFrame + 1);
//         for (std::vector<tBoneTransform>::iterator it = (*this->frames)[this->cFrame]->begin(); it != (*this->frames)[this->cFrame]->end(); it++) {
//             const std::string boneId = it->boneId;
//             vec3    t = (*this->skeleton)[boneId]->getModel()->getTranslation();
//             vec3    r = (*this->skeleton)[boneId]->getModel()->getRotation();
//             (*this->skeleton)[boneId]->getModel()->setTranslation(t + it->translation);
//             (*this->skeleton)[boneId]->getModel()->setRotation(r + it->rotation);
//             this->skeleton->update();
//         }
//     }
// }

/*
    - the skeleton contains bones.
    - bones contain informations about their translation/rotation/scale,
      * the translation is position in its parent local-space
      * the rotation is in the bone local-space, and relative to the joint (which is in the bone local-space)
      * the scale is an absolute value
      their initial value is the state of the skeleton at rest.
    - animations sit on op of that by showing the transformation for each frame with
      translation/rotation

    the bones have a default transform for the base character.
    and another transform for animation translation/rotation
*/
