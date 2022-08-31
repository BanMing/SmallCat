#ifndef _EVENTQUEUE_H_
#define _EVENTQUEUE_H_
#include <bx/spscqueue.h>

#include "Event.h"
namespace Tiga
{
    class EventQueue
    {
    private:
        bx::SpScUnboundedQueueT<Event> mQueue;

    public:
        EventQueue() : mQueue(GetAllocator()){};
        ~EventQueue()
        {
            for (const Event *ev = Pop(); ev != NULL; ev = Pop())
            {
                Release(ev);
            }
        }

        void Post(Event *ev)
        {
            mQueue.push(ev);
        }

        const Event *Pop()
        {
            return mQueue.pop();
        }

        const Event *Pop(WindowHandle handle)
        {
            if (IsWindowHandleValid(handle))
            {
                Event *ev = mQueue.peek();
                if (ev == NULL || handle.idx != ev->mHandle.idx)
                {
                    return NULL;
                }
            }
            return Pop();
        }

        void Release(const Event *event) const
        {
            BX_DELETE(GetAllocator(), const_cast<Event *>(event));
        }
    };

} // namespace Tiga

#endif //_EVENTQUEUE_H_