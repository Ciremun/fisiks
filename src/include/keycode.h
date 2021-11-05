#ifndef _KEYCODES_H_
#define _KEYCODES_H_

#define SPACE_KEY 32
#define ONE_KEY 49
#define TWO_KEY 50

#ifdef __wasm__
#define R_KEY 82
#else
#define R_KEY 114
#endif // __wasm__

#if defined(_WIN32) || defined(__wasm__)
#define MINUS_KEY 189
#define PLUS_KEY 187
#else
#define MINUS_KEY 45
#define PLUS_KEY 43
#define EQ_KEY 61
#endif // _WIN32
#endif // _KEYCODES_H_
