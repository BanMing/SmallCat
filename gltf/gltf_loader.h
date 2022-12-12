#ifndef _GLTF_LOADER_H_
#define _GLTF_LOADER_H_

#include "../animation/animation_clip.h"
#include <vector>

// #define CGLTF_VALIDATE_ENABLE_ASSERTS BX_CONFIG_DEBUG

#include <cgltf/cgltf.h>

cgltf_data *loadGLTFFile(const char *_path);
void freeGLTFFile(cgltf_data *_data);

std::vector<AnimationClip> loadAnimationClips(cgltf_data *_data);

#endif //_GLTF_LOADER_H_