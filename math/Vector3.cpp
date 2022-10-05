#include "Vector3.h"

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