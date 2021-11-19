#include "grid.h"

Grid grid = {0};
Grid next_grid = {0};

// TODO: null check
void change_grid_size(int new_rows, int new_cols)
{
    cell_height = cell_width = min(w, h) / min(new_rows, new_cols);
    grid.rows = w / cell_width;
    grid.cols = h / cell_height;
    next_grid.rows = grid.rows;
    next_grid.cols = grid.cols;
    grid.cells = realloc(grid.cells, GRID_SIZE(grid));
    next_grid.cells = realloc(next_grid.cells, GRID_SIZE(grid));
    memset(grid.cells, 0, GRID_SIZE(grid));
    memset(next_grid.cells, 0, GRID_SIZE(grid));
    message.length = stbsp_snprintf(message.content, MAX_MESSAGE_SIZE, "Grid Size: %dx%d", new_rows, new_cols);
    message_t = (int)OGGetAbsoluteTime();
    change_animation_state(&message_a, FADE_IN);
    memcpy(cells_count_buffer, "0", 2);
}

void draw_cell(Cell cell, int x, int y)
{
    int cell_x = x * cell_width;
    int cell_y = y * cell_height;

    CNFGColor(cell.color);

    CNFGTackRectangle(cell_x, cell_y, cell_x + cell_width,
                      cell_y + cell_height);
}

void draw_cells()
{
    for (int y = 0; y < grid.cols; ++y)
    {
        for (int x = 0; x < grid.rows; ++x)
        {
            if (!paused)
                apply_game_rules(x, y);
            switch (grid.cells[grid.cols * x + y].state)
            {
            case ALIVE:
            case STATIC:
                draw_cell(grid.cells[grid.cols * x + y], x, y);
            default:
                break;
            }
        }
    }
    memcpy(grid.cells, next_grid.cells, GRID_SIZE(grid));
}

void cell_index(int x, int y, int *cell_x, int *cell_y)
{
    *cell_x = x / (w / grid.rows);
    *cell_y = y / (h / grid.cols);
}

int row_exists(int cell_y)
{
    return cell_y >= 0 && cell_y < grid.rows;
}

int col_exists(int cell_x)
{
    return cell_x >= 0 && cell_x < grid.cols;
}

void toggle_cell(CellState state, int x, int y, uint32_t color)
{
    int cell_x, cell_y;
    cell_index(x, y, &cell_x, &cell_y);
    if (state == EMPTY)
    {
        if (grid.cells[grid.cols * cell_x + cell_y].state == EMPTY)
        set_adjacent_cells_state_if_not_empty(&next_grid, ALIVE, cell_x, cell_y, ALL);
    }
    else
    {
        if (grid.cells[grid.cols * cell_x + cell_y].state != EMPTY)
            return;
    }
    next_grid.cells[grid.cols * cell_x + cell_y] = (Cell)
    {
        .state = state, .color = color
    };
    update_cells_count = 1;
}

void apply_game_rules(int x, int y)
{
    if (grid.cells[grid.cols * x + y].state == ALIVE)
    {
        if (col_exists(y + 1))
        {
            // sand
            if (grid.cells[grid.cols * x + y + 1].state == EMPTY)
            {
                next_grid.cells[grid.cols * x + y].state = EMPTY;
                next_grid.cells[grid.cols * x + y + 1] = (Cell)
                {
                    .state = grid.cells[grid.cols * x + y].state,
                    .color = SAND,
                };
                set_adjacent_cells_state_if_not_empty(&next_grid, ALIVE, x, y, TOP | LEFT | RIGHT);
            }
            else
            {
                // bottom left cell is free
                if (row_exists(x - 1) && grid.cells[grid.cols * (x - 1) + y + 1].state == EMPTY)
                {
                    next_grid.cells[grid.cols * x + y].state = EMPTY;
                    next_grid.cells[grid.cols * (x - 1) + y + 1] = (Cell)
                    {
                        .state = ALIVE,
                        .color = SAND,
                    };
                    set_adjacent_cells_state_if_not_empty(&next_grid, ALIVE, x, y, ALL);
                }
                // bottom right cell is free
                else if (row_exists(x + 1) && grid.cells[grid.cols * (x + 1) + y + 1].state == EMPTY)
                {
                    next_grid.cells[grid.cols * x + y].state = EMPTY;
                    next_grid.cells[grid.cols * (x + 1) + y + 1] = (Cell)
                    {
                        .state = ALIVE,
                        .color = SAND,
                    };
                    set_adjacent_cells_state_if_not_empty(&next_grid, ALIVE, x, y, ALL);
                }
                else
                {
                    next_grid.cells[grid.cols * x + y] = (Cell)
                    {
                        .state = STATIC,
                        .color = STATIC_SAND,
                    };
                    set_adjacent_cells_state_if_not_empty(&next_grid, ALIVE, x, y, ALL);
                }
            }
        }
        else
        {
            next_grid.cells[grid.cols * x + y] = (Cell)
            {
                .state = STATIC,
                .color = STATIC_SAND,
            };
            set_adjacent_cells_state_if_not_empty(&next_grid, ALIVE, x, y, ALL);
        }
    }
}

void set_cell_state_at_if_not_empty(Grid *grid, uint64_t idx, CellState state)
{
    Cell *cell = &grid->cells[idx];
    if (cell->state != EMPTY) cell->state = state;
}

void set_adjacent_cells_state_if_not_empty(Grid* grid, CellState state, int x, int y, uint8_t flags)
{
    if (flags & TOP)
        set_cell_state_at_if_not_empty(grid, grid->cols * x + y - 1, state);
    if (flags & RIGHT)
        set_cell_state_at_if_not_empty(grid, grid->cols * (x + 1) + y, state);
    if (flags & BOTTOM)
        set_cell_state_at_if_not_empty(grid, grid->cols * x + y + 1, state);
    if (flags & LEFT)
        set_cell_state_at_if_not_empty(grid, grid->cols * (x - 1) + y, state);
}
