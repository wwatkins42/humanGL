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
    if (this->getElapsedMilliseconds().count() > this->frameDuration) {
        this->pTimepoint = std::chrono::steady_clock::now();
        this->cFrame = this->getNextFrame();
    }
    float   t = this->getInterpolation();
    mat4    transform;
    for (size_t i = 0; i < (*this->frames)[this->cFrame]->size(); ++i) {
        tBoneTransform curr = (*(*this->frames)[this->cFrame])[i];
        tBoneTransform next = (*(*this->frames)[this->getNextFrame()])[i];
        transform.identity();
        mtls::translate(transform, mtls::lerp(curr.translation, next.translation, t));
        mtls::rotate(transform, mtls::lerp(curr.rotation, next.rotation, t), (*this->skeleton)[curr.boneId]->getModel()->getJoint());
        (*this->skeleton)[curr.boneId]->getModel()->setExternalTransform(transform);
    }
    this->skeleton->update();
}

tMilliseconds   Animator::getElapsedMilliseconds( void ) {
    return (std::chrono::steady_clock::now() - this->pTimepoint);
}

size_t  Animator::getNextFrame( void ) {
    return (this->cFrame + 1 >= this->frames->size() ? 0 : this->cFrame + 1);
}

float   Animator::getInterpolation( void ) {
    return (1 - (this->frameDuration - this->getElapsedMilliseconds().count()) / (float)this->frameDuration);
}
