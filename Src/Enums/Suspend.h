#ifndef _SUSPEND_H_
#define _SUSPEND_H_
namespace Tiga
{
    struct Suspend
    {
        enum Enum
        {
            WillSuspend,
            DidSuspend,
            WillResume,
            DidResume,

            Count
        };
    };

} // namespace Tiga

#endif //_SUSPEND_H_