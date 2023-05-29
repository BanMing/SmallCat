#ifndef _ANIMATION_CLIP_H_
#define _ANIMATION_CLIP_H_

#include "animated_joint.h"
#include "track.h"
#include "pose.h"
#include <vector>
#include <string>

class AnimationClip
{
protected:
    std::vector<AnimatedJoint> m_animated_joints;
    std::string m_clipName;
    bool m_isLooping;
    float m_duration;
    float m_startTime;
    float m_endTime;

protected:
    float clampTime(float _inTime) const;

public:
    AnimationClip();
    ~AnimationClip();
    size_t size() const;
    AnimatedJoint &operator[](size_t _jointID);

    void setName(const std::string &_newName);
    std::string &getName() ;

    void updateDuration();
    float getDuration() const;

    bool isLooping() const;
    void setLooping(bool _isLooping);

    void sampleAniamtion(float _inTime, Pose &_outPose) const;
};

#endif //_ANIMATION_CLIP_H_