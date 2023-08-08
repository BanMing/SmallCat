#include "joint_track.h"
#include "../math/constants.h"
JointTrack::JointTrack() {}

std::string &JointTrack::getJointName()
{
    return m_jointName;
}

void JointTrack::setJointName(const std::string &_jointName)
{
    m_jointName = _jointName;
}

size_t JointTrack::getJointID() const
{
    return m_jointID;
}

void JointTrack::setJointID(size_t _ID)
{
    m_jointID = _ID;
}

VectorTrack &JointTrack::getPositionTrack()
{
    return m_positionTrack;
}

VectorTrack &JointTrack::getScaleTrack()
{
    return m_scaleTrack;
}

QuaternionTrack &JointTrack::getRotationTrack()
{
    return m_rotationTrack;
}

float JointTrack::getStartTime()
{
    float startTime = kMaxFloat;

    if (m_positionTrack.getFrameSize() > 0 && startTime > m_positionTrack.getStartTime())
    {
        startTime = m_positionTrack.getStartTime();
    }

    if (m_rotationTrack.getFrameSize() > 0 && startTime > m_rotationTrack.getStartTime())
    {
        startTime = m_rotationTrack.getStartTime();
    }

    if (m_scaleTrack.getFrameSize() > 0 && startTime > m_scaleTrack.getStartTime())
    {
        startTime = m_scaleTrack.getStartTime();
    }

    return startTime == kMaxFloat ? 0.0f : startTime;
}

float JointTrack::getEndTime()
{
    float endTime = 0.0f;
    if (m_positionTrack.getFrameSize() > 0 && endTime < m_positionTrack.getEndTime())
    {
        endTime = m_positionTrack.getEndTime();
    }

    if (m_rotationTrack.getFrameSize() > 0 && endTime < m_rotationTrack.getEndTime())
    {
        endTime = m_rotationTrack.getEndTime();
    }

    if (m_scaleTrack.getFrameSize() > 0 && endTime < m_scaleTrack.getEndTime())
    {
        endTime = m_scaleTrack.getEndTime();
    }
    return endTime;
}

bool JointTrack::isValid() const
{
    return m_positionTrack.getFrameSize() > 1 || m_scaleTrack.getFrameSize() > 1 || m_rotationTrack.getFrameSize() > 1;
}

Transform JointTrack::sample(float _time, bool _isLooping, const Transform &_defalutTrans) const
{
    Transform result = _defalutTrans;

    if (m_positionTrack.getFrameSize() > 1)
    {
        result.position = m_positionTrack.sample(_time, _isLooping);
    }
    if (m_scaleTrack.getFrameSize() > 1)
    {
        result.scale = m_scaleTrack.sample(_time, _isLooping);
    }
    if (m_rotationTrack.getFrameSize() > 1)
    {
        result.rotation = m_rotationTrack.sample(_time, _isLooping);
    }

    return result;
}
