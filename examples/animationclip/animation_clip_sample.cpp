#include "animation_clip_sample.h"
#include "../../gltf/gltf_loader.h"
#include "../../graphic/debug_draw.h"

#include <camera.h>
#include <debugdraw/debugdraw.h>

void AnimationClipSample::onInit()
{
    cgltf_data *data = loadGLTFFile("../../assets/Woman.gltf");
    m_clips = loadAnimationClips(data);
    m_restPose = loadRestPose(data);
    m_curPose = m_restPose;
    freeGLTFFile(data);

    ddInit();

    m_curPlayClipIndex = 0;
    m_curPlaybackTime = 0;
    for (size_t i = 0; i < m_clips.size(); i++)
    {
        m_clipNames += m_clips[i].getName() + '\0';
    }
}

void AnimationClipSample::onShutdown()
{
    ddShutdown();
}

void AnimationClipSample::onUpdate(float _deltaTime)
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
    m_curPlaybackTime = m_clips[m_curPlayClipIndex].sample(m_curPlaybackTime + _deltaTime, m_curPose);
}

void AnimationClipSample::onRender(float _inAspectRatio)
{
    DebugDraw dde;

    dde.begin(0);
    dde.drawAxis(0.0f, 0.0f, 0.0f);
    // dde.drawPose(m_restPose, 0xff0000ff);
    dde.drawPose(m_curPose);
    dde.end();
}

void AnimationClipSample::onGui()
{
    ImGui::ShowDemoWindow();
    ImGui::SetNextWindowPos(
        ImVec2(m_width - 310.0f, 50.0f), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(
        ImVec2(300.0f, 210.0f), ImGuiCond_FirstUseEver);

    ImGui::Begin("Animation Clips");
    ImGui::Text("Playback Time: %f", m_curPlaybackTime);
    ImGui::Combo("Current Clip", &m_curPlayClipIndex, m_clipNames.c_str());
    ImGui::End();
}
