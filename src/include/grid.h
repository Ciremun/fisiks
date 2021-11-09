#ifndef _GRID_H_
#define _GRID_H_

#define DEFAULT_CELL_SIZE 32

#define GRID_SIZE(gs) (sizeof(Cell) * grid.rows * grid.cols)

typedef enum {
    EMPTY = 0,
    ALIVE,
    STATIC,
} CellState;

typedef struct {
    uint32_t color;
    CellState state;
} Cell;

typedef struct {
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
void toggle_cell(CellState state, int x, int y);
void apply_game_rules(int x, int y);

#endif // _GRID_H_
