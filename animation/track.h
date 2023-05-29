#ifndef _TRACK_H_
#define _TRACK_H_

#include "keyframe.h"
#include "interpolation.h"
#include <vector>

template <typename T>
class Track
{
protected:
    InterpolationType m_interpolation = InterpolationType::Linear;
    std::vector<Keyframe<T>> m_keyframes;

public:
    Track();
    void setInterpolation(InterpolationType _interpolation);
    void resize(size_t _size);

    InterpolationType getInterpolation() const;
    Keyframe<T> &operator[](size_t _index);

    float getStartTime() const;
    float getEndTime() const;
};

typedef Track<Vector3> VectorTrack;
typedef Track<Quaternion> QuaternionTrack;

#endif //_TRACK_H_