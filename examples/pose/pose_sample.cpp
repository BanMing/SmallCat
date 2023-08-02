#include "pose_sample.h"
#include "../../gltf/gltf_loader.h"
#include "../../graphic/debug_draw.h"

void PoseSample::onInit()
{
    ddInit();
    cgltf_data *data = loadGLTFFile("../../assets/Woman.gltf");
    m_resetPose = loadRestPose(data);
    freeGLTFFile(data);
}

void PoseSample::onShutdown()
{
    ddShutdown();
}

void PoseSample::onUpdate(float _deltaTime)
{
    const bx::Vec3 at = {0.0f, 3.0f, 0.0f};
    const bx::Vec3 eye = {0.0f, 0.0f, -10.0f};

    // Set view and projection matrix for view 0.
    {
        float view[16];
        bx::mtxLookAt(view, eye, at);

        float proj[16];
        bx::mtxProj(proj, 60.0f, float(m_width) / float(m_height), 0.1f, 100.0f, bgfx::getCaps()->homogeneousDepth);
        bgfx::setViewTransform(0, view, proj);
    }
}

void PoseSample::onRender(float _inAspectRatio)
{
    DebugDraw dd;
    dd.begin(0);
    dd.drawPose(m_resetPose);
    dd.end();
}

void PoseSample::onGui()
{
}
