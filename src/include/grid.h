#ifndef _GRID_H_
#define _GRID_H_

void change_grid_size(int new_size);
void draw_cell(int x, int y);
void draw_cells();
void cell_index(int x, int y, int *cell_x, int *cell_y);
int on_grid(int cell_i);
void toggle_cell(int x, int y, int val);
void apply_game_rules(int x, int y);

#endif // _GRID_H_
