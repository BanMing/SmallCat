#include <iostream>
#include <bx/uint32_t.h>
#include <common.h>
#include <bgfx_utils.h>
#include <imgui/imgui.h>

// int main(int argc, const char **argv)
// {
//     Tiga::Application *application = new Tiga::HelloSample();
//     return Tiga::Run(application, argc, argv);
// }

class HelloWorld : public entry ::AppI
{
protected:
    entry::MouseState m_mouseState;

    uint32_t m_width;
    uint32_t m_height;
    uint32_t m_debug;
    uint32_t m_reset;

public:
    HelloWorld(const char *_name, const char *_description, const char *_url)
        : entry::AppI(_name, _description, _url)
    {
    }

    void init(int32_t _argc, const char *const *_argv, uint32_t _width, uint32_t _height) override
    {
        Args args(_argc, _argv);

        m_width = _width;
        m_height = _height;
        m_debug = BGFX_DEBUG_TEXT;
        m_reset = BGFX_RESET_VSYNC;

        bgfx::Init init;
        init.type = args.m_type;
        init.vendorId = args.m_pciId;
        init.resolution.width = m_width;
        init.resolution.height = m_height;
        init.resolution.reset = m_reset;
        bgfx::init(init);

        // Enanle debug text.
        bgfx::setDebug(m_debug);

        // Set view 0 clear state.
        bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff, 1.0f, 0);

        imguiCreate();
    }

    virtual int shutdown() override
    {
        imguiDestroy();

        // Shutdown bgfx.
        bgfx::shutdown();

        return 0;
    }

    bool update() override
    {
        if (!entry::processEvents(m_width, m_height, m_debug, m_reset, &m_mouseState))
        {
            imguiBeginFrame(m_mouseState.m_mx, m_mouseState.m_my,
                            (m_mouseState.m_buttons[entry::MouseButton::Left] ? IMGUI_MBUT_LEFT : 0) |
                                (m_mouseState.m_buttons[entry::MouseButton::Right] ? IMGUI_MBUT_RIGHT : 0) |
                                (m_mouseState.m_buttons[entry::MouseButton::Middle] ? IMGUI_MBUT_MIDDLE : 0),
                            m_mouseState.m_mz, uint16_t(m_width), uint16_t(m_height));
        }
    }
};
