#ifndef _VECTOR3_H_
#define _VECTOR3_H_

struct Vector3
{
    union
    {
        float v[3];

        struct
        {
            float x, y, z;
        };
    };

    inline Vector3() : x(0.0f), y(0.0f), z(0.0f) {}
    inline Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
    inline Vector3(float *_fv) : x(_fv[0]), y(_fv[1]), z(_fv[2]) {}
};

Vector3 operator+(const Vector3 &_l, const Vector3 &_r);
Vector3 operator-(const Vector3 &_l, const Vector3 &_r);
Vector3 operator*(const Vector3 &_v, float _scaler);

float dot(const Vector3 &_l, const Vector3 &_r);
Vector3 cross(const Vector3 &_l, const Vector3 &_r);

#endif //_VECTOR3_H_