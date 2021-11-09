#ifndef _GRID_H_
#define _GRID_H_

#define ALIVE 1
#define DEAD 0
#define DEFAULT_CELL_SIZE 32

#define GRID_SIZE(gs) (sizeof(int) * grid.rows * grid.cols)

void change_grid_size(int new_rows, int new_cols);
void draw_cell(int x, int y);
void draw_cells();
void cell_index(int x, int y, int *cell_x, int *cell_y);
int row_exists(int cell_x);
int col_exists(int cell_y);
void toggle_cell(int x, int y, int val);
void apply_game_rules(int x, int y);

typedef struct {
    int rows;
    int cols;
    int *data;
} Grid;

#endif // _GRID_H_
