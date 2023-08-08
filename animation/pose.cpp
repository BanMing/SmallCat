#include "pose.h"

Pose::Pose()
{
}

Pose::Pose(const Pose &_pose)
{
    *this = _pose;
}

Pose::Pose(size_t _jointsNum)
{
    resizeJoints(_jointsNum);
}

Pose &Pose::operator=(const Pose &_pose)
{
    if (&_pose == this)
    {
        return *this;
    }

    if (m_parents.size() != _pose.m_parents.size())
    {
        m_parents.resize(_pose.m_parents.size());
    }
    if (m_parents.size() > 0)
    {
        memcpy(&m_parents[0], &_pose.m_parents[0], sizeof(int) * m_parents.size());
    }

    if (m_joints.size() != _pose.m_joints.size())
    {
        m_joints.resize(_pose.m_joints.size());
    }

    if (m_joints.size() > 0)
    {
        memcpy(&m_joints[0], &_pose.m_joints[0], sizeof(Transform) * m_joints.size());
    }

    return *this;
}

void Pose::resizeJoints(size_t _size)
{
    m_parents.resize(_size);
    m_joints.resize(_size);
}

size_t Pose::getJointsSize() const
{
    return m_parents.size();
}

int Pose::getParent(size_t _jointID) const
{
    return m_parents[_jointID];
}

void Pose::setParent(size_t _jointID, size_t _parentJointID)
{
    m_parents[_jointID] = _parentJointID;
}

Transform Pose::getLocalTransfrom(size_t _jointID) const
{
    return m_joints[_jointID];
}

void Pose::setLocalTransfrom(size_t _jointID, const Transform &_transfrom)
{
    m_joints[_jointID] = _transfrom;
}

Transform Pose::getGlobalTransform(size_t _jointID) const
{
    // int curIndex = _jointID;
    // int parentIndex = m_parents[curIndex];
    Transform res = m_joints[_jointID];
    // while (parentIndex >= 0)
    // {
    //     res = getTargetSpaceTransform(m_joints[parentIndex], m_joints[curIndex]);
    //     curIndex = parentIndex;
    //     parentIndex = m_parents[parentIndex];
    // }
    for (int parent = m_parents[_jointID]; parent >= 0; parent = m_parents[parent])
    {
        res = getTargetSpaceTransform(m_joints[parent], res);
    }

    return res;
}

Transform Pose::operator[](size_t _jointID) const
{
    return getGlobalTransform(_jointID);
}

void Pose::getMatrixPalette(std::vector<Matrix4> &_out) const
{
    size_t size = getJointsSize();
    if (size != _out.size())
    {
        _out.resize(size);
    }

    for (size_t i = 0; i < size; i++)
    {
        Transform globalTrans = getGlobalTransform(i);
        _out[i] = transformToMat4(globalTrans);
    }
}