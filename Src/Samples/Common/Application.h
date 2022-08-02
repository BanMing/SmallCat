#ifndef _APPLICATION_H_
#define _APPLICATION_H_

// #include <dear-imgui/imgui.h>

namespace Tiga
{
    class Application
    {
    private:
        Application(const Application &);
        Application &operator=(const Application &);

    public:
        inline Application(/* args */) {}

        inline virtual ~Application() {}

        inline virtual void Initialize() {}

        inline virtual void Update(float inDeltaTime) {}

        inline virtual void Render(float inAspectRatio) {}

        inline virtual void OnGUI() {}

        inline virtual void Shutdown() {}
    };
} // namespace Tiga
#endif