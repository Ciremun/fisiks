#include "event.h"
#include "util.h"
#include "color.h"

Controls controls =
{
    .mouse_x = -1,
    .mouse_y = -1,
    .lmb_down = 0,
    .rmb_down = 0,
};

void EXPORT("HandleKey") HandleKey(int keycode, int bDown)
{
    if (bDown)
        switch (keycode)
        {
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
            memcpy(cells_count_buffer, "0", 2);
            memset(grid.cells, 0, GRID_SIZE(grid));
            memset(next_grid.cells, 0, GRID_SIZE(grid));
            reset_t = (int)OGGetAbsoluteTime();
            break;
        case MINUS_KEY:
        {
            int new_rows = grid.rows / 2;
            int new_cols = grid.cols / 2;
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
            change_grid_size(grid.rows * 2, grid.cols * 2);
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
    controls.lmb_down = bDown && (button == LMB_KEY);
    controls.rmb_down = bDown && (button == RMB_KEY);
    controls.mouse_x = x;
    controls.mouse_y = y;
}

void EXPORT("HandleMotion") HandleMotion(int x, int y, int mask)
{
#ifndef __ANDROID__
    if (!mask)
        return;
#endif // __ANDROID__
    controls.mouse_x = x;
    controls.mouse_y = y;

#ifdef __ANDROID__
    toggle_cell(ALIVE, x, y, SAND);
#endif // __ANDROID__
}

void HandleDestroy() {}

void EXPORT("OnResize") OnResize(int new_width, int new_height)
{
    w = new_width;
    h = new_height;
    CNFGSetup(WINDOW_NAME, w, h);
    cell_height = cell_width = min(w, h) / DEFAULT_CELL_SIZE;
    grid.rows = w / cell_width;
    grid.cols = h / cell_height;
    next_grid.rows = grid.rows;
    next_grid.cols = grid.cols;

    for (int y = 0; y < grid.cols; ++y)
        for (int x = 0; x < grid.rows; ++x)
            if (grid.cells[grid.cols * x + y].state != EMPTY)
                grid.cells[grid.cols * x + y].state = ALIVE;
}

#ifndef __wasm__
void HandleSuspend()
{
    suspended = 1;
}
void HandleResume()
{
    suspended = 0;
}
#else
void EXPORT("touchend") touchend(void)
{
    controls.lmb_down = 0;
}
#endif // __wasm__
