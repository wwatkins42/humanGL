#include "Skeleton.hpp"

Skeleton::Skeleton( std::unordered_map<std::string, Bone*> bones, const std::string& parentBoneId ) : bones(bones), parentBoneId(parentBoneId) {
    this->parentBone = this->bones[this->parentBoneId];
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
    float s = (keys[GLFW_KEY_EQUAL].value - keys[GLFW_KEY_MINUS].value) * 0.05f;
    vec3    scaling = mtls::max(this->bones[boneId]->getModel()->getScaling() + s, 0);
    // this->bones[boneId]->getModel()->setScale(scaling);
    this->bones[boneId]->rescale(scaling);
    this->update();
}
