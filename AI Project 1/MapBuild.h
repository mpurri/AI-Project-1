#pragma once

#include <vector>
#include <iostream>

#define ROW 120
#define COL 160

#include "Coordinate.h"


void h2t_cells(Coordinate map[][COL], int points[][2]);
void save_file(Coordinate map[][COL], int points[][2], string filename);
bool make_road(Coordinate map[][COL], vector<Coordinate>& vec);
bool check_map(Coordinate map[][COL], int x_start, int y_start, int x_end, int y_end, vector<Coordinate>& vec);
pair <int, int> add_2_vector(Coordinate map[][COL], int x_start, int y_start, int x_end, int y_end, vector<Coordinate>& vec);
char turn_road(Coordinate map[][COL], int x_start, int y_start, vector<Coordinate>& vec);
void add_road(Coordinate map[][COL], vector<Coordinate>& vec);
char boundary(Coordinate map[][COL], int x_start, int y_start, int x_end, int y_end);
bool line_loop(Coordinate map[][COL], int x_new, int y_new, int x_end, int y_end, vector<Coordinate>& vec, pair <int, int> p);
void add_blocked_cell(Coordinate map[][COL]);
bool find_location_in_vector(vector<Coordinate>& vec, pair <int, int> p);
bool check_distance(vector<Coordinate>& vec);
void endpoints(Coordinate map[][COL], int goals[][2]);
void makeMap();

void openMap(Coordinate map[][COL], char letterMap[][COL], int start_goal[][2], int index, int mapNumber);
bool checkVertices(Coordinate map[][COL], int s_x, int s_y, int e_x, int e_y);