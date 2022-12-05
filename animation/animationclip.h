#ifndef _ANIMATIONCLIP_H_
#define _ANIMATIONCLIP_H_

#include "keyframe.h"
#include <vector>;

class AnimationClip
{
protected:
    std::vector<Keyframe> m_frames;
};

#endif //_ANIMATIONCLIP_H_