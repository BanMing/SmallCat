#include "animation_clip.h"
#include "../math/constants.h"
#include "interpolation.h"
#include <cmath>

AnimationClip ::AnimationClip()
{
    m_clipName = "No Name";
    m_isLooping = true;
    m_duration = 0.0f;
    m_startTime = 0.0f;
    m_endTime = 0.0f;
}

AnimationClip::~AnimationClip()
{
}

size_t AnimationClip::size() const
{
    return m_jointsTrack.size();
}

float AnimationClip::clampTime(float _inTime) const
{
    if (m_isLooping && _inTime > m_duration)
    {
        return fmod(_inTime, m_duration);
    }
    else
    {
        if (_inTime < m_startTime)
        {
            return m_startTime;
        }
        else if (_inTime > m_endTime)
        {
            return m_endTime;
        }
    }

    return _inTime;
}

JointTrack &AnimationClip ::operator[](size_t _jointID)
{
    for (size_t i = 0; i < m_jointsTrack.size(); i++)
    {
        if (m_jointsTrack[i].getJointID() == _jointID)
        {
            return m_jointsTrack[i];
        }
    }

    JointTrack newAnimatedJoint;
    m_jointsTrack.push_back(newAnimatedJoint);

    return m_jointsTrack[m_jointsTrack.size() - 1];
}

void AnimationClip ::setName(const std::string &_newName)
{
    m_clipName = _newName;
}

std::string &AnimationClip ::getName()
{
    return m_clipName;
}

void AnimationClip ::updateDuration()
{
    m_startTime = kMaxFloat;
    m_endTime = 0;

    for (size_t i = 0; i < m_jointsTrack.size(); i++)
    {
        float trackEndTime = m_jointsTrack[i].getEndTime();
        float trackStartTime = m_jointsTrack[i].getStartTime();
        m_startTime = m_startTime < trackStartTime ? m_startTime : trackStartTime;
        m_endTime = m_endTime > trackEndTime ? m_endTime : trackEndTime;
    }

    m_startTime = kMaxFloat == m_startTime ? 0 : m_startTime;
    m_duration = m_endTime - m_startTime;
}

float AnimationClip ::getDuration() const
{
    return m_duration;
}

bool AnimationClip ::isLooping() const
{
    return m_isLooping;
}

void AnimationClip ::setLooping(bool _isLooping)
{
    m_isLooping = _isLooping;
}

float AnimationClip ::sample(float _inTime, Pose &_outPose) const
{
    if (getDuration() == 0.0f)
    {
        return 0.0f;
    }

    float inTime = clampTime(_inTime);

    for (size_t i = 0; i < m_jointsTrack.size(); i++)
    {
        size_t jointID = m_jointsTrack[i].getJointID();
        Transform localJointTrans = _outPose.getLocalTransfrom(jointID);
        Transform animatiedTrans = m_jointsTrack[i].sample(inTime, m_isLooping, localJointTrans);
        _outPose.setLocalTransfrom(jointID, animatiedTrans);
    }

    return inTime;
}