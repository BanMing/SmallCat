#ifndef _ANIMATION_CLIP_H_
#define _ANIMATION_CLIP_H_

#include "joint_animation_track.h"
#include "pose.h"
#include <vector>
#include <string>

class AnimationClip
{
protected:
    std::vector<JointAnimationTrack> m_jointTracks;
    std::string m_clipName;
    bool m_isLooping;
    float m_duration;
    float m_startTime;
    float m_endTime;

protected:
    float AdjustTimeToFitRange(float _inTime);

public:
    void Sample(float _inTime) const;

    JointAnimationTrack &operator[](size_t index) const;
    
    void SetName(const std::string &_newName);
    std::string &GetName() const;

    void UpdateDuration();
    float GetDuration() const;

    bool IsLooping() const;
    void SetLooping(bool _isLooping);
};

#endif //_ANIMATION_CLIP_H_