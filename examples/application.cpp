#include "application.h"

void Application::init(int32_t _argc, const char *const *_argv, uint32_t _width, uint32_t _height)
{
    Args args(_argc, _argv);

    m_width = _width;
    m_height = _height;
    m_debug = BGFX_DEBUG_TEXT;
    m_reset = BGFX_RESET_VSYNC;

    bgfx::Init init;
    init.type = args.m_type;
    init.vendorId = args.m_pciId;
    init.platformData.nwh = entry::getNativeWindowHandle(entry::kDefaultWindowHandle);
    init.platformData.ndt = entry::getNativeDisplayHandle();
    init.resolution.width = m_width;
    init.resolution.height = m_height;
    init.resolution.reset = m_reset;
    bgfx::init(init);

    // Enanle debug text.
    bgfx::setDebug(m_debug);

    // Set view 0 clear state.
    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff, 1.0f, 0);

    imguiCreate();
    
    onInit();
}

int Application::shutdown()
{
    onShutdown();

    imguiDestroy();

    // Shutdown bgfx.
    bgfx::shutdown();

    return 0;
}

bool Application::update()
{
    if (!entry::processEvents(m_width, m_height, m_debug, m_reset, &m_mouseState))
    {
        int64_t now = bx::getHPCounter();
        static int64_t last = now;
        const int64_t frameTime = now - last;
        last = now;
        const double freq = double(bx::getHPFrequency());
        const float deltaTime = float(frameTime / freq);

        onUpdate(deltaTime);

        // Set view 0 default viewport.
        bgfx::setViewRect(0, 0, 0, uint16_t(m_width), uint16_t(m_height));

        // This dummy draw call is here to make sure that view 0 is cleard
        // if no other draw calls are submitted to view 0
        bgfx::touch(0);

        const float inAspectRatio = float(m_width / m_height);
        onRender(inAspectRatio);

        // Advance to next frame. Rendering thread will be kicked to
        // process submitted rendering primitives.
        bgfx::frame();

        // Gui Tick
        imguiBeginFrame(m_mouseState.m_mx, m_mouseState.m_my,
                        (m_mouseState.m_buttons[entry::MouseButton::Left] ? IMGUI_MBUT_LEFT : 0) |
                            (m_mouseState.m_buttons[entry::MouseButton::Right] ? IMGUI_MBUT_RIGHT : 0) |
                            (m_mouseState.m_buttons[entry::MouseButton::Middle] ? IMGUI_MBUT_MIDDLE : 0),
                        m_mouseState.m_mz, uint16_t(m_width), uint16_t(m_height));

        showExampleDialog(this);

        onGui();

        imguiEndFrame();

        return true;
    }

    return false;
}
