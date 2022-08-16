#ifndef _APPCONST_H_
#define _APPCONST_H_
#include <iostream>
namespace Tiga
{
    const uint16_t kAppWidth = 800;
    const uint16_t kAppHeight = 600;

    const int kMaxKeyCount = 256;
    const int kMaxWindows = 8;

    const int kMainWindowIndex = 0;

#ifndef CONFIG_MAX_WINDOWS
#define CONFIG_MAX_WINDOWS 8
#endif // CONFIG_MAX_WINDOWS
} // namespace Tiga

#endif //_APPCONST_H_