#include "BgfxUtils.h"

namespace Tiga
{
    bx::AllocatorI *g_allocator = GetDefaultAllocator();
    bx::AllocatorI *GetDefaultAllocator()
    {
        BX_PRAGMA_DIAGNOSTIC_PUSH();
        BX_PRAGMA_DIAGNOSTIC_IGNORED_MSVC(4459); // warning C4459: declaration of 's_allocator' hides global declaration
        BX_PRAGMA_DIAGNOSTIC_IGNORED_CLANG_GCC("-Wshadow");
        static bx::DefaultAllocator s_allocator;
        return &s_allocator;
        BX_PRAGMA_DIAGNOSTIC_POP();
    }

    bx::AllocatorI *GetAllocator()
    {
        if (NULL == g_allocator)
        {
            g_allocator = GetDefaultAllocator();
        }

        return g_allocator;
    }
} // namespace Tiga
