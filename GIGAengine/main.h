
#pragma once

#define UNICODE

#include <windows.h>
#include <Gdiplus.h>
#include <gl/gl.h>
#include "glext.h"
#include "wglext.h"
#include <cmath>
#include <string>
#include <sstream>

#pragma warning(disable:995)
#include <dshow.h>
#pragma warning(default:995)


//don't define this unless you're window.cpp
#ifdef declaregl
#include "gldeclare.h"
#else
#include "gldefines.h"
#endif

#include "mesh.h"
#include "shader.h"
#include "texture.h"
#include "texture3D.h"
#include "target.h"
#include "window.h"
#include "song.h"
#include "video.h"

#include "resource.h"

#include <map>

extern int global_screenw;
extern int global_screenh;