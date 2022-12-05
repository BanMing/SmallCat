#ifndef _KEYFRAME_H_
#define _KEYFRAME_H_
#include "../math/transform.h"

class Keyframe
{
public:
    Transform m_joint;
    float m_time;
};

#endif //_KEYFRAME_H_