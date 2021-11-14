#include "event.h"
#include "util.h"

Controls controls =
{
    .mouse_x = -1,
    .mouse_y = -1,
    .lmb_down = 0,
};

void EXPORT("HandleKey") HandleKey(int keycode, int bDown)
{
    if (bDown)
        switch (keycode)
        {
        case NINE_KEY:
        {
            if (grid_size_change_step - 1 <= 0)
            {
                display_message("too smol grid step");
                return;
            }
            grid_size_change_step--;
            stbsp_snprintf(message.content, MAX_MESSAGE_SIZE, "Grid Step: %d", grid_size_change_step);
            message_t = (int)OGGetAbsoluteTime();
            change_animation_state(&message_a, FADE_IN);
        }
        break;
        case ZERO_KEY:
        {
            grid_size_change_step++;
            stbsp_snprintf(message.content, MAX_MESSAGE_SIZE, "Grid Step: %d", grid_size_change_step);
            message_t = (int)OGGetAbsoluteTime();
            change_animation_state(&message_a, FADE_IN);
        }
        break;
        case SPACE_KEY:
            paused = !paused;
            switch (pause_a.state)
            {
            case FADE_IN:
                pause_a.state = FADE_OUT;
                break;
            case FADE_OUT:
                pause_a.state = FADE_IN;
                break;
            case IDLE:
                change_animation_state(&pause_a, FADE_OUT);
                break;
            case HIDDEN:
                change_animation_state(&pause_a, FADE_IN);
                break;
            }
            break;
        case R_KEY:
            memset(grid.cells, 0, GRID_SIZE(grid));
            memset(next_grid.cells, 0, GRID_SIZE(grid));
            reset_t = (int)OGGetAbsoluteTime();
            break;
        case MINUS_KEY:
        {
            int new_rows = grid.rows - grid_size_change_step;
            int new_cols = grid.cols - grid_size_change_step;
            if (new_rows <= 0 || new_cols <= 0)
            {
                display_message("too smol");
                return;
            }
            change_grid_size(new_rows, new_cols);
        }
        break;
#if !defined(_WIN32) && !defined(__wasm__)
        case EQ_KEY:
#endif
        case PLUS_KEY:
            change_grid_size(grid.rows + grid_size_change_step, grid.cols + grid_size_change_step);
            break;
        }
#ifdef __ANDROID__
    else
    {
        switch (keycode)
        {
        case 10:
            keyboard_up = 0;
            AndroidDisplayKeyboard(keyboard_up);
            break;
        case 4:
            AndroidSendToBack(1);
            break;
        }
    }
#endif // __ANDROID__
}

void EXPORT("HandleButton") HandleButton(int x, int y, int button, int bDown)
{
    (void)button;
    if (bDown)
    {
#ifdef __ANDROID__
        if ((w - 100 <= x && x <= w) && (0 <= y && y <= 100))
        {
            keyboard_up = !keyboard_up;
            AndroidDisplayKeyboard(keyboard_up);
            return;
        }
#endif // __ANDROID__
    }
    controls.lmb_down = bDown;
    controls.mouse_x = x;
    controls.mouse_y = y;
}

void EXPORT("HandleMotion") HandleMotion(int x, int y, int mask)
{
#ifndef __ANDROID__
    if (!mask)
        return;
#endif
    controls.mouse_x = x;
    controls.mouse_y = y;
    toggle_cell(ALIVE, x, y);
}

void HandleDestroy() {}

#ifndef __wasm__
void HandleSuspend()
{
    suspended = 1;
}
void HandleResume()
{
    suspended = 0;
}
#endif // __wasm__
