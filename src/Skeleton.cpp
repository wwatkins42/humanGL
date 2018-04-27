#include "Skeleton.hpp"

Skeleton::Skeleton( std::unordered_map<std::string, Bone*> bones, const std::string& parentBoneId ) : bones(bones), parentBoneId(parentBoneId) {
    this->parentBone = this->bones[this->parentBoneId];
    this->parentBone->getModel()->setPosition(vec3({0, 2, -2}));
    this->parentBone->getModel()->setOrientation(vec3({0, -(float)(M_PI/6.0f), 0}));
    // this->parentBone->getModel()->setOrientation(vec3({0, -(float)(M_PI/2.0f), 0}));
}

Skeleton::Skeleton( const Skeleton& rhs ) {
    *this = rhs;
}

Skeleton& Skeleton::operator=( const Skeleton& rhs ) {
    (void)rhs;
    return (*this);
}

Skeleton::~Skeleton( void ) {
    delete this->parentBone;
}

void    Skeleton::update( void ) {
    this->parentBone->update(mtls::mat4identity);
    this->bones["torso"]->getModel()->setOrientation(vec3({0, (float)glfwGetTime()*0.5f, 0}));
    // this->bones["upperArmLeft"]->getModel()->setRotation(vec3({0, 0, (float)std::cos(glfwGetTime()*4)*0.99f+0.99f}));
    // this->bones["torso"]->getModel()->setRotation(vec3({0, -0.5f+(float)std::cos(glfwGetTime()*2)*0.55f, 0}));
    /* dynamic rescale, I'll do that with switch during animation */
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

void    Skeleton::render( Shader* shader ) {
    this->parentBone->render(shader);
}

Bone*   Skeleton::operator[]( const std::string& id ) {
    if (this->bones.find(id) == this->bones.end())
        throw Exception::SkeletonMapAccessError(id);
    return (this->bones[id]);
}
