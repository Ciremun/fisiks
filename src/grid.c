#include "grid.h"

Grid grid = {0};
int grid_size_change_step = 4;

// TODO: null check
void change_grid_size(int new_rows, int new_cols)
{
    cell_height = cell_width = min(w, h) / min(new_rows, new_cols);
    grid.rows = w / cell_width;
    grid.cols = h / cell_height;
    grid.data = realloc(grid.data, GRID_SIZE(grid));
    next_grid_data = realloc(next_grid_data, GRID_SIZE(grid));
    memset(grid.data, 0, GRID_SIZE(grid));
    memset(next_grid_data, 0, GRID_SIZE(grid));
    stbsp_snprintf(message.content, MAX_MESSAGE_SIZE, "Grid Size: %dx%d", new_rows, new_cols);
    message_t = (int)OGGetAbsoluteTime();
    change_animation_state(&message_a, FADE_IN);
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
    for (int y = 0; y < grid.cols; ++y)
        for (int x = 0; x < grid.rows; ++x) {
            if (!paused)
                apply_game_rules(x, y);
            if (grid.data[x * grid.cols + y] == ALIVE)
                draw_cell(x, y);
        }
    memcpy(grid.data, next_grid_data, GRID_SIZE(grid));
}

void cell_index(int x, int y, int *cell_x, int *cell_y)
{
    *cell_x = x / (w / grid.rows);
    *cell_y = y / (h / grid.cols);
}

int row_exists(int cell_x)
{
    return cell_x < grid.rows;
}

int col_exists(int cell_y)
{
    return cell_y < grid.cols;
}

void toggle_cell(int x, int y, int val)
{
    int cell_x, cell_y;
    cell_index(x, y, &cell_x, &cell_y);
    if (row_exists(cell_x) && col_exists(cell_y))
        next_grid_data[cell_x * grid.cols + cell_y] = val;
}

void apply_game_rules(int x, int y)
{
    if (grid.data[x * grid.cols + y] == ALIVE && col_exists(y + 1)) {
        if (next_grid_data[x * grid.cols + y + 1] == DEAD) {
            next_grid_data[x * grid.cols + y] = DEAD;
            next_grid_data[x * grid.cols + y + 1] = ALIVE;
        }
    }
}
