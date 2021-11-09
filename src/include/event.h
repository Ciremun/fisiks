#ifndef _EVENT_H_
#define _EVENT_H_

#include "util.h"

void EXPORT("HandleKey") HandleKey(int keycode, int bDown);
void EXPORT("HandleButton") HandleButton(int x, int y, int button, int bDown);
void HandleDestroy();

#ifndef __wasm__
void HandleSuspend();
void HandleResume();
#endif // __wasm__

typedef struct {
    int mouse_x;
    int mouse_y;
    int lmb_down;
} Controls;

#endif // _EVENT_H_
