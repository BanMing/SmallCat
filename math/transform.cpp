#include "transform.h"
#include <cmath>

Matrix4 transformToMat4(const Transform &_trans)
{
    Matrix4 rotationMat4 = quatToMat4(_trans.rotation);
    Matrix4 res;

    res.m[0] = rotationMat4.m[0] * _trans.scale.x;
    res.m[1] = rotationMat4.m[1] * _trans.scale.x;
    res.m[2] = rotationMat4.m[2] * _trans.scale.x;
    res.m[3] = 0;

    res.m[4] = rotationMat4.m[4] * _trans.scale.y;
    res.m[5] = rotationMat4.m[5] * _trans.scale.y;
    res.m[6] = rotationMat4.m[6] * _trans.scale.y;
    res.m[7] = 0;

    res.m[8] = rotationMat4.m[8] * _trans.scale.z;
    res.m[9] = rotationMat4.m[9] * _trans.scale.z;
    res.m[10] = rotationMat4.m[10] * _trans.scale.z;
    res.m[11] = 0;

    res.m[12] = _trans.position.x;
    res.m[13] = _trans.position.y;
    res.m[14] = _trans.position.z;
    res.m[15] = 1;

    return res;
}

Transform mat4ToTransform(const Matrix4 &_m)
{
    Transform res;
    res.position = Vector3(_m.m[12], _m.m[13], _m.m[14]);
    res.rotation = mat4ToQuat(_m);

    res.scale.x = sqrt(_m.m[0] * _m.m[0] + _m.m[1] * _m.m[1] + _m.m[2] * _m.m[2]);
    res.scale.y = sqrt(_m.m[4] * _m.m[4] + _m.m[5] * _m.m[5] + _m.m[6] * _m.m[6]);
    res.scale.z = sqrt(_m.m[8] * _m.m[8] + _m.m[9] * _m.m[9] + _m.m[10] * _m.m[10]);

    return res;
}