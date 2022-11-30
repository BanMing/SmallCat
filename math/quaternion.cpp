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
        return Quaternion();
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

Vector3 operator*(const Quaternion &_q, const Vector3 &_v)
{
    // V' = V + 2w(Q x V) + (2Q x (Q x V))
    // refactor:
    // V' = V + w(2(Q x V)) + (Q x (2(Q x V)))
    // T = 2(Q x V);
    // V' = V + w*(T) + (Q x T)
    // const FVector Q(X, Y, Z);
    // T-> const FVector T = 2.f * FVector::CrossProduct(Q, V);
    // const FVector Result = V + (W * T) + FVector::CrossProduct(Q, T);

    // return Result;
    Vector3 q(_q.x, _q.y, _q.z);
    Vector3 t = cross(q, _v) * 2.0f;
    return _v + (t * _q.w) + cross(q, t);
}

Quaternion operator*(const Quaternion &_a, const Quaternion &_b)
{
    return Quaternion(
        _a.w * _b.x + _a.x * _b.w + _a.y * _b.z - _a.z * _b.y,
        _a.w * _b.y + _a.y * _b.w + _a.z * _b.x - _a.x * _b.z,
        _a.w * _b.z + _a.z * _b.w + _a.x * _b.y - _a.y * _b.x,
        _a.w * _b.w - _a.x * _b.x - _a.y * _b.y - _a.z * _b.z);
}

bool operator==(const Quaternion &_a, const Quaternion &_b)
{
    return _a.x - _b.x < kFloatEpsilon &&
           _a.y - _b.y < kFloatEpsilon &&
           _a.z - _b.z < kFloatEpsilon &&
           _a.w - _b.w < kFloatEpsilon;
}

bool operator!=(const Quaternion &_a, const Quaternion &_b)
{
    return _a.x - _b.x > kFloatEpsilon ||
           _a.y - _b.y > kFloatEpsilon ||
           _a.z - _b.z > kFloatEpsilon ||
           _a.w - _b.w > kFloatEpsilon;
}

float dot(const Quaternion &_a, const Quaternion &_b)
{
    return _a.x * _b.x + _a.y * _b.y + _a.z * _b.z + _a.w * _b.w;
}

Quaternion conjugate(const Quaternion &_q)
{
    return Quaternion(-_q.x, -_q.y, -_q.z, _q.w);
}

Quaternion inverse(const Quaternion &_q)
{
    float lenSq = _q.x * _q.x + _q.y * _q.y + _q.z * _q.z + _q.w * _q.w;
    if (lenSq < kFloatEpsilon)
    {
        return Quaternion();
    }
    float invLen = 1.0f / lenSq;

    // conjugate / normalize
    return Quaternion(
        -_q.x * invLen,
        -_q.y * invLen,
        -_q.z * invLen,
        _q.w * invLen);
}

Quaternion angleAxis(float _angle, const Vector3 &_axis)
{
    float halfAngle = _angle * 0.5f;
    Vector3 axis = normalized(_axis) * sin(halfAngle);
    return Quaternion(axis.x, axis.y, axis.z, cos(halfAngle));
}

Vector3 getAxis(const Quaternion &_q)
{
    return Vector3(_q.x, _q.y, _q.z);
}

float getAngle(const Quaternion &_q)
{
    return 2.0f * acos(_q.w);
}

Quaternion fromVectors(const Vector3 &_from, const Vector3 &_to)
{
    if (_from == _to)
    {
        return Quaternion();
    }

    if (_from == _to * -1)
    {
        Vector3 ortho = Vector3(1, 0, 0);
        if (fabsf(_from.y) < fabsf(_from.x))
        {
            ortho = Vector3(0, 1, 0);
        }

        if (fabsf(_from.z) < fabsf(_from.y) && fabsf(_from.z) < fabsf(_from.x))
        {
            ortho = Vector3(0, 0, 1);
        }

        Vector3 axis = normalized(cross(_from, ortho));
        return Quaternion(axis.x, axis.y, axis.z, 0);
    }
    else
    {
        Vector3 w = cross(_from, _to);
        float d = dot(_from, _to);
        return normalized(Quaternion(w.x, w.y, w.z, d + sqrt(d * d + dot(w, w))));
    }
}

Quaternion fromUnitVectors(const Vector3 &_from, const Vector3 &_to)
{
    if (_from == _to)
    {
        return Quaternion();
    }

    if (_from == _to * -1)
    {
        Vector3 ortho = Vector3(1, 0, 0);
        if (fabsf(_from.y) < fabsf(_from.x))
        {
            ortho = Vector3(0, 1, 0);
        }

        if (fabsf(_from.z) < fabsf(_from.y) && fabsf(_from.z) < fabsf(_from.x))
        {
            ortho = Vector3(0, 0, 1);
        }

        Vector3 axis = normalized(cross(_from, ortho));
        return Quaternion(axis.x, axis.y, axis.z, 0);
    }
    else
    {
        Vector3 w = cross(_from, _to);
        float d = dot(_from, _to);
        return normalized(Quaternion(w.x, w.y, w.z, 1 + d));
    }
}

Matrix4 quatToMat4(const Quaternion &_q)
{
    float x2 = _q.x + _q.x;
    float y2 = _q.y + _q.y;
    float z2 = _q.z + _q.z;

    float xx2 = _q.x * x2;
    float yy2 = _q.y * y2;
    float zz2 = _q.z * z2;
    float xy2 = _q.x * y2;
    float xz2 = _q.x * z2;
    float yz2 = _q.y * z2;
    float wx2 = _q.w * x2;
    float wy2 = _q.w * y2;
    float wz2 = _q.w * z2;

    return Matrix4(1.0f - yy2 - zz2, xy2 + wz2, xz2 - wy2, 0.0f,
                   xy2 - wz2, 1.0f - xx2 - zz2, yz2 + wx2, 0.0f,
                   xz2 + wy2, yz2 - wx2, 1.0f - xx2 - yy2, 0.0f,
                   0.0f, 0.0f, 0.0f, 1.0f);
}

Quaternion mat4ToQuat(const Matrix4 &_m)
{
    float trace = _m.cloumn[0].v[0] + _m.cloumn[1].v[1] + _m.cloumn[2].v[2];
    if (trace >= kFloatEpsilon)
    {
        float s = sqrt(trace + 1.0f);
        float is = 0.5f / s;
        return Quaternion((_m.cloumn[1].v[2] - _m.cloumn[2].v[1]) * is,
                          (_m.cloumn[2].v[0] - _m.cloumn[0].v[2]) * is,
                          (_m.cloumn[0].v[1] - _m.cloumn[1].v[0]) * is,
                          0.5f * s);
    }
    else
    {
        int i = 0;
        if (_m.cloumn[1].v[1] > _m.cloumn[0].v[0])
        {
            i = 1;
        }
        if (_m.cloumn[2].v[2] > _m.cloumn[i].v[i])
        {
            i = 2;
        }

        if (i == 0)
        {
            float s = sqrt(_m.cloumn[0].v[0] - (_m.cloumn[1].v[1] + _m.cloumn[2].v[2]) + 1);
            float is = 0.5f / s;
            return Quaternion(0.5f * s,
                        (_m.cloumn[1].v[0] + _m.cloumn[0].v[1]) * is,
                        (_m.cloumn[0].v[2] + _m.cloumn[2].v[0]) * is,
                        (_m.cloumn[1].v[2] - _m.cloumn[2].v[1]) * is);
        }
        else if (i == 1)
        {
            float s = sqrt(_m.cloumn[1].v[1] - (_m.cloumn[2].v[2] + _m.cloumn[0].v[0]) + 1);
            float is = 0.5f / s;
            return Quaternion((_m.cloumn[1].v[0] + _m.cloumn[0].v[1]) * is,
                        0.5f * s,
                        (_m.cloumn[2].v[1] + _m.cloumn[1].v[2]) * is,
                        (_m.cloumn[2].v[0] - _m.cloumn[0].v[2]) * is);
        }
        else
        {
            float s = sqrt(_m.cloumn[2].v[2] - (_m.cloumn[0].v[0] + _m.cloumn[1].v[1]) + 1);
            float is = 0.5f / s;
            return Quaternion((_m.cloumn[0].v[2] + _m.cloumn[2].v[0]) * is,
                        (_m.cloumn[2].v[1] + _m.cloumn[1].v[2]) * is,
                        0.5f * s,
                        (_m.cloumn[0].v[1] - _m.cloumn[1].v[0]) * is);
        }
    }
}