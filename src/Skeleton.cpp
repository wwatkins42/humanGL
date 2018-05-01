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
