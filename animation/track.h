#ifndef _TRACK_H_
#define _TRACK_H_

#include "keyframe.h"
#include "interpolation.h"
#include <vector>

template <typename T>
struct Track
{
    InterpolationType interpolation = InterpolationType::Linear;
    std::vector<Keyframe<T> *> m_keyframes;
    Track() {}
    ~Track()
    {
        for (size_t i = 0; i < m_keyframes.size(); i++)
        {
            delete m_keyframes[i];
        }
    }
};

typedef Track<Vector3> VectorTrack;
typedef Track<Quaternion> QuaternionTrack;

#endif //_TRACK_H_