#ifndef _JOINT_ANIMATION_TRACK_H_
#define _JOINT_ANIMATION_TRACK_H_

#include "keyframe.h"
#include "track.h"
#include <vector>
#include <string>

struct AnimatedJoint
{
    std::string m_jointName;
    size_t m_jointID;

    VectorTrack m_positionTrack;
    QuaternionTrack m_rotationTrack;
    VectorTrack m_scaleTrack;
};

#endif //_JOINT_ANIMATION_TRACK_H_
