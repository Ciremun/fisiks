#ifndef _ANIM_H_
#define _ANIM_H_

#include <stdint.h>

#define FADE_IN 0
#define FADE_OUT 1
#define IDLE 2
#define HIDDEN 3

typedef struct
{
    uint32_t color;
    double duration;
    double start;
    int state;
} Animation;

void change_animation_state(Animation *a, int new_state);
void display_message(char *msg);
void set_fade_color(Animation *a);
void draw_messages();

#endif // _ANIM_H_
