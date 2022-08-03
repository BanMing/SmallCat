#ifndef _HELLO_SAMPLE_H_
#define _HELLO_SAMPLE_H_

#include "../Application.h"

namespace Tiga
{
    class HelloSample : public Application
    {
    protected:
        /* data */
    public:
        void Initialize();
        void Update(float deltaTime);
        void Render(float inAspectRatio);
        void OnGUI();
        void Shutdown();
    };

} // namespace Tiga

#endif // _HELLO_SAMPLE_H_
