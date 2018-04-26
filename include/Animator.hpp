#pragma once

#include <iostream>
#include <string>
#include <vector>
// #include <> // time

#include "Exception.hpp"
#include "Skeleton.hpp"
#include "Matrix.hpp"
#include "Bone.hpp"

typedef struct  sBoneTransform {
    std::string boneId;
    vec3        translation;
    vec3        rotation;
    vec3        scale;
}               tBoneTransform;

typedef std::vector<std::vector<tBoneTransform>*>  tAnimationFrames;

class Animator {

public:
    Animator( Skeleton* skeleton, tAnimationFrames* animation, size_t frameDuration = 50 );
    Animator( const Animator& rhs );
    Animator& operator=( const Animator& rhs );
    ~Animator( void );

    void        update( void );

    Skeleton*                       getSkeleton( void ) const { return (skeleton); };
    size_t                          getFrameDuration( void ) const { return (frameDuration); };
    // const std::list<const tFrame>&  getFrames( void ) const { return (frames); };

private:
    Skeleton*               skeleton;
    tAnimationFrames*       frames;
    size_t                  frameDuration; // the duration of a frame in milliseconds
    // or float framesPerSecond instead

};

// vec< vec< obj >* >*
// pointer to vec | of pointers to vec | of obj
