#include <bx/bx.h>
#include <bx/file.h>
#include <bx/sort.h>
#include <bgfx/bgfx.h>

#include <iostream>

#include "Samples/Hello/HelloSample.h"
#include "Platform/WindowHandle.h"

// namespace Tiga
// {
Tiga::Application *gApplication = 0;
float deltaTime = 0.333f; // test code
char gRestartArgs[1024] = {'\0'};

// App entry
int main(int, char **)
{
    std::cout << "Hello, world!\n";
    Tiga::WindowHandle defaultWindow = {0};
    gApplication = new Tiga::HelloSample();

#pragma region Initialize
    bgfx::Init init;
    init.type = bgfx::RendererType::Count;
    init.vendorId = BGFX_PCI_ID_NONE;
    init.resolution.width = 800;
    init.resolution.height = 600;
    init.resolution.reset = BGFX_RESET_VSYNC;
    bgfx::init(init);

    // Enable debug text.
    bgfx::setDebug(BGFX_DEBUG_TEXT);

    // Set view 0 clear state.
    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff, 1.0f, 0);

    gApplication->Initialize();
#pragma endregion Initialize
    bgfx::frame();

    while (true)
    {

#pragma region Events
        if (0 != bx::strLen(gRestartArgs))
        {
            gApplication->Shutdown();
            // Shutdown bgfx.
            bgfx::shutdown();
            gApplication = 0;
            break;
        }
#pragma endregion Events

#pragma region Update
        gApplication->Update(deltaTime);
#pragma endregion Update

#pragma region Render
        gApplication->Render(deltaTime);

        // Set view 0 default viewport.
        bgfx::setViewRect(0, 0, 0, 800, 600);
        // This dummy draw call is here to make sure that view 0 is cleared
        // if no other draw calls are submitted to view 0.
        bgfx::touch(0);
        // Advance to next frame. Rendering thread will be kicked to
        // process submitted rendering primitives.
        bgfx::frame();
#pragma endregion Render

#pragma region IMGUI Update
        gApplication->OnGUI();
#pragma endregion IMGUI Update
    }

    if (gApplication != 0)
    {
        delete gApplication;
    }

    return 0;
}
// } // namespace Tiga
