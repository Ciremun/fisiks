#ifndef _GRID_H_
#define _GRID_H_

#define DEFAULT_CELL_SIZE 32

#define GRID_SIZE(gs) (sizeof(Cell) * grid.rows * grid.cols)

typedef enum
{
    EMPTY = 0,
    ALIVE,
    STATIC,
} CellState;

typedef enum
{
    TOP = 1 << 0,
    RIGHT = 1 << 1,
    BOTTOM = 1 << 2,
    LEFT = 1 << 3,
    ALL = (1 << 8) - 1,
} Side;

typedef struct
{
    uint32_t color;
    CellState state;
} Cell;

typedef struct
{
    int rows;
    int cols;
    Cell *cells;
} Grid;

void change_grid_size(int new_rows, int new_cols);
void draw_cell(Cell cell, int x, int y);
void draw_cells();
void cell_index(int x, int y, int *cell_x, int *cell_y);
int row_exists(int cell_y);
int col_exists(int cell_x);
void toggle_cell(CellState state, int x, int y, uint32_t color);
void apply_game_rules(int x, int y);
void set_adjacent_cells_state_if_not_empty(Grid* grid, CellState state, int x, int y, uint8_t flags);
void set_cell_state_at_if_not_empty(Grid *grid, uint64_t idx, CellState state);

#endif // _GRID_H_
