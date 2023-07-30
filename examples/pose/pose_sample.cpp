#include "pose_sample.h"
#include "../../gltf/gltf_loader.h"
#include <debugdraw/debugdraw.h>
#pragma optimize("", off)
std::vector<DdVertex> poseVertices;

void PoseSample::onInit()
{
    ddInit();
    printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
    cgltf_data *data = loadGLTFFile("../../assets/Woman.gltf");
    m_resetPose = loadRestPose(data);
    freeGLTFFile(data);
    const size_t numVertices = m_resetPose.getJointsSize();
    poseVertices.resize(numVertices * 2);
    for (size_t i = 0; i < numVertices; i++)
    {
        // printf("parent:%d\n", m_resetPose.getParent(i));
        Transform localJoint = m_resetPose.getLocalTransfrom(i);
        Transform joint = m_resetPose.getGlobalTransform(i);
        printf("local position [%f,%f,%f]\n", localJoint.position.x, localJoint.position.y, localJoint.position.z);
        printf("local rotation [%f,%f,%f,%f]\n", localJoint.rotation.x, localJoint.rotation.y, localJoint.rotation.z, localJoint.rotation.w);
        // printf("world position [%f,%f,%f]\n", joint.position.x, joint.position.y, joint.position.z);
        // poseVertices[i] = {joint.position.x, joint.position.y, joint.position.z};
        // if (i != 0)
        // {
        //     poseVertices[i + 1] = {joint.position.x, joint.position.y, joint.position.z};
        // }
    }
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
    DebugDrawEncoder dde;
    dde.begin(0);
    const size_t numVertices = m_resetPose.getJointsSize();

    // void drawLineList(uint32_t _numVertices, const DdVertex* _vertices, uint32_t _numIndices = 0, const uint16_t* _indices = NULL);
    // dde.drawLineList(numVertices, &poseVertices[0]);
    // printf("@@@@@@@@@@@@@@@@\n");
    dde.drawAxis(0, 0, 0, 10);
    // Transform joint = m_resetPose.getGlobalTransform(0);
    // dde.drawCapsule(joint.position.bxVector3, joint.position.bxVector3, 0.1f);
    for (size_t i = 0; i < numVertices; i++)
    {
        Transform joint = m_resetPose.getGlobalTransform(i);
        // poseVertices[i] = {joint.position.x, joint.position.y, joint.position.z};
        // if (i != 0)
        // {
        //     poseVertices[i + 1] = {joint.position.x, joint.position.y, joint.position.z};
        // }
        // printf("position [%f,%f,%f]\n", joint.position.x, joint.position.y, joint.position.z);
        dde.drawCapsule(joint.position.bxVector3, joint.position.bxVector3, 0.1f);
    }
    dde.end();
}

void PoseSample::onGui()
{
}
