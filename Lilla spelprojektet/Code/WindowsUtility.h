#ifndef WINDOWSUTILITY_H
#define WINDOWSUTILITY_H

#include <vector>
#include <WindowsX.h>

#define SAFE_DELETE(x) if( x ) { delete(x); (x) = NULL; }
#define SAFE_DELETE_ARRAY(x) if( x ) { delete[](x); (x) = NULL; }

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768



#endif