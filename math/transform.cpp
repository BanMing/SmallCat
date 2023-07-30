#include "transform.h"
#include "constants.h"
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
    // M = SRT
    Transform res;
    res.position = Vector3(_m.m[12], _m.m[13], _m.m[14]);

    res.scale.x = sqrt(_m.m[0] * _m.m[0] + _m.m[1] * _m.m[1] + _m.m[2] * _m.m[2]);
    res.scale.y = sqrt(_m.m[4] * _m.m[4] + _m.m[5] * _m.m[5] + _m.m[6] * _m.m[6]);
    res.scale.z = sqrt(_m.m[8] * _m.m[8] + _m.m[9] * _m.m[9] + _m.m[10] * _m.m[10]);

    Matrix4 m = _m;
    float rn;
    // Factor the scale out of the matrix axes.
    rn = 1.0F / res.scale.x;
    m.cloumn[0].x *= rn;
    m.cloumn[0].y *= rn;
    m.cloumn[0].z *= rn;

    rn = 1.0F / res.scale.y;
    m.cloumn[1].x *= rn;
    m.cloumn[1].y *= rn;
    m.cloumn[1].z *= rn;

    rn = 1.0F / res.scale.z;
    m.cloumn[2].x *= rn;
    m.cloumn[2].y *= rn;
    m.cloumn[2].z *= rn;

    res.rotation = mat4ToQuat(m);
    return res;
}

Transform getTargetSpaceTransform(const Transform &_target, const Transform &_t)
{
    Transform res;
    res.scale = _target.scale * _t.scale;
    // TODO: Find out 
    res.rotation = _t.rotation * _target.rotation;

    res.position = _target.rotation * (_target.scale * _t.position);
    res.position = _target.position + res.position;

    return res;
}

Vector3 transformPoint(const Transform &_t, const Vector3 &_p)
{
    Vector3 res;
    res = _t.rotation * (_t.scale * _p);
    res = _t.position + res;
    return res;
}

Vector3 transformVector(const Transform &_t, const Vector3 &_v)
{
    Vector3 res;
    res = _t.rotation * (_t.scale * _v);
    return res;
}

Transform inverse(const Transform &_t)
{
    Transform res;
    res.rotation = inverse(_t.rotation);

    res.scale.x = fabs(_t.scale.x) < kFloatEpsilon ? 0.0f : 1.0f / _t.scale.x;
    res.scale.y = fabs(_t.scale.y) < kFloatEpsilon ? 0.0f : 1.0f / _t.scale.y;
    res.scale.z = fabs(_t.scale.z) < kFloatEpsilon ? 0.0f : 1.0f / _t.scale.z;

    Vector3 invTranslation = _t.position * -1.0f;
    res.position = res.rotation * (res.scale * invTranslation);
    return res;
}