#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <chrono>

#include "Exception.hpp"
#include "Skeleton.hpp"
#include "Matrix.hpp"
#include "Bone.hpp"

// enum eAnimationPlay {
//     once,
//     loop,
//     reverse
// };

enum eFrameInterpolation {
    none,
    sinerp,
    coserp,
    smoothstep,
    smootherstep
};

typedef struct  sBoneTransform {
    std::string boneId;
    vec3        translation;
    vec3        rotation;
}               tBoneTransform;

typedef std::vector<std::vector<tBoneTransform>*> tAnimationFrames;
typedef std::chrono::duration<double,std::milli> tMilliseconds;

class Animator {

public:
    Animator( Skeleton* skeleton, tAnimationFrames* animation, size_t cycleDuration = 500 );
    Animator( const Animator& rhs );
    Animator& operator=( const Animator& rhs );
    ~Animator( void );

    void                    update( void );
    size_t                  getNextFrame( void );
    float                   getFrameInterpolation( eFrameInterpolation interpolation = none );
    tMilliseconds           getElapsedMilliseconds( void );
    
    Skeleton*               getSkeleton( void ) const { return (skeleton); };
    size_t                  getFrameDuration( void ) const { return (frameDuration); };
    const tAnimationFrames* getFrames( void ) const { return (frames); };

private:
    Skeleton*                               skeleton;
    tAnimationFrames*                       frames;
    size_t                                  frameDuration; // the duration of a frame in milliseconds
    size_t                                  cFrame;
    std::chrono::steady_clock::time_point   pTimepoint;

};
