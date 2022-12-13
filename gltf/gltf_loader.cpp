#define CGLTF_IMPLEMENTATION
#include "gltf_loader.h"
#include "../animation/keyframe.h"
#include <iostream>

size_t getNodeIndex(cgltf_node *_node, cgltf_node *_allNodes, size_t _nodesNum)
{
    if (_node == 0)
    {
        return -1;
    }

    for (size_t i = 0; i < _nodesNum; i++)
    {
        if (_node == &_allNodes[i])
        {
            return i;
        }
    }
    return -1;
}

void getFloatValues(std::vector<float> &_outFloats, size_t _inComponentCount, const cgltf_accessor &_inAccessor)
{
    _outFloats.resize(_inAccessor.count * _inComponentCount);
    for (cgltf_size i = 0; i < _inAccessor.count; ++i)
    {
        cgltf_accessor_read_float(&_inAccessor, i, &_outFloats[i * _inComponentCount], _inComponentCount);
    }
}

template <typename T>
void setKeyframeValueFromSampler(Keyframe<T> *_keyframe, bool _isSampleCubic, size_t _curFrameNum, size_t _componentCount, const std::vector<float> &_floatValues)
{
    int baseIndex = _curFrameNum * _componentCount * (_isSampleCubic ? 3 : 1);
    int offset = 0;
    float temp[_componentCount];

    // set current keyframe in tangent
    if (_isSampleCubic)
    {
        for (size_t j = 0; j < _componentCount; j++)
        {
            temp[j] = _floatValues[baseIndex + offset++];
        }
        CubicKeyframe<T> *keyframe = static_cast<CubicKeyframe<T> *>(_keyframe);
        memcpy(&keyframe->m_inTangent, &temp, sizeof(float) * _componentCount);
    }

    // set current keyframe value
    for (size_t j = 0; j < _componentCount; j++)
    {
        temp[j] = _floatValues[baseIndex + offset++];
    }
    memcpy(&_keyframe->m_value, &temp, sizeof(float) * _componentCount);

    // set current keyframe out tangent
    if (_isSampleCubic)
    {
        for (size_t j = 0; j < _componentCount; j++)
        {
            temp[j] = _floatValues[baseIndex + offset++];
        }
        CubicKeyframe<T> *keyframe = static_cast<CubicKeyframe<T> *>(_keyframe);
        memcpy(&keyframe->m_outTangent, &temp, sizeof(float) * _componentCount);
    }
}

template <typename T>
void setTrackFromChannel(Track<T> &_track, const cgltf_animation_channel &_channel, size_t _componentCount)
{
    cgltf_animation_sampler &sampler = *_channel.sampler;
    bool isSampleCubic = sampler.interpolation == cgltf_interpolation_type_cubic_spline;

    switch (sampler.interpolation)
    {
    case cgltf_interpolation_type_step:
        _track.interpolation = InterpolationType::Step;
        break;
    case cgltf_interpolation_type_cubic_spline:
        _track.interpolation = InterpolationType::Cubic;
        break;
    default:
        _track.interpolation = InterpolationType::Linear;
        break;
    }

    std::vector<float> timelineFloats;
    getFloatValues(timelineFloats, 1, *sampler.input);

    std::vector<float> floatValues;
    getFloatValues(floatValues, _componentCount, *sampler.output);

    size_t framesNum = sampler.input->count;
    _track.m_keyframes.resize(framesNum);

    // Copy data to track keyframe
    for (size_t i = 0; i < framesNum; ++i)
    {
        _track.m_keyframes[i] = isSampleCubic ? new CubicKeyframe<T>() : new Keyframe<T>();
        _track.m_keyframes[i]->m_time = timelineFloats[i];
        setKeyframeValueFromSampler(_track.m_keyframes[i], isSampleCubic, i, _componentCount, floatValues);
    }
}

cgltf_data *loadGLTFFile(const char *_path)
{
    cgltf_options options = {};
    cgltf_data *data = NULL;

    cgltf_result result = cgltf_parse_file(&options, _path, &data);
    if (result != cgltf_result_success)
    {
        std::cout << "Could not load input file: " << _path << "\n";
        return 0;
    }

    result = cgltf_load_buffers(&options, data, _path);
    if (result != cgltf_result_success)
    {
        cgltf_free(data);
        std::cout << "Could not load buffers for: " << _path << "\n";
        return 0;
    }

    result = cgltf_validate(data);
    if (result != cgltf_result_success)
    {
        cgltf_free(data);
        std::cout << "Invalid gltf file: " << _path << "\n";
        return 0;
    }

    return data;
}

void freeGLTFFile(cgltf_data *_data)
{
    if (_data == 0)
    {
        std::cout << "WARNING: Can't free null data\n";
    }
    else
    {
        cgltf_free(_data);
    }
}

std::vector<AnimationClip> loadAnimationClips(cgltf_data *_data)
{
    size_t clipsNum = _data->animations_count;
    size_t nodesNum = _data->nodes_count;

    std::vector<AnimationClip> res;
    res.resize(clipsNum);

    for (size_t i = 0; i < clipsNum; i++)
    {
        res[i].setName(_data->animations[i].name);

        size_t channelsNum = _data->animations[i].channels_count;

        for (size_t j = 0; j < channelsNum; j++)
        {
            cgltf_animation_channel &channel = _data->animations[i].channels[j];
            cgltf_node *node = channel.target_node;
            size_t nodeId = getNodeIndex(node, _data->nodes, _data->nodes_count);

            AnimatedJoint *animatedJoint = res[i][nodeId];
            animatedJoint->m_jointID = nodeId;
            animatedJoint->m_jointName = node->name;

            if (channel.target_path == cgltf_animation_path_type_translation)
            {
                setTrackFromChannel(animatedJoint->m_positionTrack, channel, 3);
            }
            else if (channel.target_path == cgltf_animation_path_type_scale)
            {
                setTrackFromChannel(animatedJoint->m_scaleTrack, channel, 3);
            }
            else if (channel.target_path == cgltf_animation_path_type_rotation)
            {
                setTrackFromChannel(animatedJoint->m_rotationTrack, channel, 4);
            }
        }

        res[i].updateDuration();
    }

    return res;
}