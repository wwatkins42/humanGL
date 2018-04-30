#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <chrono>

#include "Exception.hpp"
#include "Skeleton.hpp"
#include "Matrix.hpp"
#include "Bone.hpp"

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
    vec3        scale;
}               tBoneTransform;

typedef std::vector<std::vector<tBoneTransform>*> tAnimationFrames;
typedef std::chrono::duration<double,std::milli> tMilliseconds;

typedef struct  sAnimation {
    tAnimationFrames*   frames;
    size_t              cycleDuration;
}               tAnimation;

class Animator {

public:
    Animator( Skeleton* skeleton, std::vector<tAnimation> animations );
    ~Animator( void );

    void                    selectAnim( size_t id );
    void                    update( void );
    size_t                  getNextFrame( void );
    float                   getFrameInterpolation( eFrameInterpolation interpolation = none );
    tMilliseconds           getElapsedMilliseconds( void );

    Skeleton*               getSkeleton( void ) const { return (skeleton); };
    size_t                  getCAnim( void ) const { return (cAnim); };
    size_t                  getCFrame( void ) const { return (cFrame); };
    size_t                  getCFrameDuration( void ) const { return (cFrameDuration); };

private:
    Skeleton*                               skeleton;
    std::vector<tAnimation>                 animations;
    size_t                                  cAnim;
    size_t                                  cFrame;
    size_t                                  cFrameDuration;
    std::chrono::steady_clock::time_point   pTimepoint;

};
