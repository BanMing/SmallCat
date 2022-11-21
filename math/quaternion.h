#ifndef _QUATERNION_H_
#define _QUATERNION_H_
#include "vector3.h"

struct Quaternion
{
    union
    {
        struct
        {
            float x;
            float y;
            float z;
            float w;
        };

        float v[4];
    };
    Quaternion() : x(0), y(0), z(0), w(1) {}
    Quaternion(float _x, float _y, float _z, float _w)
        : x(_x), y(_y), z(_z), w(_w) {}
};

float lenSq(const Quaternion &_q);
float len(const Quaternion &_q);
void normalize(Quaternion &_q);
Quaternion normalized(const Quaternion &_q);

Quaternion operator+(const Quaternion &_a, const Quaternion &_b);
Quaternion operator-(const Quaternion &_a, const Quaternion &_b);
Quaternion operator-(const Quaternion &_q);
Quaternion operator*(const Quaternion &_q, float _s);
Vector3 operator*(const Quaternion &_q, const Vector3 &_v);
Quaternion operator*(const Quaternion &_a, const Quaternion &_b);

bool operator==(const Quaternion &_a, const Quaternion &_b);
bool operator!=(const Quaternion &_a, const Quaternion &_b);

float dot(const Quaternion &_a, const Quaternion &_b);
Quaternion conjugate(const Quaternion &_q);
Quaternion inverse(const Quaternion &_q);

Quaternion angleAxis(float _angle, const Vector3 &_axis);
Vector3 getAxis(const Quaternion& _q);
float getAngle(const Quaternion& _q);

Quaternion fromVectors(const Vector3 &_from, const Vector3 &_to);
Quaternion fromUnitVectors(const Vector3 &_from, const Vector3 &_to);

/// @brief (0, 0, 0, 1)
static const Quaternion kIdentity;
#endif //_QUATERNION_H_