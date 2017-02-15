#include <iostream>
#include <string> // Naming files
#include <fstream> // Read and write txt files
#include <stdlib.h> // For rand function
#include <time.h> // For time in srand function
#include <vector> 
#include <utility> // pair

#include "MapBuild.h"
#include "Coordinate.h"

using namespace std;

void makeMap() {
	for (int m = 1; m < 6; m++) {
		Coordinate map[ROW][COL];

		// Set the (x,y) values for each coordinate
		for (int row = 0; row < ROW; row++) {
			for (int col = 0; col < COL; col++) {
				Coordinate point = map[row][col];
				pair <int, int> p;
				p = make_pair(row, col);
				point.set_location(p);
				map[row][col] = point;
			}
		}

		// Find hard to traverse cells
		srand(time(NULL));
		int data[28][2];
		h2t_cells(map, data);

		// Add highways
		vector<Coordinate> v1, v2, v3, v4;
		int highway = 0;
		int count = 0;
		while (highway <= 1) {
			cout << "Building highway 1..." << endl;
			while (count < 1000) {
				highway = 0;
				if (make_road(map, v1)) {
					add_road(map, v1);
					highway = 1; // 1 highway done
					break;
				}
				count++;
			}
			cout << "Building highway 2..." << endl;
			while (count < 1000) {
				highway = 1;
				if (make_road(map, v2)) {
					add_road(map, v2);
					highway = 2; // 2 highways done
					break;
				}
				count++;
			}
			cout << "Building highway 3..." << endl;
			while (count < 1000) {
				highway = 2;
				if (make_road(map, v3)) {
					add_road(map, v3);
					highway = 3; // 2 highways done
					break;
				}
				count++;
			}
			cout << "Building highway 4..." << endl;
			while (count < 1000) {
				highway = 3;
				if (make_road(map, v4)) {
					add_road(map, v4);
					highway = 4; // 3 highways done
					break;
				}
				count++;
			}
		}

		// Add blocked cells randomly (20% of map)
		add_blocked_cell(map);

		// Add starting and bottom cells
		endpoints(map, data);

		// Save file
		string filename = "Map_";
		filename = filename + to_string(m) + ".txt";
		save_file(map, data, filename);

		cout << "Map complete!" << endl;
	}
}
void openMap(Coordinate map[][COL], char letterMap[][COL], int start_goal[][2], int index, int mapNumber) {

	string filename = "Map_" + to_string(mapNumber) + ".txt";

	ifstream file(filename);
	string line;
	if (file.is_open()) {
		int x = 0;
		while (getline(file, line)) {
			if (x == (0 + index * 2)) { // first line
				file >> start_goal[0][0] >> start_goal[0][1];
			}
			if (x == (1 + index * 2)) { // first line
				file >> start_goal[1][0] >> start_goal[1][1];
			}
			if (x > 28) {
				for (int c = 0; c < COL; c++) {
					letterMap[x - 29][c] = line.at(c);
				}
			}
			// cout << x << ": " << line << endl;
			x++;
		}
	}
	else {
		cout << "File was not opened" << endl;
	}
	file.close();

	for (int row = 0; row < ROW; row++) {
		for (int col = 0; col < COL; col++) {
			Coordinate c = map[row][col];
			pair <int, int> p;
			p = make_pair(row, col);
			c.set_location(p);
			char cell = letterMap[row][col];
			c.set_value(cell);
			map[row][col] = c;
		}
	}
	printf("Successfully opened...Map: %d, Index: %d \n", mapNumber, index);
	printf("Start: (%d,%d), End: (%d,%d) \n", start_goal[0][0], start_goal[0][1], start_goal[1][0], start_goal[1][1]);
}

void h2t_cells(Coordinate map[][COL], int points[][2]) {
	// Select coordinates and put in 2d array
	//int points[9][2];
	for (int i = 20; i < 28; i++) {
		points[i][0] = rand() % ROW;
		points[i][1] = rand() % COL;
		int place = i;
		while (place > 0) {
			place--;
			while (points[i][0] == points[place][1]) {
				points[i][0] = rand() % ROW;
			}
		}
		//printf("Points: (%d,%d)\n", points[i][1], points[i][2]);

		// Place hard to traverse cells
		int left_edge = points[i][0] - 15;
		int right_edge = points[i][0] + 15;
		int top_edge = points[i][1] + 15;
		int bot_edge = points[i][1] - 15;
		if (left_edge < 0) {
			left_edge = 0;
		}
		if (right_edge > ROW - 1) {
			right_edge = ROW - 1;
		}
		if (top_edge > COL - 1) {
			top_edge = COL - 1;
		}
		if (bot_edge < 0) {
			bot_edge = 0;
		}

		for (int hor = left_edge; hor < right_edge; hor++) {
			for (int ver = bot_edge; ver < top_edge; ver++) {
				int coin = rand() % 10;
				if (coin >= 5) {
					Coordinate c = map[hor][ver];
					c.set_value('2');
					map[hor][ver] = c;
				}

			}
		}
	}
}
void save_file(Coordinate map[][COL], int points[][2], string filename) {
	ofstream mapFile;
	mapFile.open(filename); // Create txt file
	mapFile << "  " << endl;
	for (int r = 0; r < 28; r++) {
		mapFile << points[r][0] << " " << points[r][1] << "\n";
	}
	for (int row = 0; row < ROW; row++) { // Add cells to the file
		for (int col = 0; col < COL; col++) {
			Coordinate c = map[row][col];
			char val = c.get_value();
			if (col == COL - 1) { // If there is an end do not add a comma
				mapFile << val << "\n";
			}
			else {
				mapFile << val;
			}
		}
	}
	mapFile.close();
}
bool make_road(Coordinate map[][COL], vector<Coordinate>& vec) {
	vec.clear();
	// Determine whether to start at left or top
	int coin = rand() % 10;
	if (coin >= 0) { // Start at left
		int x_start = rand() % ROW - 1;
		int y_start = 0;

		while (x_start == 0 || x_start == ROW - 1) { // Do not start in corner
			x_start = rand() % ROW - 1;
		}

		// Pop coordinate into vector
		vec.push_back(map[x_start][y_start]);

		// Check to see if next 20 are good
		if (check_map(map, x_start, y_start, x_start, y_start + 20, vec)) {
			// Add coordinates to vector
			pair <int, int> p = add_2_vector(map, x_start, y_start, x_start, y_start + 20, vec);
			vec.pop_back();
			int x_new = p.first;
			int y_new = p.second;
			int x_end = x_new;
			int y_end = y_new;

			if (line_loop(map, x_new, y_new, x_end, y_end, vec, p)) {
				if (check_distance(vec)) {
					return true;
				}
				else {
					// cout << "Too short!" << endl;
					return false;
				}
			}
			else {
				return false;
			}
		}
	}
}
bool line_loop(Coordinate map[][COL], int x_new, int y_new, int x_end, int y_end, vector<Coordinate>& vec, pair <int, int> p) {
	while (boundary(map, x_new, y_new, x_end, y_end) == 'X') {
		char direct = turn_road(map, x_new, y_new, vec);
		if (direct == 'E') {
			if (y_end + 20 > COL - 1) { // Check bounds
				y_end = COL - 1;
			}
			else { // In bounds add 20
				y_end = y_end + 20;
			}

			if (check_map(map, x_new, y_new, x_end, y_end, vec)) {
				// cout << "East" << endl;
				p = add_2_vector(map, x_new, y_new, x_end, y_end, vec);
				y_new = y_end;
			}
			else {
				return false;
			}
		}
		if (direct == 'W') {
			if (y_end - 20 < 0) {
				y_end = 0;
			}
			else {
				y_end = y_end - 20;
			}
			if (check_map(map, x_new, y_new, x_end, y_end, vec)) {
				// cout << "West" << endl;
				p = add_2_vector(map, x_new, y_new, x_end, y_end, vec);
				y_new = y_end;
			}
			else {
				return false;
			}
		}
		if (direct == 'S') {
			if (x_end + 20 > ROW - 1) {
				x_end = ROW - 1;
			}
			else {
				x_end = x_end + 20;
			}
			if (check_map(map, x_new, y_new, x_end, y_end, vec)) {
				// cout << "South" << endl;
				p = add_2_vector(map, x_new, y_new, x_end, y_end, vec);
				x_new = x_end;
			}
			else {
				return false;
			}
		}
		if (direct == 'N') {
			if (x_end - 20 < 0) {
				x_end = 0;
			}
			else {
				x_end = x_end - 20;
			}
			if (check_map(map, x_new, y_new, x_end, y_end, vec)) {
				// cout << "North" << endl;
				p = add_2_vector(map, x_new, y_new, x_end, y_end, vec);
				x_new = x_end;
			}
			else {
				return false;
			}
		}
	}
	return true;
}
bool check_map(Coordinate map[][COL], int x_start, int y_start, int x_end, int y_end, vector<Coordinate>& vec) {
	if (x_start == x_end) {
		if (y_start < y_end) {
			for (int col = y_start; col < y_end; col++) {
				Coordinate c = map[x_start][col];
				char cell = c.get_value();
				if (cell == 'a' || cell == 'b') {
					return false;
				}
				if (find_location_in_vector(vec, c.get_location()) == false) {
					return false;
				}
			}
		}
		else {
			for (int col = y_start; col > y_end; col--) {
				Coordinate c = map[x_start][col];
				char cell = c.get_value();
				if (cell == 'a' || cell == 'b') {
					return false;
				}
				if (find_location_in_vector(vec, c.get_location()) == false) {
					return false;
				}
			}
		}
	}
	else {
		if (x_start < x_end) {
			for (int row = x_start; row < x_end; row++) {
				Coordinate c = map[row][y_start];
				char cell = c.get_value();
				if (cell == 'a' || cell == 'b') {
					return false;
				}
				if (find_location_in_vector(vec, c.get_location()) == false) {
					return false;
				}
			}
		}
		else {
			for (int row = x_start; row > x_end; row--) {
				Coordinate c = map[row][y_start];
				char cell = c.get_value();
				if (cell == 'a' || cell == 'b') {
					return false;
				}
				if (find_location_in_vector(vec, c.get_location()) == false) {
					return false;
				}
			}
		}
	}
	return true;
}
char turn_road(Coordinate map[][COL], int x_start, int y_start, vector<Coordinate>& vec) {
	// Determine which direction it came from
	//Coordinate c = vec.end()[-2]; // Second to last element
	Coordinate c = vec.back();
	pair <int, int> p = c.get_location();

	// Probability
	int coin = rand() % 10;

	if (p.first - x_start == 1) { // came from south
								  // Only can go left, up, or right
		if (coin < 6) { // Go straight
			return 'N';
		}
		else if (coin < 8 && coin > 5) {
			return 'E';
		}
		else {
			return 'W';
		}
	}
	if (p.first - x_start == -1) { // came from north
								   // Only can go down, right, left
		if (coin < 6) { // Go straight
			return 'S';
		}
		else if (coin < 8 && coin > 5) {
			return 'E';
		}
		else {
			return 'W';
		}
	}
	if (p.second - y_start == 1) { // came from east
								   // Only can go left, up, down
		if (coin < 6) { // Go straight
			return 'W';
		}
		else if (coin < 8 && coin > 5) {
			return 'N';
		}
		else {
			return 'S';
		}
	}
	if (p.second - y_start == -1) { // came from west
									// Only can go right, up, down
		if (coin < 6) { // Go straight
			return 'E';
		}
		else if (coin < 8 && coin > 5) {
			return 'N';
		}
		else {
			return 'S';
		}
	}
}
char boundary(Coordinate map[][COL], int x_start, int y_start, int x_end, int y_end) {
	if (x_start >= ROW - 1) { // Hit bottom of map
		return 'S';
	}
	if (x_start <= 0) { // Hit top of map
		return 'N';
	}
	if (y_start >= COL - 1) { // Hit right of map
		return 'E';
	}
	if (y_start <= 0) { // Hit left of map
		return 'W';
	}
	return 'X';
}
pair <int, int> add_2_vector(Coordinate map[][COL], int x_start, int y_start, int x_end, int y_end, vector<Coordinate>& vec) {
	if (x_start == x_end) { // Change in y (left or right)
		if (y_start > y_end) {
			for (int row = y_start; row > y_end; row--) {
				vec.push_back(map[x_start][row]);
			}
		}
		else {
			for (int row = y_start; row < y_end; row++) {
				vec.push_back(map[x_start][row]);
			}
		}

	}
	else { // Change in x (up or down)
		if (x_start > x_end) {
			for (int col = x_start; col > x_end; col--) {
				vec.push_back(map[col][y_start]);
			}
		}
		else {
			for (int col = x_start; col < x_end; col++) {
				vec.push_back(map[col][y_start]);
			}
		}

	}
	Coordinate c = vec.back();
	pair <int, int> p;
	p = c.get_location();
	return p;
}
void add_road(Coordinate map[][COL], vector<Coordinate>& vec) {
	Coordinate last = vec.back();
	pair <int, int> last_p = last.get_location();
	if (last_p.first == 1) { // add last element
		Coordinate end;
		pair <int, int> end_p;
		end_p = make_pair(0, last_p.second);
		end.set_location(end_p);
		vec.push_back(end);
	}
	if (last_p.first == ROW - 2) {
		Coordinate end;
		pair <int, int> end_p;
		end_p = make_pair(ROW - 1, last_p.second);
		end.set_location(end_p);
		vec.push_back(end);
	}
	if (last_p.second == 1) {
		Coordinate end;
		pair <int, int> end_p;
		end_p = make_pair(last_p.first, 0);
		end.set_location(end_p);
		vec.push_back(end);
	}
	if (last_p.second == COL - 2) {
		Coordinate end;
		pair <int, int> end_p;
		end_p = make_pair(last_p.first, COL - 1);
		end.set_location(end_p);
		vec.push_back(end);
	}
	while (!vec.empty()) {
		// Get locations
		Coordinate c = vec.back();
		pair <int, int> p;
		p = c.get_location();

		if (p.first > ROW || p.second > COL || p.first < 0 || p.second < 0) {
			break;
		}
		//printf("Points: (%d,%d)\n", p.first, p.second);

		// Get value at old position
		Coordinate old = map[p.first][p.second];
		char cell = old.get_value();

		// Alter old value
		if (cell == '1') {
			old.set_value('a');
		}
		if (cell == '2') {
			old.set_value('b');
		}

		// Set coordinate to new value
		map[p.first][p.second] = old;

		// Pop off value in vector
		vec.pop_back();
	}
}
void add_blocked_cell(Coordinate map[][COL]) {
	int count = 0;
	int x, y;
	char val;
	while (count < 3840) {
		x = rand() % ROW;
		y = rand() % COL;
		val = map[x][y].get_value();
		if (val == 'a' || val == 'b') { // Skip its a highway

		}
		else { // Change to blocked
			map[x][y].set_value('0');
			count++;
		}
	}
}
bool find_location_in_vector(vector<Coordinate>& vec, pair <int, int> p) {

	for (int i = 0; i < vec.size() - 1; i++) {
		pair <int, int> it = vec[i].get_location();
		if (it == p) {
			// cout << "Ran over itself!" << endl;
			return false;
		}
	}

	return true;
}
bool check_distance(vector<Coordinate>& vec) {
	Coordinate f = vec.front();
	Coordinate b = vec.back();
	pair <int, int> f_loc = f.get_location();
	pair <int, int> b_loc = b.get_location();

	float dist = sqrt(pow((f_loc.first - b_loc.first), 2) + pow((f_loc.second - b_loc.second), 2));
	// printf("Distance: %f \n", dist);
	if (dist < 100) {
		return false;
	}
	return true;
}
void endpoints(Coordinate map[][COL], int goals[][2]) {
	for (int i = 0; i < 10; i++) {
		// Use first and last 20 columns
		int start_x = rand() % ROW;
		int start_y = rand() % 20;
		int goal_x = rand() % ROW;
		int goal_y = COL - 1 - (rand() % 20);

		float dist = sqrt(pow((start_x - goal_x), 2) + pow((start_y - goal_y), 2));

		while (dist < 100 || checkVertices(map,start_x,start_y,goal_x,goal_y) == false) { // try new points
			start_x = rand() % ROW;
			start_y = rand() % 20;
			goal_x = rand() % ROW;
			goal_y = COL - 1 - (rand() % 20);
			dist = sqrt(pow((start_x - goal_x), 2) + pow((start_y - goal_y), 2));
		}

		// Add vertices to array for making file
		goals[0 + i*2][0] = start_x;
		goals[0 + i*2][1] = start_y;
		goals[1 + i*2][0] = goal_x;
		goals[1 + i*2][1] = goal_y;
		printf("Start: (%d,%d), Goal: (%d,%d), Distance: %f \n", start_x, start_y, goal_x, goal_y, dist);
	}
}

bool checkVertices(Coordinate map[][COL], int s_x, int s_y, int e_x, int e_y) {
	Coordinate s_c = map[s_x][s_y];
	char s_cell = s_c.get_value();
	if (s_cell == '0' || s_cell == 'a' || s_cell == 'b') {
		// cout << "Start vertex is bad...\n";
		return false;
	}
	Coordinate e_c = map[e_x][e_y];
	char e_cell = e_c.get_value();
	if (e_cell == '0' || e_cell == 'a' || e_cell == 'b') {
		// cout << "End vertex is bad...\n";
		return false;
	}
	return true;
}