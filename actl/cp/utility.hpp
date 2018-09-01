#pragma once

#define TRY_ASSIGN(x, y)   [&] { try { x = (y); return true; } catch (...) { return false; } }()