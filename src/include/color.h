#ifndef _COLOR_H_
#define _COLOR_H_

#ifdef __wasm__
#define SWAPS(v) ((v>>24)&0xff) | ((v<<8)&0xff0000) | ((v>>8)&0xff00) | ((v<<24)&0xff000000)
#define COLOR(c) SWAPS(c)
#else
#define COLOR(c) c
#endif // __wasm__

#define TRANSPARENT_ 0xffffff00
#define WHITE COLOR(0xffffffff)
#define BLACK COLOR(0x000000ff)
#define SAND_COLOR COLOR(0xfcbf49ff)

#endif // _COLOR_H_
