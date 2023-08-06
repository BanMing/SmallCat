#ifndef _ANIMATION_CLIP_SAMPLE_H_
#define _ANIMATION_CLIP_SAMPLE_H_
#include "../application.h"
#include "../../animation/animation_clip.h"
#include "../../animation/pose.h"
#include <vector>

class AnimationClipSample : public Application
{

protected:
    Pose m_curPose;
    Pose m_restPose;
    std::vector<AnimationClip> m_clips;

public:
    AnimationClipSample(const char *_name, const char *_description)
        : Application(_name, _description)
    {
    }

protected:
    void onInit() override;
    void onShutdown() override;
    void onUpdate(float _deltaTime) override;
    void onRender(float _inAspectRatio) override;
    void onGui() override;
};
#endif //_ANIMATION_CLIP_SAMPLE_H_