#ifndef _APPCONST_H_
#define _APPCONST_H_
#include <iostream>
namespace Tiga
{

#define WINDOW_FLAG_NONE UINT32_C(0x00000000)
#define WINDOW_FLAG_ASPECT_RATIO UINT32_C(0x00000001)
#define WINDOW_FLAG_FRAME UINT32_C(0x00000002)

    const uint16_t kAppWidth = 800;
    const uint16_t kAppHeight = 600;

    const int kMaxKeyCount = 256;
    const int kMaxWindows = 8;

    const int kMainWindowIndex = 0;

} // namespace Tiga

#endif //_APPCONST_H_