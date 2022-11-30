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

#endif //_TRANSFORM_H_