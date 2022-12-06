#ifndef _POSE_H_
#define _POSE_H_

#include "../math/transform.h"
#include <vector>

class Pose
{
protected:
    std::vector<Transform> m_joints;
    std::vector<int> m_parents;

public:
    Pose();
    Pose(const Pose &_pose);
    Pose &operator=(const Pose &_pose);
    Pose(size_t _jointsNum);

    void ResizeJoints(size_t _size);
    size_t GetJointsSize() const;

    int GetParent(size_t _index) const;
    void SetParent(size_t _index, size_t _parent);

    Transform GetLocalTransfrom(size_t _index) const;
    void SetLocalTransfrom(size_t _index, const Transform &_transfrom);

    
};

#endif //_POSE_H_