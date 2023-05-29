#include "animation_clip.h"
#include "interpolation.h"
#include <cmath>

AnimationClip ::AnimationClip()
{
    m_clipName = "No Name";
    m_isLooping = false;
    m_duration = 0.0f;
    m_startTime = 0.0f;
    m_endTime = 0.0f;
}

AnimationClip::~AnimationClip()
{
}

size_t AnimationClip::size() const
{
    return m_animated_joints.size();
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

AnimatedJoint &AnimationClip ::operator[](size_t _jointID)
{
    for (size_t i = 0; i < m_animated_joints.size(); i++)
    {
        if (m_animated_joints[i].m_jointID == _jointID)
        {
            return m_animated_joints[i];
        }
    }

    AnimatedJoint newAnimatedJoint;
    m_animated_joints.push_back(newAnimatedJoint);

    return m_animated_joints[m_animated_joints.size() - 1];
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
    m_startTime = MAXFLOAT;
    m_endTime = 0;

    for (size_t i = 0; i < m_animated_joints.size(); i++)
    {
        float trackEndTime = m_animated_joints[i].m_positionTrack.getEndTime();
        float trackStartTime = m_animated_joints[i].m_positionTrack.getStartTime();
        m_startTime = m_startTime < trackStartTime ? m_startTime : trackStartTime;
        m_endTime = m_endTime > trackEndTime ? m_endTime : trackEndTime;
    }

    m_startTime = MAXFLOAT == m_startTime ? 0 : m_startTime;
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

void AnimationClip ::sampleAniamtion(float _inTime, Pose &_outPose) const
{
    // float inTime = clampTime(_inTime);

    // for (size_t i = 0; i < m_animated_joints.size(); i++)
    // {
    //     // Do binary search for keyframe
    //     int high = (int)m_animated_joints[i].m_keyframes.size();
    //     int low = -1;
    //     while (high - low > 1)
    //     {
    //         int probe = (high + low) / 2;
    //         if (m_animated_joints[i].m_keyframes[probe].m_time < inTime)
    //         {
    //             low = probe;
    //         }
    //         else
    //         {
    //             high = probe;
    //         }
    //     }

    //     if (low == -1)
    //     {
    //         // Before first key, return first key
    //         _outPose.setLocalTransfrom(m_animated_joints[i].m_index,
    //                                    m_animated_joints[i].m_keyframes.front().m_joint);
    //     }
    //     else if (high == (int)m_animated_joints[i].m_keyframes.size())
    //     {
    //         // Beyond last key, return last key
    //         _outPose.setLocalTransfrom(m_animated_joints[i].m_index,
    //                                    m_animated_joints[i].m_keyframes.back().m_joint);
    //     }
    //     else
    //     {
    //         // Interpolate
    //         const Keyframe frame1 = m_animated_joints[i].m_keyframes[low];
    //         const Keyframe frame2 = m_animated_joints[i].m_keyframes[low + 1];

    //         float t = (inTime - frame1.m_time) / (frame2.m_time - frame1.m_time);

    //         Transform curTrans;
    //         curTrans.position = lerp(frame1.m_joint.position, frame2.m_joint.position, t);
    //         curTrans.scale = lerp(frame1.m_joint.scale, frame2.m_joint.scale, t);

    //         // rotation must be normaized
    //         curTrans.rotation = slerp(frame1.m_joint.rotation, frame2.m_joint.rotation, t);
    //     }
    // }
}