#include "Skeleton.hpp"

Skeleton::Skeleton( std::unordered_map<std::string, Bone*> bones, const std::string& parentBoneId ) : bones(bones), parentBoneId(parentBoneId) {
    this->parentBone = this->bones[this->parentBoneId];
    this->parentBone->getModel()->setOrientation(vec3({0, 5*M_PI/6, 0}));
}

Skeleton::~Skeleton( void ) {
    delete this->parentBone;
}

void    Skeleton::update( void ) {
    this->parentBone->update(mtls::mat4identity, this->shader);
}

Bone*   Skeleton::operator[]( const std::string& id ) {
    if (this->bones.find(id) == this->bones.end())
        throw Exception::SkeletonMapAccessError(id);
    return (this->bones[id]);
}

void    Skeleton::scaleSelection( const std::array<tKey, N_KEY>& keys, const std::string& boneId ) {
    /* scaling all axis */
    float   gScale = (keys[GLFW_KEY_EQUAL].value - keys[GLFW_KEY_MINUS].value) * 0.05f;
    /* individual axis scaling */
    vec3    aScale({
        (keys[GLFW_KEY_L].value - keys[GLFW_KEY_J].value) * 0.05f,
        (keys[GLFW_KEY_Y].value - keys[GLFW_KEY_H].value) * 0.05f,
        (keys[GLFW_KEY_I].value - keys[GLFW_KEY_K].value) * 0.05f,
    });
    this->bones[boneId]->getModel()->scaleExternal = mtls::max(this->bones[boneId]->getModel()->scaleExternal + aScale + gScale, 0);
    this->bones[boneId]->rescale(mtls::max(this->bones[boneId]->getModel()->getScaling() + aScale + gScale, 0));
    this->update();
}

void    Skeleton::switchBonesModel( short key ) {
    for (auto it = this->bones.begin(); it != this->bones.end(); it++)
        it->second->getModel()->switchModel(key);
}
