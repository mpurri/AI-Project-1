#include <iostream>
#include <fstream> // Read and write txt files
#include <stdlib.h> // For rand function
#include <vector> 
#include <utility> // pair

#define ROW 120
#define COL 160

class Search_Algorithm {
	double weight;
	int heuristic;
public:
	void findPath(char letterMap[][COL], int xStart, int yStart, int xGoal, int yGoal, int mNum, int inNum);
};

class Uniform : public Search_Algorithm {
	double weight = 0;
	int heuristic = 0;
};

class A_Star : public Search_Algorithm {
	double weight = 0.25;
	int heuristic = 0;
public:
	void setHeuristic(int heur);
};

class Weighted_A_Star : public A_Star {
	double weight = 0.25;
	int heuristic = 0;
public:
	void setWeight(int wei);
};