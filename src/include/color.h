#ifndef _COLOR_H_
#define _COLOR_H_

#include "util.h"

#ifdef __wasm__
#define COLOR(c) SWAPS(c)
#else
#define COLOR(c) c
#endif // __wasm__

#define TRANSPARENT_ 0xffffff00
#define WHITE COLOR(0xffffffff)
#define BLACK COLOR(0x000000ff)
#define SAND_COLOR COLOR(0xfcbf49ff)

#endif // _COLOR_H_
