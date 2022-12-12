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
void setKeyframeFromChannel(Track<T> &_track, const cgltf_animation_channel &_channel, int _componentCount)
{
    cgltf_animation_sampler &sampler = *_channel.sampler;
    bool isSamplerCubic = sampler.interpolation == cgltf_interpolation_type_cubic_spline;

    std::vector<float> timelineFloats;
    getFloatValues(timelineFloats, 1, *sampler.input);

    std::vector<float> valueFloats;
    getFloatValues(valueFloats, _componentCount, *sampler.output);

    size_t framesNum = sampler.input->count;
    _track.m_keyframes.resize(framesNum);
    _track.m_isLooping = false;
    _track.interpolation = InterpolationType::Linear;
    for (size_t i = 0; i < framesNum; ++i)
    {
        int baseIndex = i * _componentCount;
        int offset = isSamplerCubic ? 1 : 0;
        _track.m_keyframes[i] = new Keyframe<T>();
        _track.m_keyframes[i]->m_time = timelineFloats[i];
        
        float test[_componentCount];
        for (size_t j = 0; j < _componentCount; j++)
        {
            test[j] = valueFloats[baseIndex + j];
        }
        memcpy(&_track.m_keyframes[i]->m_values, &test, sizeof(float) * _componentCount);
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
        res[i].m_transformTracks.resize(_data->nodes_count);
        for (size_t j = 0; j < channelsNum; j++)
        {
            cgltf_animation_channel &channel = _data->animations[i].channels[j];
            cgltf_node *node = channel.target_node;
            size_t nodeId = getNodeIndex(node, _data->nodes, _data->nodes_count);
            if (channel.target_path == cgltf_animation_path_type_translation)
            {
                setKeyframeFromChannel(res[i].m_transformTracks[nodeId].m_positionTrack, channel, 3);
            }
            else if (channel.target_path == cgltf_animation_path_type_scale)
            {
                setKeyframeFromChannel(res[i].m_transformTracks[nodeId].m_scaleTrack, channel, 3);
            }
            else
            if (channel.target_path == cgltf_animation_path_type_rotation)
            {
                setKeyframeFromChannel(res[i].m_transformTracks[nodeId].m_rotationTrack, channel, 4);
            }
        }

        res[i].updateDuration();
    }

    return res;
}