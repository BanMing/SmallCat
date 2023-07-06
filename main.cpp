#include "examples/pose/pose_sample.h"

int _main_(int _argc, char **_argv)
{
    PoseSample *app = new PoseSample("Hello World", "Hello World Demo.");
    return entry::runApp(app, _argc, _argv);
}