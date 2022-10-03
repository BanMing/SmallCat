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
    Application(const char *_name, const char *_description, const char *_url)
        : entry::AppI(_name, _description, _url)
    {
    }
    void init(int32_t _argc, const char *const *_argv, uint32_t _width, uint32_t _height) override;

    int shutdown() override;
    bool update() override;

protected:
    virtual void onInit() = 0;
    virtual int onShutdown() = 0;
    virtual void onUpdate(float _deltaTime) = 0;
    virtual void onRender(float _inAspectRatio) = 0;
    virtual void onGui() = 0;
};
#endif //_APP_H_