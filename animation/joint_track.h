
#ifndef _JOINT_TRACK_H_
#define _JOINT_TRACK_H_

#include "keyframe.h"
#include "track.h"
#include <string>
#include <vector>

class JointTrack
{
protected:
    std::string m_jointName;
    size_t m_jointID;

    VectorTrack m_positionTrack;
    QuaternionTrack m_rotationTrack;
    VectorTrack m_scaleTrack;

public:
    JointTrack();
    std::string &getJointName();
    void setJointName(const std::string &_jointName);
    size_t getJointID() const;
    void setJointID(size_t _ID);
    VectorTrack &getPositionTrack();
    VectorTrack &getScaleTrack();
    QuaternionTrack &getRotationTrack();
    float getStartTime();
    float getEndTime();
    bool isValid() const;
    Transform sample(float _time, bool _isLooping, const Transform &_defalutTrans) const;
};

#endif //_JOINT_TRACK_H_