#include <iostream>

#include "Samples/Hello/HelloSample.h"
#include "Platform/MainApp.h"

int main(int argc, const char **argv)
{
    Tiga::Application *application = new Tiga::HelloSample();
    return Tiga::Run(application, argc, argv);
}
