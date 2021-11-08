// Copyright (c) 2011-2020 <>< Charles Lohr - Under the MIT/x11 or NewBSD
// License you choose.
// NO WARRANTY! NO GUARANTEE OF SUPPORT! USE AT YOUR OWN RISK

#ifdef _MSC_VER
#pragma comment(lib, "gdi32")
#pragma comment(lib, "User32")
#endif // _MSC_VER

#ifndef __wasm__
#include "os_generic.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#endif // __wasm__

#define STB_SPRINTF_IMPLEMENTATION
#include "stb_sprintf.h"

#define CNFG_IMPLEMENTATION
#include "rawdraw_sf.h"

#include "anim.h"
#include "grid.h"
#include "color.h"
#include "keycode.h"
#include "typedef.h"
#include "util.h"

#define WINDOW_NAME "fisiks"
#define MAX_MESSAGE_SIZE 256

#define ALIVE 1
#define DEAD 0
#define DEFAULT_GRID_SIZE 32
#define GRID_SIZE_CHANGE_STEP 8

#define FADE_IN 0
#define FADE_OUT 1
#define IDLE 2
#define HIDDEN 3

int *grid = 0;
int *next_grid = 0;
int grid_size = DEFAULT_GRID_SIZE;
int paused = 0;
int reset_t = 0;
int message_t = 0;

short w, h;
int cell_width, cell_height;
double absolute_time;
char message[MAX_MESSAGE_SIZE];

extern Animation message_a;
extern Animation pause_a;

volatile int suspended;

#ifdef __ANDROID__
static int keyboard_up;
int font_size = 20;
int paused_t_width = 350;
#else
int font_size = 10;
int paused_t_width = 200;
#endif // __ANDROID__

void setup_window()
{
#ifdef __ANDROID__
    CNFGSetupFullscreen(WINDOW_NAME, 0);
    CNFGGetDimensions(&w, &h);
#elif defined(__wasm__)
    CNFGGetDimensions(&w, &h);
#else
    w = 900;
    h = 900;
#endif // __ANDROID__
    CNFGSetup(WINDOW_NAME, w, h);
}

int EXPORT("main") main()
{
    CNFGBGColor = BLACK;
    setup_window();

    cell_width = w / grid_size;
    cell_height = h / grid_size;

    grid = calloc(1, GRID_SIZE(grid_size));
    next_grid = calloc(1, GRID_SIZE(grid_size));

    grid[grid_size * 14 + 2] = ALIVE;
    grid[grid_size * 16 + 2] = ALIVE;
    grid[grid_size * 15 + 3] = ALIVE;
    grid[grid_size * 14 + 4] = ALIVE;
    grid[grid_size * 16 + 4] = ALIVE;

    grid[grid_size * 14 + 6] = ALIVE;
    grid[grid_size * 16 + 6] = ALIVE;
    grid[grid_size * 15 + 7] = ALIVE;
    grid[grid_size * 15 + 8] = ALIVE;

    display_message("fisiks");

#ifdef RAWDRAW_USE_LOOP_FUNCTION
    return 0;
}
int EXPORT("loop") loop()
{
#else
    while (1)
#endif
    {
        CNFGClearFrame();
        CNFGHandleInput();

#ifndef __wasm__
        if (suspended)
            continue;
        if (!paused)
            OGUSleep(5000);
#endif // __wasm__

        CNFGColor(SAND_COLOR);
        draw_cells();

        absolute_time = OGGetAbsoluteTime();
        draw_messages();

        CNFGSwapBuffers();
    }

    return (0);
}
