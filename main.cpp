#include "examples/animationclip/animation_clip_sample.h"

int _main_(int _argc, char **_argv)
{
    AnimationClipSample *app = new AnimationClipSample("Hello World", "Hello World Demo.");
    return entry::runApp(app, _argc, _argv);
}