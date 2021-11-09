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

#include "event.h"
#include "anim.h"
#include "grid.h"
#include "color.h"
#include "keycode.h"
#include "typedef.h"
#include "util.h"

#define WINDOW_NAME "fisiks"
#define MAX_MESSAGE_SIZE 256

int paused = 0;
int reset_t = 0;
int message_t = 0;

short w, h;
int cell_width, cell_height;
double absolute_time;

char message_buffer[MAX_MESSAGE_SIZE];

String message = {
    .content = message_buffer,
    .length = 0,
};

extern Animation message_a;
extern Animation pause_a;
extern Controls controls;
extern Grid grid;
extern Grid next_grid;

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
    CNFGSetup(WINDOW_NAME, w, h);
#else
    w = 1600;
    h = 800;
    CNFGSetup(WINDOW_NAME, w, h);
#endif // __ANDROID__
}

int EXPORT("main") main()
{
    CNFGBGColor = BLACK;
    setup_window();

    cell_height = cell_width = min(w, h) / DEFAULT_CELL_SIZE;
    grid.rows = w / cell_width;
    grid.cols = h / cell_height;

    grid.cells = calloc(1, GRID_SIZE(grid));
    next_grid.cells = calloc(1, GRID_SIZE(grid));

    next_grid.cells[grid.cols * 14 + 2] = (Cell) {
        .state = ALIVE, .color = SAND_COLOR
    };
    next_grid.cells[grid.cols * 16 + 2] = (Cell) {
        .state = ALIVE, .color = SAND_COLOR
    };
    next_grid.cells[grid.cols * 15 + 3] = (Cell) {
        .state = ALIVE, .color = SAND_COLOR
    };
    next_grid.cells[grid.cols * 14 + 4] = (Cell) {
        .state = ALIVE, .color = SAND_COLOR
    };
    next_grid.cells[grid.cols * 16 + 4] = (Cell) {
        .state = ALIVE, .color = SAND_COLOR
    };
    next_grid.cells[grid.cols * 14 + 6] = (Cell) {
        .state = ALIVE, .color = SAND_COLOR
    };
    next_grid.cells[grid.cols * 16 + 6] = (Cell) {
        .state = ALIVE, .color = SAND_COLOR
    };
    next_grid.cells[grid.cols * 15 + 7] = (Cell) {
        .state = ALIVE, .color = SAND_COLOR
    };
    next_grid.cells[grid.cols * 15 + 8] = (Cell) {
        .state = ALIVE, .color = SAND_COLOR
    };

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

        if (controls.lmb_down)
            toggle_cell(ALIVE, controls.mouse_x, controls.mouse_y);

        draw_cells();

        absolute_time = OGGetAbsoluteTime();
        draw_messages();

        CNFGSwapBuffers();
    }

    return (0);
}
