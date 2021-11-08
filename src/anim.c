#include "anim.h"

extern int font_size;
extern String message;

Animation pause_a = {
    .color = WHITE,
    .duration = .5,
    .start = 0.0,
    .state = HIDDEN,
};

Animation message_a = {
    .color = WHITE,
    .duration = 1.0,
    .start = 0.0,
    .state = HIDDEN,
};

void change_animation_state(Animation *a, int new_state)
{
    a->start = OGGetAbsoluteTime();
    a->state = new_state;
}

void display_message(char *msg)
{
    u64 message_length = strlen(msg);
    memset(message.content, 0, MAX_MESSAGE_SIZE);
    memcpy(message.content, msg, message_length + 1);
    message.length = message_length;
    message_t = (int)OGGetAbsoluteTime();
    change_animation_state(&message_a, FADE_IN);
}

void set_fade_color(Animation *a)
{
    uint32_t new_color = 0;
    switch (a->state) {
    case FADE_IN: {
        double s_passed = absolute_time - a->start;
        if (s_passed >= a->duration) {
            a->state = IDLE;
            new_color = a->color;
        } else {
            new_color = (uint32_t)((a->color & TRANSPARENT_)
                                   + (s_passed / a->duration) * 255);
        }
    }
    break;
    case FADE_OUT: {
        double s_passed = absolute_time - a->start;
        if (s_passed >= a->duration) {
            a->state = HIDDEN;
            new_color = a->color & TRANSPARENT_;
        } else {
            new_color
                = (uint32_t)((a->color & TRANSPARENT_)
                             + ((a->duration - s_passed) / a->duration) * 255);
        }
    }
    break;
    case IDLE:
        new_color = a->color;
        break;
    default:
        break;
    }
    CNFGColor(COLOR(new_color));
}

void draw_messages()
{
    if (pause_a.state != HIDDEN) {
        set_fade_color(&pause_a);
        draw_text("Paused", w - paused_t_width, 10, font_size);
    }
    if (message_a.state != HIDDEN) {
        if (message_t && absolute_time - message_t > 2) {
            message_t = 0;
            change_animation_state(&message_a, FADE_OUT);
        }
        set_fade_color(&message_a);
        draw_text(message.content, w / 2 - message.length * 30, 120, font_size);
    }
    if (reset_t) {
        if (absolute_time - reset_t <= 1) {
            CNFGColor(WHITE);
            draw_text("Reset", 10, 10, font_size);
        } else {
            reset_t = 0;
        }
    }
}
