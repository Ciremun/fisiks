#include "grid.h"

void change_grid_size(int new_size)
{
    cell_width = w / new_size;
    cell_height = h / new_size;
    grid = realloc(grid, GRID_SIZE(new_size));
    next_grid = realloc(next_grid, GRID_SIZE(new_size));
    memset(grid, 0, GRID_SIZE(new_size));
    memset(next_grid, 0, GRID_SIZE(new_size));
    stbsp_snprintf(message, MAX_MESSAGE_SIZE, "%s: %d", "Grid Size", new_size);
    message_t = (int)OGGetAbsoluteTime();
    change_animation_state(&message_a, FADE_IN);
    grid_size = new_size;
}

void draw_cell(int x, int y)
{
    int cell_x = x * cell_width;
    int cell_y = y * cell_height;

    CNFGTackRectangle(cell_x, cell_y, cell_x + cell_width,
                      cell_y + cell_height);
}

void draw_cells()
{
    memcpy(next_grid, grid, GRID_SIZE(grid_size));
    for (int y = 0; y < grid_size; ++y)
        for (int x = 0; x < grid_size; ++x) {
            if (!paused)
                apply_game_rules(x, y);
            if (grid[x * grid_size + y] == ALIVE)
                draw_cell(x, y);
        }
    memcpy(grid, next_grid, GRID_SIZE(grid_size));
}

void cell_index(int x, int y, int *cell_x, int *cell_y)
{
    *cell_x = x / (w / grid_size);
    *cell_y = y / (h / grid_size);
}

int on_grid(int cell_i)
{
    return 0 <= cell_i && cell_i <= grid_size - 1;
}

void toggle_cell(int x, int y, int val)
{
    int cell_x, cell_y;
    cell_index(x, y, &cell_x, &cell_y);
    if (on_grid(cell_x) && on_grid(cell_y))
        grid[cell_x * grid_size + cell_y] = val;
}

void apply_game_rules(int x, int y)
{
    if (grid[x * grid_size + y] == ALIVE && on_grid(y + 1)) {
        if (next_grid[x * grid_size + y + 1] == DEAD) {
            next_grid[x * grid_size + y] = DEAD;
            next_grid[x * grid_size + y + 1] = ALIVE;
        }
    }
}
