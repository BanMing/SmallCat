#ifndef _DROPFILEEVENT_H_
#define _DROPFILEEVENT_H_
#include <bx/filepath.h>

#include "Event.h"
#include "../Utils/BgfxUtils.h"
namespace Tiga
{
    struct DropFileEvent : public Event
    {
        bx::FilePath mFilePath;

        DropFileEvent(WindowHandle handle) : Event(Event::DropFile, handle) {}

        static DropFileEvent *GetEvent(WindowHandle handle, const bx::FilePath &filePath)
        {
            DropFileEvent *ev = BX_NEW(GetAllocator(), DropFileEvent)(handle);
            ev->mFilePath = filePath;
            return ev;
        }
    };

} // namespace Tiga

#endif //_DROPFILEEVENT_H_