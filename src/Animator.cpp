#include "Animator.hpp"

Animator::Animator( Skeleton* skeleton, std::vector<tAnimation> animations ) : skeleton(skeleton), animations(animations) {
    if (animations.size() == 0)
        throw Exception::RuntimeError("Animator should contain at least one tAnimation");
    this->pTimepoint = std::chrono::steady_clock::now();
    this->cAnim = 0;
    this->cFrame = 0;
    this->cFrameDuration = this->animations[0].cycleDuration / static_cast<float>(this->animations[0].frames->size());
    this->selectAnim(0);
}

Animator::~Animator( void ) {
    for (size_t a = 0; a < this->animations.size(); ++a) {
        for (size_t i = 0; i < this->animations[a].frames->size(); ++i)
            delete (*this->animations[a].frames)[i];
        delete this->animations[a].frames;
    }
}

void    Animator::selectAnim( size_t id ) {
    if (id >= this->animations.size())
        id = this->animations.size() - 1;
    // Maybe do a smooth transition between animations ??
    if (this->cAnim != id) {
        /* reset all the bones' externalTransform to identity matrix */
        if (this->animations[cAnim].frames->size() > 1) {
            for (size_t i = 0; i < (*this->animations[cAnim].frames)[this->cFrame]->size(); ++i) {
                tBoneTransform curr = (*(*this->animations[cAnim].frames)[this->cFrame])[i];
                (*this->skeleton)[curr.boneId]->rescale(vec3({0, 0, 0}));
                (*this->skeleton)[curr.boneId]->getModel()->setExternalTransform(mtls::mat4identity);
            }
            this->skeleton->update();
        }
        /* update the current variables */
        this->cAnim = id;
        this->cFrame = 0;
        this->cFrameDuration = this->animations[id].cycleDuration / static_cast<float>(this->animations[id].frames->size());
    }
}

void    Animator::update( void ) {
    if (this->getElapsedMilliseconds().count() > this->cFrameDuration) {
        this->pTimepoint = std::chrono::steady_clock::now();
        this->cFrame = this->getNextFrame();
    }
    if (this->animations[cAnim].frames->size() > 1) {
        float   t = this->getFrameInterpolation(eInterpolationType::linear);
        mat4    transform;
        for (size_t i = 0; i < (*this->animations[cAnim].frames)[this->cFrame]->size(); ++i) {
            tBoneTransform curr = (*(*this->animations[cAnim].frames)[this->cFrame])[i];
            tBoneTransform next = (*(*this->animations[cAnim].frames)[this->getNextFrame()])[i];
            if (this->skeleton->getBones().find(curr.boneId) == this->skeleton->getBones().end())
                continue;
            (*this->skeleton)[curr.boneId]->rescale(mtls::lerp(curr.scale, next.scale, t));
            transform.identity();
            mtls::translate(transform, mtls::lerp(curr.translation, next.translation, t));
            mtls::rotate(transform, mtls::lerp(curr.rotation, next.rotation, t), (*this->skeleton)[curr.boneId]->getModel()->getJoint());
            (*this->skeleton)[curr.boneId]->getModel()->setExternalTransform(transform);
        }
    }
    this->skeleton->update();
}

tMilliseconds   Animator::getElapsedMilliseconds( void ) {
    return (std::chrono::steady_clock::now() - this->pTimepoint);
}

size_t  Animator::getNextFrame( void ) {
    return (this->cFrame + 1 >= this->animations[cAnim].frames->size() ? 0 : this->cFrame + 1);
}

float   Animator::getFrameInterpolation( eInterpolationType interpolation ) {
    float t = (1 - (this->cFrameDuration - this->getElapsedMilliseconds().count()) / this->cFrameDuration);
    switch (interpolation) {
        case eInterpolationType::sinerp:       return(std::sin(t * M_PI * 0.5f));
        case eInterpolationType::coserp:       return(std::cos(t * M_PI * 0.5f));
        case eInterpolationType::smoothstep:   return(t * t * (3.0f - 2.0f * t));
        case eInterpolationType::smootherstep: return(t * t * t * (t * (t * 6.0f - 15.0f) + 10.0f));
        default: break;
    };
    return (t);
}
