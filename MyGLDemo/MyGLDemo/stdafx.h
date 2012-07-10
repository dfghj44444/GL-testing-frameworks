// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头中排除极少使用的资料
// Windows 头文件:
#include <windows.h>


// C 运行时头文件
#include <assert.h>
#include <stdlib.h>
#include <malloc.h>
#include <tchar.h>
#include <mmsystem.h>



//STL header
#include <list>
#include <vector>
#include <iostream> 
#include <memory>
#include <iostream>
#include <algorithm>
#include <cmath>


// TODO: 在此处引用程序需要的其他头文件
#include <gl/glew.h>
#include <gl/gl.h>
#include <gl/glu.h>


//lib
#pragma comment (lib,"winmm.lib");
#pragma comment (lib,"glew32.lib");
#pragma comment (lib,"OpenGL32.lib");
#pragma comment (lib,"GLu32.lib");
