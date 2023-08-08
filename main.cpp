// #include "examples/pose/pose_sample.h"
// #include "examples/debugdraw/debug_draw_sample.h"
#include "examples/animationclip/animation_clip_sample.h"

int _main_(int _argc, char **_argv)
{
    AnimationClipSample *app = new AnimationClipSample("Animation Clip Sample", "Hello World Demo.");
    // PoseSample *app = new PoseSample("Pose Sample", "Hello World Demo.");
    // DebugDrawSample *app = new DebugDrawSample("Pose Sample", "Hello World Demo.");
    return entry::runApp(app, _argc, _argv);
}