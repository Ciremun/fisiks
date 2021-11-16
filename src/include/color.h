#ifndef _COLOR_H_
#define _COLOR_H_

#ifdef __wasm__
#define SWAPS(v) ((v>>24)&0xff) | ((v<<8)&0xff0000) | ((v>>8)&0xff00) | ((v<<24)&0xff000000)
#define COLOR(c) SWAPS(c)
#else
#define COLOR(c) c
#endif // __wasm__

#define TRANSPARENT_ 0XFFFFFF00
#define WHITE COLOR(0XFFFFFFFF)
#define BLACK COLOR(0X000000FF)
#define CAMEL COLOR(0xB88B4AFF)
#define FLAX COLOR(0xDDCA7DFF)
#define GOLDEN_BROWN COLOR(0xA27035FF)
#define CAFE_NOIR COLOR(0x533E2DFF)
#define RAISIN_BLACK COLOR(0x242331FF)
#define CARAMEL COLOR(0xFCBF49FF)
#define MAX_YELLOW_RED COLOR(0xFEBE5DFF)
#define SAND MAX_YELLOW_RED
#define STATIC_SAND CARAMEL

#endif // _COLOR_H_
