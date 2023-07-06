#ifndef _POSE_SAMPLE_H_
#define _POSE_SAMPLE_H_

#include "../../animation/pose.h"
#include "../application.h"

class PoseSample : public Application
{

protected:
    Pose m_resetPose;

public:
    PoseSample(const char *_name, const char *_description)
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

#endif //_POSE_SAMPLE_H_