#include "quaternion.h"
#include "constants.h"
#include <cmath>

float lenSq(const Quaternion &_q)
{
    return _q.x * _q.x + _q.y * _q.y + _q.z * _q.z + _q.w * _q.w;
}

float len(const Quaternion &_q)
{
    return sqrt(_q.x * _q.x + _q.y * _q.y + _q.z * _q.z + _q.w * _q.w);
}

void normalize(Quaternion &_q)
{
    float lenSq = _q.x * _q.x + _q.y * _q.y + _q.z * _q.z + _q.w * _q.w;
    if (lenSq < kFloatEpsilon)
    {
        return;
    }

    float invLen = 1.0f / sqrt(lenSq);

    _q.x *= invLen;
    _q.y *= invLen;
    _q.z *= invLen;
    _q.w *= invLen;
}

Quaternion normalized(const Quaternion &_q)
{
    float lenSq = _q.x * _q.x + _q.y * _q.y + _q.z * _q.z + _q.w * _q.w;
    if (lenSq < kFloatEpsilon)
    {
        return;
    }

    float invLen = 1.0f / sqrt(lenSq);

    return Quaternion(_q.x * invLen, _q.y * invLen, _q.z * invLen, _q.w * invLen);
}

Quaternion operator+(const Quaternion &_a, const Quaternion &_b)
{
    return Quaternion(_a.x + _b.x, _a.y + _b.y, _a.z + _b.z, _a.w + _b.w);
}

Quaternion operator-(const Quaternion &_a, const Quaternion &_b)
{
    return Quaternion(_a.x - _b.x, _a.y - _b.y, _a.z - _b.z, _a.w - _b.w);
}

Quaternion operator-(const Quaternion &_q)
{
    return Quaternion(-_q.x, -_q.y, -_q.z, -_q.w);
}

Quaternion operator*(const Quaternion &_q, float _s)
{
    return Quaternion(_q.x * _s, _q.y * _s, _q.z * _s, _q.w * _s);
}

Quaternion operator*(const Quaternion &_a, const Quaternion &_b)
{
    return Quaternion(
        _a.w * _b.x + _a.x * _b.w + _a.y * _b.z - _a.z * _b.y,
        _a.w * _b.y + _a.y * _b.w + _a.z * _b.x - _a.x * _b.z,
        _a.w * _b.z + _a.z * _b.w + _a.x * _b.y - _a.y * _b.x,
        _a.w * _b.w - _a.x * _b.x - _a.y * _b.y - _a.z * _b.z);
}

Quaternion angleAxis(float _angle, const Vector3 &_axis)
{
    float halfAngle = _angle * 0.5f;
    Vector3 axis = normalized(_axis) * sin(halfAngle);
    return Quaternion(axis.x, axis.y, axis.z, cos(halfAngle));
}

Quaternion fromToRotation(const Vector3 &_from, const Vector3 &_to)
{
    Vector3 from = normalized(_from);
    Vector3 to = normalized(_to);

    Vector3 c = cross(from, to);
    float d = dot(from, to);

    if (d < -1.0f + kFloatEpsilon)
    {
        return Quaternion(0, 1, 0, 0);
    }
    else
    {
        float s = sqrt((1.0f + d) * 2.0f);
        float rs = 1.0f / s;

        return Quaternion(c.x * rs, c.y * rs, c.z * rs, s * 0.5f);
    }
}