#pragma once

#include "debugger.h"
#define ASSERT(expr, message) if (expr) { DEBUG ( DebugLevel::ERROR, message ); }
