#include "matrix4.h"

Matrix4 operator+(const Matrix4 &_a, const Matrix4 &_b)
{
    return Matrix4(_a.xx + _b.xx, _a.xy + _b.xy, _a.xz + _b.xz, _a.xw + _b.xw,
                   _a.yx + _b.yx, _a.yy + _b.yy, _a.yz + _b.yz, _a.yw + _b.yw,
                   _a.zx + _b.zx, _a.zy + _b.zy, _a.zz + _b.zz, _a.zw + _b.zw,
                   _a.tx + _b.tx, _a.ty + _b.ty, _a.tz + _b.tz, _a.tw + _b.tw);
}

Matrix4 operator-(const Matrix4 &_a, const Matrix4 &_b)
{
    return Matrix4(_a.xx - _b.xx, _a.xy - _b.xy, _a.xz - _b.xz, _a.xw - _b.xw,
                   _a.yx - _b.yx, _a.yy - _b.yy, _a.yz - _b.yz, _a.yw - _b.yw,
                   _a.zx - _b.zx, _a.zy - _b.zy, _a.zz - _b.zz, _a.zw - _b.zw,
                   _a.tx - _b.tx, _a.ty - _b.ty, _a.tz - _b.tz, _a.tw - _b.tw);
}

Matrix4 operator*(const Matrix4 &_m, float _s)
{
    return Matrix4(_m.xx * _s, _m.xy * _s, _m.xz * _s, _m.xw * _s,
                   _m.yx * _s, _m.yy * _s, _m.yz * _s, _m.yw * _s,
                   _m.zx * _s, _m.zy * _s, _m.zz * _s, _m.zw * _s,
                   _m.tx * _s, _m.ty * _s, _m.tz * _s, _m.tw * _s);
}

inline float MatrixRowDotColumn(const Matrix4 &_a, const Matrix4 &_b,
                                int _aRow, int _bCol)
{
    return _a.m[0 * 4 + _aRow] * _b.m[_bCol * 4 + 0] +
           _a.m[1 * 4 + _aRow] * _b.m[_bCol * 4 + 1] +
           _a.m[2 * 4 + _aRow] * _b.m[_bCol * 4 + 2] +
           _a.m[3 * 4 + _aRow] * _b.m[_bCol * 4 + 3];
}

Matrix4 operator*(const Matrix4 &_a, const Matrix4 &_b)
{
    Matrix4 res;

    res.cloumn[0].x = _a.m[0] * _b.m[0] + _a.m[4] * _b.m[1] + _a.m[8] * _b.m[2] + _a.m[12] * _b.m[3];
    res.cloumn[0].y = _a.m[1] * _b.m[0] + _a.m[5] * _b.m[1] + _a.m[9] * _b.m[2] + _a.m[13] * _b.m[3];
    res.cloumn[0].z = _a.m[2] * _b.m[0] + _a.m[6] * _b.m[1] + _a.m[10] * _b.m[2] + _a.m[14] * _b.m[3];
    res.cloumn[0].w = _a.m[3] * _b.m[0] + _a.m[7] * _b.m[1] + _a.m[11] * _b.m[2] + _a.m[15] * _b.m[3];

    res.cloumn[1].x = _a.m[0] * _b.m[4] + _a.m[4] * _b.m[5] + _a.m[8] * _b.m[6] + _a.m[12] * _b.m[7];
    res.cloumn[1].y = _a.m[1] * _b.m[4] + _a.m[5] * _b.m[5] + _a.m[9] * _b.m[6] + _a.m[13] * _b.m[7];
    res.cloumn[1].z = _a.m[2] * _b.m[4] + _a.m[6] * _b.m[5] + _a.m[10] * _b.m[6] + _a.m[14] * _b.m[7];
    res.cloumn[1].w = _a.m[3] * _b.m[4] + _a.m[7] * _b.m[5] + _a.m[11] * _b.m[6] + _a.m[15] * _b.m[7];

    res.cloumn[2].x = _a.m[0] * _b.m[8] + _a.m[4] * _b.m[9] + _a.m[8] * _b.m[10] + _a.m[12] * _b.m[11];
    res.cloumn[2].y = _a.m[1] * _b.m[8] + _a.m[5] * _b.m[9] + _a.m[9] * _b.m[10] + _a.m[13] * _b.m[11];
    res.cloumn[2].z = _a.m[2] * _b.m[8] + _a.m[6] * _b.m[9] + _a.m[10] * _b.m[10] + _a.m[14] * _b.m[11];
    res.cloumn[2].w = _a.m[3] * _b.m[8] + _a.m[7] * _b.m[9] + _a.m[11] * _b.m[10] + _a.m[15] * _b.m[11];

    res.cloumn[3].x = _a.m[0] * _b.m[12] + _a.m[4] * _b.m[13] + _a.m[8] * _b.m[14] + _a.m[12] * _b.m[15];
    res.cloumn[3].y = _a.m[1] * _b.m[12] + _a.m[5] * _b.m[13] + _a.m[9] * _b.m[14] + _a.m[13] * _b.m[15];
    res.cloumn[3].z = _a.m[2] * _b.m[12] + _a.m[6] * _b.m[13] + _a.m[10] * _b.m[14] + _a.m[14] * _b.m[15];
    res.cloumn[3].w = _a.m[3] * _b.m[12] + _a.m[7] * _b.m[13] + _a.m[11] * _b.m[14] + _a.m[15] * _b.m[15];

    return res;
}

Vector4 operator*(const Matrix4 &_m, const Vector4 &_v)
{
    Vector4 res;

    res.x = _v.v[0] * _m.m[0] + _v.v[1] * _m.m[4] + _v.v[2] * _m.m[8] + _v.v[3] * _m.m[12];
    res.y = _v.v[0] * _m.m[1] + _v.v[1] * _m.m[5] + _v.v[2] * _m.m[9] + _v.v[3] * _m.m[13];
    res.z = _v.v[0] * _m.m[2] + _v.v[1] * _m.m[6] + _v.v[2] * _m.m[10] + _v.v[3] * _m.m[14];
    res.w = _v.v[0] * _m.m[3] + _v.v[1] * _m.m[7] + _v.v[2] * _m.m[11] + _v.v[3] * _m.m[15];

    return res;
}

Vector3 transformPoint(const Matrix4 &_m, const Vector3 &_v)
{
    Vector4 multiplication = _m * Vector4(_v.x, _v.y, _v.z, 1.0f);
    return Vector3(multiplication.x, multiplication.y, multiplication.z);
}

Vector3 transformVector(const Matrix4 &_m, const Vector3 &_v)
{
    Vector4 multiplication = _m * Vector4(_v.x, _v.y, _v.z, 0);
    return Vector3(multiplication.x, multiplication.y, multiplication.z);
}

Matrix4 transposed(const Matrix4 &_m)
{
    return Matrix4(
        _m.cloumn[0].x, _m.cloumn[1].x, _m.cloumn[2].x, _m.cloumn[3].x,
        _m.cloumn[0].y, _m.cloumn[1].y, _m.cloumn[2].y, _m.cloumn[3].y,
        _m.cloumn[0].z, _m.cloumn[1].z, _m.cloumn[2].z, _m.cloumn[3].z,
        _m.cloumn[0].w, _m.cloumn[1].w, _m.cloumn[2].w, _m.cloumn[3].w);
}

inline float get4x4Minor(const Matrix4 &_m, int _c0, int _c1, int _c2, int _r0, int _r1, int _r2)
{
    return _m.m[_c0 * 4 + _r0] * (_m.m[_c1 * 4 + _r1] * _m.m[_c2 * 4 + _r2] - _m.m[_c1 * 4 + _r2] * _m.m[_c2 * 4 + _r1]) -
           _m.m[_c1 * 4 + _r0] * (_m.m[_c0 * 4 + _r1] * _m.m[_c2 * 4 + _r2] - _m.m[_c0 * 4 + _r2] * _m.m[_c2 * 4 + _r1]) +
           _m.m[_c2 * 4 + _r0] * (_m.m[_c0 * 4 + _r1] * _m.m[_c1 * 4 + _r2] - _m.m[_c0 * 4 + _r2] * _m.m[_c1 * 4 + _r1]);
}

float determinant(const Matrix4 &_m)
{
    return _m.cloumn[0].x * get4x4Minor(_m, 1, 2, 3, 1, 2, 3) -
           _m.cloumn[1].x * get4x4Minor(_m, 0, 2, 3, 1, 2, 3) +
           _m.cloumn[2].x * get4x4Minor(_m, 0, 1, 3, 1, 2, 3) -
           _m.cloumn[3].x * get4x4Minor(_m, 0, 1, 2, 1, 2, 3);
}

Matrix4 adjugate(const Matrix4 &_m)
{
    Matrix4 cofactor;

    cofactor.cloumn[0].x = get4x4Minor(_m, 1, 2, 3, 1, 2, 3);
    cofactor.cloumn[0].y = -get4x4Minor(_m, 1, 2, 3, 0, 2, 3);
    cofactor.cloumn[0].z = get4x4Minor(_m, 1, 2, 3, 0, 1, 3);
    cofactor.cloumn[0].w = -get4x4Minor(_m, 1, 2, 3, 0, 1, 2);

    cofactor.cloumn[1].x = -get4x4Minor(_m, 0, 2, 3, 1, 2, 3);
    cofactor.cloumn[1].y = get4x4Minor(_m, 0, 2, 3, 0, 2, 3);
    cofactor.cloumn[1].z = -get4x4Minor(_m, 0, 2, 3, 0, 1, 3);
    cofactor.cloumn[1].w = get4x4Minor(_m, 0, 2, 3, 0, 1, 2);

    cofactor.cloumn[2].x = get4x4Minor(_m, 0, 1, 3, 1, 2, 3);
    cofactor.cloumn[2].y = -get4x4Minor(_m, 0, 1, 3, 0, 2, 3);
    cofactor.cloumn[2].z = get4x4Minor(_m, 0, 1, 3, 0, 1, 3);
    cofactor.cloumn[2].w = -get4x4Minor(_m, 0, 1, 3, 0, 1, 2);

    cofactor.cloumn[3].x = -get4x4Minor(_m, 0, 1, 2, 1, 2, 3);
    cofactor.cloumn[3].y = get4x4Minor(_m, 0, 1, 2, 0, 2, 3);
    cofactor.cloumn[3].z = -get4x4Minor(_m, 0, 1, 2, 0, 1, 3);
    cofactor.cloumn[3].w = get4x4Minor(_m, 0, 1, 2, 0, 1, 2);

    return transposed(cofactor);
}

Matrix4 inverse(const Matrix4& m)
{
	float det = determinant(m);

	if (det == 0.0f)
	{ 
		return Matrix4();
	}
	Matrix4 adj = adjugate(m);

	return adj * (1.0f / det);
}