#ifndef _KEYCODE_H_
#define _KEYCODE_H_

#define SPACE_KEY 32
#define ZERO_KEY 48
#define ONE_KEY 49
#define TWO_KEY 50
#define NINE_KEY 57


#ifdef __wasm__
#define R_KEY 82
#define LMB_KEY 0
#elif defined(__ANDROID__)
#define LMB_KEY 0
#define R_KEY 114
#else
#define LMB_KEY 1
#define R_KEY 114
#endif // __wasm__


#if defined(_WIN32) || defined(__wasm__)
#define MINUS_KEY 189
#define PLUS_KEY 187
#define RMB_KEY 2
#else
#define MINUS_KEY 45
#define PLUS_KEY 43
#define EQ_KEY 61
#define RMB_KEY 3
#endif // _WIN32
#endif // _KEYCODE_H_
