#ifndef _SKELETON_H_
#define _SKELETON_H_

#include "../math/matrix4.h"
#include "pose.h"
#include <string>
#include <vector>

class Skeleton
{
protected:
    Pose m_restPose;
    Pose m_bindPose;
    std::vector<Matrix4> m_invBindPose;
    std::vector<std::string> m_jointNames;

public:
    Skeleton();
    Skeleton(const Pose &_restPose, const Pose &_bindPose, const std::vector<std::string> &_names);
    ~Skeleton();

    void initialize(const Pose &_restPose, const Pose &_bindPose, const std::vector<std::string> &_names);

    Pose &getBindPose();
    Pose &getRestPose();
    std::vector<Matrix4> &getInvBindPose();
    std::vector<std::string> &getJointNames();
    std::string &getJointName(int index);
};

#endif //_SKELETON_H_