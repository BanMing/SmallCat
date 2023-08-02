#include "debug_draw.h"

void DebugDraw::drawPose(const Pose &_pose)
{
    const size_t jointNum = _pose.getJointsSize();
    m_poseVertices.resize(jointNum * 2);
    for (size_t i = 0; i < jointNum; i++)
    {
        int parentIndex = _pose.getParent(i);
        if (parentIndex >= 0)
        {
            Vector3 jointPos = _pose.getGlobalTransform(i).position;
            Vector3 parentPos = _pose.getGlobalTransform(parentIndex).position;
            m_poseVertices[i * 2] = {jointPos.x, jointPos.y, jointPos.z};
            m_poseVertices[i * 2 + 1] = {parentPos.x, parentPos.y, parentPos.z};
        }
    }
    drawLineList(jointNum * 2, &m_poseVertices[0]);
}
