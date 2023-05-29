#include "track.h"

template <typename T>
Track<T>::Track()
{
    m_interpolation = InterpolationType::Linear;
}

template <typename T>
void Track<T>::setInterpolation(InterpolationType _interpolation)
{
    m_interpolation = _interpolation;
}

template <typename T>
void Track<T>::resize(size_t _size)
{
    m_keyframes.resize(_size);
}

template <typename T>
InterpolationType Track<T>::getInterpolation() const
{
    return m_interpolation;
}

template <typename T>
Keyframe<T> &Track<T>::operator[](size_t _index)
{
    return m_keyframes[_index];
}

template <typename T>
float Track<T>::getStartTime() const
{
    return m_keyframes[0].m_time;
}

template <typename T>
float Track<T>::getEndTime() const
{
    return m_keyframes[m_keyframes.size() - 1].m_time;
}

// add template difine
template class Track<Quaternion>;
template class Track<Vector3>;