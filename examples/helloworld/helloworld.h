#ifndef _HELLOWORLD_H_
#define _HELLOWORLD_H_

#include "../application.h"

class HelloWorld : public Application
{
public:
    HelloWorld(const char *_name, const char *_description)
        : Application(_name, _description)
    {
    }

protected:
    void onRender(float _inAspectRatio) override;
};

#endif //_HELLOWORLD_H_