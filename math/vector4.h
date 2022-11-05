#ifndef _VECTOR4_H_
#define _VECTOR4_H_

template <typename T>
struct TVector4
{
    union
    {
        T v[4];

        struct
        {
            T x, y, z, w;
        };
    };

    TVector4() : x(T(0)), y(T(0)), z(T(0)), w(T(0)) {}
    TVector4(T _x, T _y, T _z, T _w) : x(_x), y(_y), z(_z), w(_w) {}
    TVector4(T *_fv) : x(_fv[0]), y(_fv[1]), z(_fv[2]), w(_fv[3]) {}
};

typedef TVector4<float> Vector4;
typedef TVector4<int> IVector4;

#endif //_VECTOR4_H_