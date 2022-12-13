#ifndef _KEYFRAME_H_
#define _KEYFRAME_H_
#include "../math/transform.h"

template <typename T>
struct Keyframe
{
    T m_value;
    float m_time;
};

template <typename T>
struct CubicKeyframe : public Keyframe<T>
{
    T m_inTangent;
    T m_outTangent;
};

#endif //_KEYFRAME_H_