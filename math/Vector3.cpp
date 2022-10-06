#include "constants.h"
#include "vector3.h"
#include <cmath>

float lenSq(const Vector3 &_v)
{
    return _v.x * _v.x + _v.y * _v.y + _v.z * _v.z;
}

float len(const Vector3 &_v)
{
    return sqrtf(_v.x * _v.x + _v.y * _v.y + _v.z * _v.z);
}

void normalize(Vector3 &_v)
{
    float lenSq = _v.x * _v.x + _v.y * _v.y + _v.z * _v.z;
    if (lenSq < kFloatEpsilon)
    {
        return;
    }

    float invLen = 1.0f / sqrtf(lenSq);
    _v.x *= invLen;
    _v.y *= invLen;
    _v.z *= invLen;
}

Vector3 normalized(const Vector3 &_v)
{
    float lenSq = _v.x * _v.x + _v.y * _v.y + _v.z * _v.z;
    if (lenSq < kFloatEpsilon)
    {
        return _v;
    }

    float invLen = 1.0f / sqrtf(lenSq);
    return Vector3(_v.x * invLen,
                   _v.y * invLen,
                   _v.z * invLen);
}

Vector3 operator+(const Vector3 &_l, const Vector3 &_r)
{
    return Vector3(_l.x + _r.x, _l.y + _r.y, _l.y + _r.y);
}

Vector3 operator-(const Vector3 &_l, const Vector3 &_r)
{
    return Vector3(_l.x - _r.x, _l.y - _r.y, _l.y - _r.y);
}

Vector3 operator*(const Vector3 &_v, float _scaler)
{
    return Vector3(_v.x * _scaler, _v.y * _scaler, _v.z * _scaler);
}

float dot(const Vector3 &_l, const Vector3 &_r)
{
    return _l.x * _r.x + _l.y * _r.y + _l.z * _r.z;
}

Vector3 cross(const Vector3 &_l, const Vector3 &_r)
{
    return Vector3(_l.y * _r.z - _l.z * _r.y,
                   _l.z * _r.x - _l.x * _r.z,
                   _l.x * _r.y - _l.y * _r.x);
}

float angle(const Vector3 &_l, const Vector3 &_r)
{
    float lenSqL = _l.x * _l.x + _l.y * _l.y + _l.z * _l.z;
    float lenSqR = _r.x * _r.x + _r.y * _r.y + _r.z * _r.z;
    if (lenSqL < kFloatEpsilon || lenSqR < kFloatEpsilon)
    {
        return 0.0f;
    }

    float dot = _l.x * _r.x + _l.y * _r.y + _l.z * _r.z;
    float len = sqrtf(lenSqL) * sqrtf(lenSqR);
    return acosf(dot / len);
}

Vector3 project(const Vector3 &_a, const Vector3 &_b)
{
    float lenSqb = len(_b);
    if (lenSqb < kFloatEpsilon)
    {
        return Vector3();
    }

    float scale = dot(_a, _b) / lenSqb;
    return _b * scale;
}

bool operator==(const Vector3 &_l, const Vector3 &_r)
{
    return _l.x - _r.x < kFloatEpsilon &&
           _l.y - _r.y < kFloatEpsilon &&
           _l.z - _r.z < kFloatEpsilon;
}

bool operator!=(const Vector3 &_l, const Vector3 &_r)
{
    return _l.x - _r.x > kFloatEpsilon ||
           _l.y - _r.y > kFloatEpsilon ||
           _l.z - _r.z > kFloatEpsilon;
}