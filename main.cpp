#include <iostream>
#include "examples/helloworld/helloworld.h"

int _main_(int _argc, char **_argv)
{
    HelloWorld *app = new HelloWorld("Hello World", "Initialization and debug text.", "https://bkaradzic.github.io/bgfx/examples.html#helloworld");
    return entry::runApp(app, _argc, _argv);
}