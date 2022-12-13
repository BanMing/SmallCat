#include "animation_clip_sample.h"
#include "../../gltf/gltf_loader.h"
#include <debugdraw/debugdraw.h>
#include <camera.h>

void AnimationClipSample::onInit()
{
    cgltf_data *data = loadGLTFFile("../assets/Woman.gltf");
    m_clips = loadAnimationClips(data);
    freeGLTFFile(data);

    cameraCreate();

    cameraSetPosition({0.0f, 2.0f, -12.0f});
    cameraSetVerticalAngle(0.0f);

    ddInit();
}

void AnimationClipSample::onShutdown()
{
    ddShutdown();
    cameraDestroy();
}

void AnimationClipSample::onUpdate(float _deltaTime)
{
    // Update camera.
    // cameraUpdate(_deltaTime, m_mouseState, ImGui::MouseOverArea());
}

void AnimationClipSample::onRender(float _inAspectRatio)
{
    float view[16];
    cameraGetViewMtx(view);

    float proj[16];

    // Set view and projection matrix for view 0.
    {
        bx::mtxProj(proj, 60.0f, float(m_width) / float(m_height), 0.1f, 100.0f, bgfx::getCaps()->homogeneousDepth);

        bgfx::setViewTransform(0, view, proj);
        bgfx::setViewRect(0, 0, 0, uint16_t(m_width), uint16_t(m_height));
    }

    // float mtxVp[16];
    // bx::mtxMul(mtxVp, view, proj);

    // float mtxInvVp[16];
    // bx::mtxInverse(mtxInvVp, mtxVp);

    // const bx::Vec3 at = {0.0f, 0.0f, 0.0f};
    // const bx::Vec3 eye = {5.0f, 10.0f, 5.0f};
    // bx::mtxLookAt(view, eye, at);
    // bx::mtxProj(proj, 45.0f, float(m_width) / float(m_height), 1.0f, 15.0f, bgfx::getCaps()->homogeneousDepth);
    // bx::mtxMul(mtxVp, view, proj);

    DebugDrawEncoder dde;

    dde.begin(0);
    dde.drawAxis(0.0f, 0.0f, 0.0f);
    // dde.drawFrustum(mtxVp);
    dde.end();
}

void AnimationClipSample::onGui()
{
}
