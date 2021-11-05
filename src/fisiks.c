// Copyright (c) 2011-2020 <>< Charles Lohr - Under the MIT/x11 or NewBSD
// License you choose.
// NO WARRANTY! NO GUARANTEE OF SUPPORT! USE AT YOUR OWN RISK

#ifndef __wasm__
#include "os_generic.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#else
#include <stdint.h>
static uint32_t SWAPS(uint32_t r)
{
    uint32_t ret = (r & 0xFF) << 24;
    r >>= 8;
    ret |= (r & 0xff) << 16;
    r >>= 8;
    ret |= (r & 0xff) << 8;
    r >>= 8;
    ret |= (r & 0xff) << 0;
    return ret;
}
#define STB_SPRINTF_IMPLEMENTATION
#include "stb_sprintf.h"
#define snprintf stbsp_snprintf
#endif // __wasm__

#ifdef _MSC_VER
#pragma comment(lib, "gdi32")
#pragma comment(lib, "User32")
#endif // _MSC_VER

#define CNFG_IMPLEMENTATION
#include "rawdraw_sf.h"

#define WINDOW_NAME      "fisiks"
#define MAX_MESSAGE_SIZE 256

#define ALIVE                 1
#define DEAD                  0
#define DYING                 2
#define DEFAULT_GRID_SIZE     32
#define GRID_SIZE_CHANGE_STEP 8

#define GAME_OF_LIFE 0
#define BRIANS_BRAIN 1

#ifdef __wasm__
#define COLOR(c)  SWAPS(c)
#define EXPORT(s) __attribute__((export_name(s)))
#else
#define COLOR(c) c
#define EXPORT(s)
#endif // __wasm__

#define TRANSPARENT_ 0xffffff00
#define WHITE        COLOR(0xffffffff)

#define FADE_IN  0
#define FADE_OUT 1
#define IDLE     2
#define HIDDEN   3

#define SPACE_KEY 32
#define ONE_KEY   49
#define TWO_KEY   50

#ifdef __wasm__
#define R_KEY 82
#else
#define R_KEY 114
#endif // __wasm__

#if defined(_WIN32) || defined(__wasm__)
#define MINUS_KEY 189
#define PLUS_KEY  187
#else
#define MINUS_KEY 45
#define PLUS_KEY  43
#define EQ_KEY    61
#endif // _WIN32

#define GRID_SIZE(gs) (sizeof(int) * gs * gs)

typedef unsigned long long int u64;

int *grid = 0;
int *next_grid = 0;
int  grid_size = DEFAULT_GRID_SIZE;
int  paused = 0;
int  reset_t = 0;
int  message_t = 0;

short  w, h;
int    cell_width, cell_height;
double absolute_time;
char   message[MAX_MESSAGE_SIZE];

volatile int suspended;

#ifdef __ANDROID__
static int keyboard_up;
int        font_size = 20;
int        paused_t_width = 350;
#else
int font_size = 10;
int paused_t_width = 200;
#endif // __ANDROID__

#ifdef __wasm__
extern unsigned char __heap_base;
char                *heap = (char *)&__heap_base;
#endif // __wasm__

typedef struct
{
    uint32_t color;
    double   duration;
    double   start;
    int      state;
} Animation;

Animation pause_a = {
    .color = 0,
    .duration = .5,
    .start = 0.0,
    .state = HIDDEN,
};

Animation message_a = {
    .color = 0,
    .duration = 1.0,
    .start = 0.0,
    .state = HIDDEN,
};

double OGGetAbsoluteTime();
void   print(double idebug);
#ifndef __wasm__
void print(double idebug) { (void)idebug; }
#endif // __wasm__

#ifdef __wasm__
#define malloc  fisiks_malloc
#define realloc fisiks_realloc
#define strlen  fisiks_strlen
#define memset  fisiks_memset
#define memcpy  fisiks_memcpy

u64 fisiks_strlen(const char *s)
{
    u64 sz = 0;
    while (s[sz] != '\0')
        sz++;
    return sz;
}

void *fisiks_memset(void *dest, int val, u64 len)
{
    unsigned char *ptr = dest;
    while (len-- > 0)
        *ptr++ = val;
    return dest;
}

void *fisiks_memcpy(void *dst, void const *src, u64 size)
{
    unsigned char *source = (unsigned char *)src;
    unsigned char *dest = (unsigned char *)dst;
    while (size--)
        *dest++ = *source++;
    return dst;
}

void *fisiks_malloc(unsigned long long size)
{
    heap += size;
    return heap - size;
}

void *fisiks_realloc(void *old_mem, unsigned long long size)
{
    // since we only have a grid
    u64 old_size = GRID_SIZE(grid_size);
    if (size <= old_size)
    {
        heap -= old_size;
        return heap;
    }
    void *new_mem = fisiks_malloc(size);
    fisiks_memcpy(new_mem, old_mem, old_size);
    return new_mem;
}
#endif // __wasm__

void change_animation_state(Animation *a, int new_state)
{
    a->start = OGGetAbsoluteTime();
    a->state = new_state;
}

void display_message(char *msg)
{
    u64 msg_size = strlen(msg) + 1;
    memset(message, 0, MAX_MESSAGE_SIZE);
    memcpy(message, msg, msg_size);
    message_t = (int)OGGetAbsoluteTime();
    change_animation_state(&message_a, FADE_IN);
}

void set_fade_color(Animation *a)
{
    uint32_t new_color = 0;
    switch (a->state)
    {
    case FADE_IN:
    {
        double s_passed = absolute_time - a->start;
        if (s_passed >= a->duration)
        {
            a->state = IDLE;
            new_color = a->color;
        }
        else
        {
            new_color = (uint32_t)((a->color & TRANSPARENT_)
                                   + (s_passed / a->duration) * 255);
        }
    }
    break;
    case FADE_OUT:
    {
        double s_passed = absolute_time - a->start;
        if (s_passed >= a->duration)
        {
            a->state = HIDDEN;
            new_color = a->color & TRANSPARENT_;
        }
        else
        {
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

void change_grid_size(int new_size)
{
    cell_width = w / new_size;
    cell_height = h / new_size;
    grid = realloc(grid, GRID_SIZE(new_size));
    next_grid = realloc(next_grid, GRID_SIZE(new_size));
    memset(grid, 0, GRID_SIZE(new_size));
    memset(next_grid, 0, GRID_SIZE(new_size));
    snprintf(message, MAX_MESSAGE_SIZE, "%s: %d", "Grid Size", new_size);
    message_t = (int)OGGetAbsoluteTime();
    change_animation_state(&message_a, FADE_IN);
    grid_size = new_size;
}

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
            memset(grid, 0, GRID_SIZE(grid_size));
            reset_t = (int)OGGetAbsoluteTime();
            break;
        case MINUS_KEY:
        {
            int new_size = grid_size - GRID_SIZE_CHANGE_STEP;
            if (new_size <= 0)
            {
                return;
            }
            change_grid_size(new_size);
        }
        break;
#if !defined(_WIN32) && !defined(__wasm__)
        case EQ_KEY:
#endif
        case PLUS_KEY:
            change_grid_size(grid_size + GRID_SIZE_CHANGE_STEP);
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

void cell_index(int x, int y, int *cell_x, int *cell_y)
{
    *cell_x = x / (w / grid_size);
    *cell_y = y / (h / grid_size);
}

int on_grid(int cell_i) { return 0 <= cell_i && cell_i <= grid_size - 1; }

void toggle_cell(int x, int y, int val)
{
    int cell_x, cell_y;
    cell_index(x, y, &cell_x, &cell_y);
    if (on_grid(cell_x) && on_grid(cell_y))
        grid[cell_x * grid_size + cell_y] = val;
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
        toggle_cell(x, y, ALIVE);
    }
}

void EXPORT("HandleMotion") HandleMotion(int x, int y, int mask)
{
#ifndef __ANDROID__
    if (!mask)
        return;
#endif
    toggle_cell(x, y, ALIVE);
}

void HandleDestroy() {}

#ifndef __wasm__
void HandleSuspend() { suspended = 1; }
void HandleResume() { suspended = 0; }
#endif // __wasm__

void setup_window()
{
#ifdef __ANDROID__
    CNFGSetupFullscreen(WINDOW_NAME, 0);
    CNFGGetDimensions(&w, &h);
#else
    w = 1024;
    h = 768;
    CNFGSetup(WINDOW_NAME, w, h);
#endif // __ANDROID__
}

void draw_message(int x, int y, const char *t)
{
    CNFGPenX = x;
    CNFGPenY = y;
    CNFGDrawText(t, font_size);
}

void draw_cell(int x, int y)
{
    int cell_x = x * cell_width;
    int cell_y = y * cell_height;

    CNFGTackRectangle(cell_x, cell_y, cell_x + cell_width,
                      cell_y + cell_height);
}

void apply_game_rules(int x, int y)
{
    if (grid[x * grid_size + y] == ALIVE && on_grid(y + 1))
    {
        if (next_grid[x * grid_size + y + 1] == DEAD)
        {
            next_grid[x * grid_size + y] = DEAD;
            next_grid[x * grid_size + y + 1] = ALIVE;
        }
    }
}

void draw_cells()
{
    memcpy(next_grid, grid, GRID_SIZE(grid_size));
    for (int y = 0; y < grid_size; ++y)
        for (int x = 0; x < grid_size; ++x)
        {
            if (!paused)
                apply_game_rules(x, y);
            if (grid[x * grid_size + y] == ALIVE)
                draw_cell(x, y);
        }
    memcpy(grid, next_grid, GRID_SIZE(grid_size));
}

void draw_messages()
{
    if (pause_a.state != HIDDEN)
    {
        set_fade_color(&pause_a);
        draw_message(w - paused_t_width, 10, "Paused");
    }
    if (message_a.state != HIDDEN)
    {
        if (message_t && absolute_time - message_t > 2)
        {
            message_t = 0;
            change_animation_state(&message_a, FADE_OUT);
        }
        set_fade_color(&message_a);
        int message_length = (int)strlen(message);
        draw_message(w / 2 - message_length * 30, 120, message);
    }
    if (reset_t)
    {
        if (absolute_time - reset_t <= 1)
        {
            CNFGColor(WHITE);
            draw_message(10, 10, "Reset");
        }
        else
        {
            reset_t = 0;
        }
    }
}

int EXPORT("main") main()
{
    CNFGBGColor = COLOR(0x000080ff);
    setup_window();

    pause_a.color = WHITE;
    message_a.color = WHITE;

    cell_width = w / grid_size;
    cell_height = h / grid_size;

    grid = malloc(GRID_SIZE(grid_size));
    next_grid = malloc(GRID_SIZE(grid_size));
    memset(grid, 0, GRID_SIZE(grid_size));
    memset(next_grid, 0, GRID_SIZE(grid_size));

    // []  []
    //   []
    // []  []
    grid[grid_size * 14 + 2] = ALIVE;
    grid[grid_size * 16 + 2] = ALIVE;
    grid[grid_size * 15 + 3] = ALIVE;
    grid[grid_size * 14 + 4] = ALIVE;
    grid[grid_size * 16 + 4] = ALIVE;

    // []  []
    //   []
    //   []
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

        CNFGColor(COLOR(0xff00ffff));
        draw_cells();

        absolute_time = OGGetAbsoluteTime();
        draw_messages();

        CNFGSwapBuffers();
    }

    return (0);
}
