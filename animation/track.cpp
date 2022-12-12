#include "track.h"
#include <cmath>

// template <typename ValueType>
// Track<ValueType>::Track()
// {
//     m_interpolation = Interpolation::Linear;
//     m_startTime = 0;
//     m_endTime = 0;
// }

// template <typename ValueType>
// Track<ValueType>::~Track()
// {
//     m_frames.clear();
//     m_frames.shrink_to_fit();
// }

// template <typename ValueType>
// void Track<ValueType>::setInterpolation(Interpolation _interpolation)
// {
//     m_interpolation = _interpolation;
// }

// template <typename ValueType>
// Interpolation Track<ValueType>::getInterpolation() const
// {
//     return m_interpolation;
// }

// template <typename ValueType>
// size_t Track<ValueType>::size() const
// {
//     return m_frames.size();
// }

// template <typename ValueType>
// void Track<ValueType>::reSize(size_t _newSize)
// {
//     m_frames.resize(_newSize);
// }

// template <typename ValueType>
// void Track<ValueType>::refreshFrames(void* compareAction)
// {
//     std::sort(m_frames.begin(), m_frames.end(),compareAction);
//     // m_startTime = m_frames[0].
// }

// template <typename ValueType>
// float Track<ValueType>::clampTime(float _inTime, bool isLooping) const
// {
//     float duration = m_endTime - m_startTime;
//     if (isLooping && _inTime > duration)
//     {
//         return fmod(_inTime, duration);
//     }
//     else
//     {
//         if (_inTime < m_startTime)
//         {
//             return m_startTime;
//         }
//         else if (_inTime > m_endTime)
//         {
//             return m_endTime;
//         }
//     }
//     return _inTime;
// }

// template <typename ValueType>
// int Track<ValueType>::getFrameIndex(float _inTime)
// {
//     float inTime = clampTime(_inTime);
//     for (size_t i = 0; i < m_frames.size(); i++)
//     {
//     }
// }
// // ValueType &operator[](size_t index);

// // void Sample(float _inTime, ValueType &_outValue);