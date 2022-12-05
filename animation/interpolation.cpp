#include "interpolation.h"
#include "../math/constants.h"
#include <cmath>

// Vector3 Interpolation
Vector3 lerp(const Vector3 &_from, const Vector3 &_to, float _t)
{
    return _from + (_to - _from) * _t;
}

Vector3 nlerp(const Vector3 &_from, const Vector3 &_to, float _t)
{
    return normalized(_from + (_to - _from) * _t);
}

// write by formula
Vector3 slerp(const Vector3 &_from, const Vector3 &_to, float _t)
{
    Vector3 fromDir = normalized(_from);
    Vector3 toDir = normalized(_to);
    float theta = angle(fromDir, toDir);

    return (fromDir * sin(1 - _t) * theta + toDir * sin(_t * theta)) / sin(theta);
}

// Quaternion Interpolation
Quaternion lerp(const Quaternion &_from, const Quaternion &_to, float _t)
{
    // To ensure the 'shortest route', we make sure the dot product between the both rotations is positive.
    float bias = 1.0f;
    if (dot(_from, _to) < kFloatEpsilon)
    {
        bias = -1.0f;
    }
    return _from + (_to * bias - _from) * _t;
}

Quaternion nlerp(const Quaternion &_from, const Quaternion &_to, float _t)
{
    // To ensure the 'shortest route', we make sure the dot product between the both rotations is positive.
    float bias = 1.0f;
    if (dot(_from, _to) < kFloatEpsilon)
    {
        bias = -1.0f;
    }

    return normalized(_from + (_to * bias - _from) * _t);
}

Quaternion slerp(const Quaternion &_from, const Quaternion &_to, float _t)
{
    if (fabs(dot(_from, _to)) > 1.0f - kFloatEpsilon)
    {
        return nlerp(_from, _to, _t);
    }

    return normalized((_to * inverse(_from) ^ _t) * _from);
}

// Cubic Interpolation
Vector3 hermite(const Vector3 &_p1, const Vector3 &_m1, const Vector3 &_p2, const Vector3 &_m2, float _t)
{
    // Calculate factors for Cubic Hermite Spline
    // See: https://en.wikipedia.org/wiki/Cubic_Hermite_spline

    float tt = _t * _t;
    float ttt = tt * _t;

    float h00 = 2.0f * ttt - 3.0f * tt + 1.0f;
    float h10 = -2.0f * ttt + 3.0f * tt;
    float h01 = ttt - 2.0f * tt + _t;
    float h11 = ttt - tt;

    return (_p1 * h00) + (_p2 * h10) + (_m1 * h01) + (_m2 * h11);
}

Quaternion hermite(const Quaternion &_p1, const Quaternion &_m1, const Quaternion &_p2, const Quaternion &_m2, float _t)
{
    // To ensure the 'shortest route', we make sure the dot product between the both rotations is positive.
    float bias = 1.0f;
    if (dot(_p1, _p2) < kFloatEpsilon)
    {
        bias = -1.0f;
    }
    Quaternion p1 = normalized(_p1);
    Quaternion p2 = normalized(_p2);

    float tt = _t * _t;
    float ttt = tt * _t;

    float h00 = 2.0f * ttt - 3.0f * tt + 1.0f;
    float h10 = -2.0f * ttt + 3.0f * tt;
    float h01 = ttt - 2.0f * tt + _t;
    float h11 = ttt - tt;

    return normalized((p1 * h00) + (p2 * bias * h10) + (_m1 * h01) + (_m2 * h11));
}