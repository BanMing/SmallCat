#ifndef _INTERPOLATION_H_
#define _INTERPOLATION_H_

#include "../math/Vector3.h"
#include "../math/quaternion.h"
// Vector3 Interpolation
Vector3 lerp(const Vector3 &_from, const Vector3 &_to, float _t);
Vector3 nlerp(const Vector3 &_from, const Vector3 &_to, float _t);
Vector3 slerp(const Vector3 &_from, const Vector3 &_to, float _t);

// Quaternion Interpolation
Quaternion lerp(const Quaternion &_from, const Quaternion &_to, float _t);
Quaternion nlerp(const Quaternion &_from, const Quaternion &_to, float _t);
Quaternion slerp(const Quaternion &_from, const Quaternion &_to, float _t);

// Cubic Interpolation
Vector3 hermite(const Vector3 &_p1, const Vector3 &_m1, const Vector3 &_p2, const Vector3 &_m2, float _t);
Quaternion hermite(const Quaternion &_p1, const Quaternion &_m1, const Quaternion &_p2, const Quaternion &_m2, float _t);
#endif //_INTERPOLATION_H_