#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

#include "vector3.h"
#include "quaternion.h"

struct Transform
{
    Vector3 position;
    Quaternion rotation;
    Vector3 scale;

    Transform() : position(Vector3()), rotation(Quaternion()), scale(Vector3(1, 1, 1)) {}
    Transform(const Vector3 &_position, const Quaternion &_rotation, const Vector3 &_scale) : position(_position), rotation(_rotation), scale(_scale) {}
};

Matrix4 transformToMat4(const Transform &_trans);
Transform mat4ToTransform(const Matrix4 &_m);

Transform getTargetSpaceTransform(const Transform& _target, const Transform& _t);

Transform inverse(const Transform& _t);
Vector3 transformPoint(const Transform &_t, const Vector3 &_p);
Vector3 transformVector(const Transform &_t, const Vector3 &_v);
#endif //_TRANSFORM_H_