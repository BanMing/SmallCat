#ifndef _MATRIX4_H_
#define _MATRIX4_H_

#include "vector3.h"
#include "vector4.h"

struct Matrix4
{
    union
    {
        Vector4 cloumn[4];
        float m[16];
        struct
        {
            float xx;
            float xy;
            float xz;
            float xw;
            float yx;
            float yy;
            float yz;
            float yw;
            float zx;
            float zy;
            float zz;
            float zw;
            float tx;
            float ty;
            float tz;
            float tw;
        };
    };

    static const Matrix4 Identity;

    Matrix4() : xx(1), xy(0), xz(0), xw(0),
                yx(0), yy(1), yz(0), yw(0),
                zx(0), zy(0), zz(1), zw(0),
                tx(0), ty(0), tz(0), tw(1) {}

    Matrix4(float *_fv) : xx(_fv[0]), xy(_fv[1]), xz(_fv[2]), xw(_fv[3]),
                          yx(_fv[4]), yy(_fv[5]), yz(_fv[6]), yw(_fv[7]),
                          zx(_fv[8]), zy(_fv[9]), zz(_fv[10]), zw(_fv[11]),
                          tx(_fv[12]), ty(_fv[13]), tz(_fv[14]), tw(_fv[15]) {}

    Matrix4(float _xx, float _xy, float _xz, float _xw,
            float _yx, float _yy, float _yz, float _yw,
            float _zx, float _zy, float _zz, float _zw,
            float _tx, float _ty, float _tz, float _tw)
        : xx(_xx), xy(_xy), xz(_xz), xw(_xw),
          yx(_yx), yy(_yy), yz(_yz), yw(_yw),
          zx(_zx), zy(_zy), zz(_zz), zw(_zw),
          tx(_tx), ty(_ty), tz(_tz), tw(_tw) {}
};

Matrix4 operator+(const Matrix4 &_a, const Matrix4 &_b);
Matrix4 operator-(const Matrix4 &_a, const Matrix4 &_b);
Matrix4 operator*(const Matrix4 &_m, float _s);
Matrix4 operator*(const Matrix4 &_a, const Matrix4 &_b);
Vector4 operator*(const Matrix4 &_m, const Vector4 &_v);

Vector3 transformPoint(const Matrix4 &_m, const Vector3 &_v);
Vector3 transformVector(const Matrix4 &_m, const Vector3 &_v);

Matrix4 transposed(const Matrix4 &_m);
float determinant(const Matrix4 &_m);
Matrix4 adjugate(const Matrix4 &_m);
Matrix4 inverse(const Matrix4 &m);
#endif //_MATRIX4_H_