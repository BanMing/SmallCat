#ifndef _DEBUG_DRAW_SAMPLE_H_
#define _DEBUG_DRAW_SAMPLE_H_

#include "../application.h"

class DebugDrawSample : public Application
{
public:
    DebugDrawSample(const char *_name, const char *_description)
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

#endif //_DEBUG_DRAW_SAMPLE_H_
