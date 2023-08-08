#ifndef _TRACK_H_
#define _TRACK_H_

#include "interpolation.h"
#include "keyframe.h"
#include <vector>

template <typename T>
class Track
{
protected:
    InterpolationType m_interpolation = InterpolationType::Linear;
    std::vector<Keyframe<T>> m_keyframes;

protected:
    T sampleStep(float _time, bool _isLooping) const;
    T sampleLinear(float _time, bool _isLooping) const;
    T sampleCubic(float _time, bool _isLooping) const;
    size_t getFrameIndex(float _time, bool _isLooping) const;
    float clampTime(float _time, bool _isLooping) const;

public:
    Track();
    void setInterpolation(InterpolationType _interpolation);
    void resize(size_t _size);
    size_t getFrameSize() const;

    InterpolationType getInterpolation() const;
    Keyframe<T> &operator[](size_t _index);
    T sample(float _time, bool _isLooping) const;

    float getStartTime() const;
    float getEndTime() const;
};

typedef Track<Vector3> VectorTrack;
typedef Track<Quaternion> QuaternionTrack;

#endif //_TRACK_H_