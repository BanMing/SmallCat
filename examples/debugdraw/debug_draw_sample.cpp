#include "debug_draw_sample.h"

void DebugDrawSample::onInit()
{
}

void DebugDrawSample::onShutdown()
{
}

void DebugDrawSample::onUpdate(float _deltaTime)
{
    const bx::Vec3 at = {0.0f, 0.0f, 0.0f};
    const bx::Vec3 eye = {0.0f, 0.0f, -35.0f};

    // Set view and projection matrix for view 0.
    {
        float view[16];
        bx::mtxLookAt(view, eye, at);

        float proj[16];
        bx::mtxProj(proj, 60.0f, float(m_width) / float(m_height), 0.1f, 100.0f, bgfx::getCaps()->homogeneousDepth);
        bgfx::setViewTransform(0, view, proj);
    }
}

void DebugDrawSample::onRender(float _inAspectRatio)
{
}

void DebugDrawSample::onGui()
{
}
