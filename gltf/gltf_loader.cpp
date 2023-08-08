#define CGLTF_IMPLEMENTATION
#include "gltf_loader.h"
#include "../animation/keyframe.h"
#include <iostream>

Transform getNodeTransform(cgltf_node *_node)
{
    Transform reslut;

    if (_node->has_matrix)
    {
        reslut = mat4ToTransform(_node->matrix);
    }

    if (_node->has_translation)
    {
        reslut.position.x = _node->translation[0];
        reslut.position.y = _node->translation[1];
        reslut.position.z = _node->translation[2];
    }

    if (_node->has_scale)
    {
        reslut.scale.x = _node->scale[0];
        reslut.scale.y = _node->scale[1];
        reslut.scale.z = _node->scale[2];
    }

    if (_node->has_rotation)
    {
        reslut.rotation.x = _node->rotation[0];
        reslut.rotation.y = _node->rotation[1];
        reslut.rotation.z = _node->rotation[2];
        reslut.rotation.w = _node->rotation[3];
    }

    return reslut;
}

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
void setKeyframeValueFromSampler(Keyframe<T> &_keyframe, bool _isSampleCubic, size_t _curFrameNum, size_t _componentCount, const std::vector<float> &_floatValues)
{
    size_t baseIndex = _curFrameNum * _componentCount * (_isSampleCubic ? 3 : 1);
    size_t offset = 0;
    std::vector<float> temp;
    temp.resize(_componentCount);

    // set current keyframe in tangent
    if (_isSampleCubic)
    {
        for (size_t j = 0; j < _componentCount; j++)
        {
            temp[j] = _floatValues[baseIndex + offset++];
        }
        CubicKeyframe<T> &keyframe = static_cast<CubicKeyframe<T> &>(_keyframe);
        memcpy(&keyframe.m_inTangent, &temp[0], sizeof(float) * _componentCount);
    }

    // set current keyframe value
    for (size_t j = 0; j < _componentCount; j++)
    {
        temp[j] = _floatValues[baseIndex + offset++];
    }
    memcpy(&_keyframe.m_value, &temp[0], sizeof(float) * _componentCount);

    // set current keyframe out tangent
    if (_isSampleCubic)
    {
        for (size_t j = 0; j < _componentCount; j++)
        {
            temp[j] = _floatValues[baseIndex + offset++];
        }
        CubicKeyframe<T> &keyframe = static_cast<CubicKeyframe<T> &>(_keyframe);
        memcpy(&keyframe.m_outTangent, &temp[0], sizeof(float) * _componentCount);
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
        _track.setInterpolation(InterpolationType::Step);
        break;
    case cgltf_interpolation_type_cubic_spline:
        _track.setInterpolation(InterpolationType::Cubic);
        break;
    default:
        _track.setInterpolation(InterpolationType::Linear);
        break;
    }

    std::vector<float> timelineFloats;
    getFloatValues(timelineFloats, 1, *sampler.input);

    std::vector<float> floatValues;
    getFloatValues(floatValues, _componentCount, *sampler.output);

    size_t framesNum = sampler.input->count;
    _track.resize(framesNum);

    // Copy data to track keyframe
    for (size_t i = 0; i < framesNum; ++i)
    {
        Keyframe<T> &keyframe = _track[i];
        keyframe.m_time = timelineFloats[i];
        setKeyframeValueFromSampler(keyframe, isSampleCubic, i, _componentCount, floatValues);
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

            JointTrack &jointTrack = res[i][nodeId];
            jointTrack.setJointID(nodeId);
            jointTrack.setJointName(node->name);

            if (channel.target_path == cgltf_animation_path_type_translation)
            {
                setTrackFromChannel<Vector3>(jointTrack.getPositionTrack(), channel, 3);
            }
            else if (channel.target_path == cgltf_animation_path_type_scale)
            {
                setTrackFromChannel<Vector3>(jointTrack.getScaleTrack(), channel, 3);
            }
            else if (channel.target_path == cgltf_animation_path_type_rotation)
            {
                setTrackFromChannel<Quaternion>(jointTrack.getRotationTrack(), channel, 4);
            }
        }

        res[i].updateDuration();
    }

    return res;
}

Pose loadRestPose(cgltf_data *_data)
{
    size_t jointCount = _data->nodes_count;
    Pose reslut(jointCount);

    for (size_t i = 0; i < jointCount; i++)
    {
        cgltf_node *joint = &(_data->nodes[i]);
        Transform localTransform = getNodeTransform(joint);
        reslut.setLocalTransfrom(i, localTransform);

        size_t parentIndex = getNodeIndex(joint->parent, _data->nodes, jointCount);
        reslut.setParent(i, parentIndex);
    }

    return reslut;
}

Pose loadBindPose(cgltf_data *_data)
{
    Pose restPose = loadRestPose(_data);
    size_t jointSize = restPose.getJointsSize();
    std::vector<Transform> worldBindJoints(jointSize);
    for (size_t i = 0; i < jointSize; i++)
    {
        worldBindJoints[i] = restPose.getGlobalTransform(i);
    }

    size_t skinsNum = _data->skins_count;
    for (size_t i = 0; i < skinsNum; i++)
    {
        cgltf_skin *skin = &(_data->skins[i]);
        std::vector<float> invBindFloats;
        getFloatValues(invBindFloats, 16, *skin->inverse_bind_matrices);

        for (size_t j = 0; j < skin->joints_count; j++)
        {
            // Read the ivnerse bind matrix of the joint
            float *matrix = &(invBindFloats[j * 16]);
            Matrix4 invBindMatrix = Matrix4(matrix);
            // invert, convert to transform
            Matrix4 bindMatrix = inverse(invBindMatrix);
            Transform bindTransform = mat4ToTransform(bindMatrix);
            // Set that transform in the worldBindJoints
            cgltf_node *jointNode = skin->joints[j];
            size_t jointIndex = getNodeIndex(jointNode, _data->nodes, jointSize);
            worldBindJoints[jointIndex] = bindTransform;
        }
    }

    // Convert the world bind pose to a regular bind pose
    Pose bindPose = restPose;
    for (size_t i = 0; i < jointSize; i++)
    {
        Transform current = worldBindJoints[i];
        int parentIndex = bindPose.getParent(i);
        if (parentIndex >= 0)
        {
            // Bring into parent space
            Transform parent = worldBindJoints[parentIndex];
            current = getTargetSpaceTransform(inverse(parent), current);
        }
        bindPose.setLocalTransfrom(i, current);
    }

    return bindPose;
}
