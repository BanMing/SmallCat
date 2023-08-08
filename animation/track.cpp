#include "track.h"

template <typename T>
T Track<T>::sampleStep(float _time, bool _isLooping) const
{
    size_t frameIndex = getFrameIndex(_time, _isLooping);
    if (frameIndex < 0 || frameIndex >= getFrameSize())
    {
        return T();
    }

    return m_keyframes[frameIndex].m_value;
}

template <typename T>
T Track<T>::sampleLinear(float _time, bool _isLooping) const
{
    size_t curFrameIndex = getFrameIndex(_time, _isLooping);
    if (curFrameIndex < 0 || curFrameIndex >= getFrameSize())
    {
        return T();
    }

    size_t nextFrameIndex = curFrameIndex + 1;

    float frameDeltaTime = m_keyframes[nextFrameIndex].m_time - m_keyframes[curFrameIndex].m_time;
    if (frameDeltaTime <= 0.0f)
    {
        return m_keyframes[curFrameIndex].m_value;
    }

    float curTrackTime = clampTime(_time, _isLooping);
    float t = (curTrackTime - m_keyframes[curFrameIndex].m_time) / frameDeltaTime;

    return lerp(m_keyframes[curFrameIndex].m_value, m_keyframes[nextFrameIndex].m_value, t);
}

template <typename T>
T Track<T>::sampleCubic(float _time, bool _isLooping) const
{
    size_t curFrameIndex = getFrameIndex(_time, _isLooping);
    if (curFrameIndex < 0 || curFrameIndex >= getFrameSize())
    {
        return T();
    }

    size_t nextFrameIndex = curFrameIndex + 1;

    float frameDeltaTime = m_keyframes[nextFrameIndex].m_time - m_keyframes[curFrameIndex].m_time;
    if (frameDeltaTime <= 0.0f)
    {
        return m_keyframes[curFrameIndex].m_value;
    }

    const CubicKeyframe<T> &curFrame = static_cast<const CubicKeyframe<T> &>(m_keyframes[curFrameIndex]);
    const CubicKeyframe<T> &nextFrame = static_cast<const CubicKeyframe<T> &>(m_keyframes[nextFrameIndex]);

    float curTrackTime = clampTime(_time, _isLooping);
    float t = (curTrackTime - curFrame.m_time) / frameDeltaTime;

    T point1 = curFrame.m_value;
    T slope1 = curFrame.m_outTangent;
    slope1 = slope1 * frameDeltaTime;

    T point2 = nextFrame.m_value;
    T slope2 = nextFrame.m_outTangent;
    slope2 = slope2 * frameDeltaTime;

    return hermite(point1, slope1, point2, slope2, t);
}

template <typename T>
size_t Track<T>::getFrameIndex(float _time, bool _isLooping) const
{
    size_t frameSize = getFrameSize();

    // Must have two frames to interpolate
    if (frameSize <= 1)
    {
        return -1;
    }

    if (_isLooping)
    {
        float startTime = m_keyframes[0].m_time;
        float endTime = m_keyframes[frameSize - 1].m_time;
        float duration = endTime - startTime;
        duration = duration < 0.0f ? fabsf(duration) : duration;

        _time = fmodf(_time - startTime, duration);
        if (_time < 0.0f)
        {
            _time += duration;
        }
        _time += startTime;
    }
    else
    {
        if (_time <= m_keyframes[0].m_time)
        {
            return 0;
        }

        if (_time >= m_keyframes[frameSize - 2].m_time)
        {
            return frameSize - 2;
        }
    }

    for (size_t i = frameSize - 1; i >= 0; i--)
    {
        if (_time >= m_keyframes[i].m_time)
        {
            return i;
        }
    }

    return -1;
}

template <typename T>
float Track<T>::clampTime(float _time, bool _isLooping) const
{
    size_t frameSize = getFrameSize();
    if (frameSize <= 1)
    {
        return 0.0f;
    }

    float startTime = m_keyframes[0].m_time;
    float endTime = m_keyframes[frameSize - 1].m_time;
    float duration = endTime - startTime;
    if (duration <= 0.0f)
    {
        return 0.0f;
    }

    if (_isLooping)
    {
        _time = fmodf(_time - startTime, duration);
        if (_time < 0.0f)
        {
            _time += duration;
        }
        _time += startTime;
    }
    else
    {
        if (_time <= m_keyframes[0].m_time)
        {
            _time = startTime;
        }
        if (_time > m_keyframes[frameSize - 1].m_time)
        {
            _time = endTime;
        }
    }

    return _time;
}

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
size_t Track<T>::getFrameSize() const
{
    return m_keyframes.size();
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
T Track<T>::sample(float _time, bool _isLooping) const
{
    switch (m_interpolation)
    {
    case InterpolationType::Step:
        return sampleStep(_time, _isLooping);
    case InterpolationType::Linear:
        return sampleLinear(_time, _isLooping);
    case InterpolationType::Cubic:
        return sampleCubic(_time, _isLooping);
    default:
        return sampleLinear(_time, _isLooping);
    }
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