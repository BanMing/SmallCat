#ifndef _VECTOR2_H_
#define _VECTOR2_H_

template <typename T>
struct TVector2
{
    union
    {
        T v[2];

        struct
        {
            T x, y;
        };
    };

    inline Vector2() : x(T(0)), y(T(0)) {}
    inline Vector2(T _x, T _y) : x(_x), y(_y) {}
    inline Vector2(T *_fv) : x(_fv[0]), y(_fv[1]) {}
};

typedef TVector2<float> Vector2;
typedef TVector2<int> IVector2;

#endif //_VECTOR2_H_