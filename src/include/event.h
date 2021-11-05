#ifndef _EVENT_H_
#define _EVENT_H_

void EXPORT("HandleKey") HandleKey(int keycode, int bDown);
void EXPORT("HandleButton") HandleButton(int x, int y, int button, int bDown);
void HandleDestroy();

#ifndef __wasm__
void HandleSuspend();
void HandleResume();
#endif // __wasm__

#endif // _EVENT_H_