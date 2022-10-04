#ifndef _APP_H_
#define _APP_H_

#include <common.h>
#include <bgfx_utils.h>
#include <imgui/imgui.h>

class Application : public entry ::AppI
{
protected:
    entry::MouseState m_mouseState;

    uint32_t m_width;
    uint32_t m_height;
    uint32_t m_debug;
    uint32_t m_reset;

public:
    Application(const char *_name, const char *_description)
        : entry::AppI(_name, _description, "")
    {
    }
    void init(int32_t _argc, const char *const *_argv, uint32_t _width, uint32_t _height) override;

    int shutdown() override;
    bool update() override;

protected:
    inline virtual void onInit() {}
    inline virtual void onShutdown() {}
    inline virtual void onUpdate(float _deltaTime) {}
    inline virtual void onRender(float _inAspectRatio) {}
    inline virtual void onGui() {}
};
#endif //_APP_H_