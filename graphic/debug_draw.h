#ifndef _DEBUG_DRAW_H_
#define _DEBUG_DRAW_H_

#include "../animation/pose.h"
#include <debugdraw/debugdraw.h>
#include <vector>

struct DebugDraw : DebugDrawEncoder
{
private:
    std::vector<DdVertex> m_poseVertices;

public:
    void drawPose(const Pose &_pose);
};
#endif //_DEBUG_DRAW_H_