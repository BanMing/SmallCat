#include "HelloSample.h"
#include <iostream>

namespace Tiga
{
    void HelloSample::Initialize()
    {
        std::cout << "Hello, Initialize!\n";
    }

    void HelloSample::Update(float deltaTime) {}
    void HelloSample::Render(float inAspectRatio) {}
    void HelloSample::OnGUI() {}
    void HelloSample::Shutdown()
    {
        std::cout << "Hello, Shutdown!\n";
    }
}