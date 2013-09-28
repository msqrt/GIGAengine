
#ifndef MAIN_H
#define MAIN_H

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


//don't define this unless you're window.cpp or know what you're doing (in which case you know not to)
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

typedef std::map<std::string, float> SyncMap;

#endif
