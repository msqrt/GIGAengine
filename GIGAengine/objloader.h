#pragma once

#include <string>
#include "main.h"

namespace util {
mesh* loadWavefrontObj(std::string path, bool verbose=false);
}