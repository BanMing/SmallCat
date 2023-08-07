#include "skeleton.h"
Skeleton::Skeleton() {}

Skeleton::Skeleton(const Pose &_restPose, const Pose &_bindPose, const std::vector<std::string> &_names)
{
    initialize(_restPose, _bindPose, _names);
}

Skeleton::~Skeleton() {}

void Skeleton::initialize(const Pose &_restPose, const Pose &_bindPose, const std::vector<std::string> &_names)
{
    m_restPose = _restPose;
    m_bindPose = _bindPose;
    m_jointNames = _names;

    size_t jointSize = m_bindPose.getJointsSize();
    m_invBindPose.resize(jointSize);

    for (size_t i = 0; i < jointSize; i++)
    {
        Transform worldTrans = m_bindPose.getGlobalTransform(i);
        m_invBindPose[i] = inverse(transformToMat4(worldTrans));
    }
}

Pose &Skeleton::getBindPose()
{
    return m_bindPose;
}

Pose &Skeleton::getRestPose()
{
    return m_restPose;
}

std::vector<Matrix4> &Skeleton::getInvBindPose()
{
    return m_invBindPose;
}

std::vector<std::string> &Skeleton::getJointNames()
{
    return m_jointNames;
}

std::string &Skeleton::getJointName(int index)
{
    return m_jointNames[index];
}
