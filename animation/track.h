#ifndef _TRACK_H_
#define _TRACK_H_

#include "keyframe.h"
#include "interpolation.h"
#include <vector>

// class TimeStamp
// {
// public:
//     float m_time;
// };

// template <typename ValueType>
// class Track
// {
// protected:
//     std::vector<ValueType> m_frames;
//     Interpolation m_interpolation;
//     float m_startTime;
//     float m_endTime;

// protected:
//     float clampTime(float _inTime, bool isLooping) const;

// public:
//     Track();

//     ~Track();

//     void setInterpolation(Interpolation _interpolation);
//     Interpolation getInterpolation() const;

//     size_t size() const;
//     void reSize(size_t _newSize);

//     void refreshFrames(void *compareAction);

//     int getFrameIndex(float _inTime);
//     ValueType &operator[](size_t index);

//     void Sample(float _inTime, ValueType &_outValue);
// };

// typedef Track<float> ScalarTrack;
// typedef Track<Vector3> VectorTrack;

template <typename T>
struct Track
{
    InterpolationType interpolation = InterpolationType::Linear;
    bool m_isLooping = true;
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

struct TransfromTrack
{
    VectorTrack m_positionTrack;
    QuaternionTrack m_rotationTrack;
    VectorTrack m_scaleTrack;
};

#endif //_TRACK_H_