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

    Vector3() : x(0.0f), y(0.0f), z(0.0f) {}
    Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
    Vector3(float *_fv) : x(_fv[0]), y(_fv[1]), z(_fv[2]) {}
};

float lenSq(const Vector3 &_v);
float len(const Vector3 &_v);
void normalize(Vector3 &_v);
Vector3 normalized(const Vector3 &_v);

Vector3 operator+(const Vector3 &_l, const Vector3 &_r);
Vector3 operator-(const Vector3 &_l, const Vector3 &_r);
Vector3 operator*(const Vector3 &_v, float _scaler);

float dot(const Vector3 &_l, const Vector3 &_r);
Vector3 cross(const Vector3 &_l, const Vector3 &_r);

float angle(const Vector3 &_l, const Vector3 &_r);
Vector3 project(const Vector3 &_a, const Vector3 &_b);

bool operator==(const Vector3 &_l, const Vector3 &_r);
bool operator!=(const Vector3 &_l, const Vector3 &_r);

/// @brief (0, 0, 0)
static const Vector3 kVector3Zero;
// @brief (1, 0, 0)
static const Vector3 kVector3Right(1, 0, 0);
/// @brief (0, 1, 0)
static const Vector3 kVector3Up(0, 1, 0);
/// @brief (0, 0, 1)
static const Vector3 kVector3Forward(0, 0, 1);
/// @brief (-1, 0, 0)
static const Vector3 kVector3Left(-1, 0, 0);
/// @brief (0, -1, 0)
static const Vector3 kVector3Down(0, -1, 0);
/// @brief (0, 0, -1)
static const Vector3 kVector3Back(0, 0, -1);

#endif //_VECTOR3_H_