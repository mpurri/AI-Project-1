class Search_Algorithm {
	int weight;
	int heuristic;
public:
	void findPath(char letterMap[][COL], int xStart, int yStart, int xGoal, int yGoal, int mNum, int inNum);
};

class Uniform: public Search_Algorithm {
	int weight = 0;
	int heuristic = 0;
};

class A_Star : public Search_Algorithm {
	int weight = 0.25;
	int heuristic = 0;
public:
	setHeuristic();
};

class Weighted_A_Star : public A_Star {
	int weight = 0.25;
	int heuristic = 0;
public:
	setHeuristic();
	setWeight();
};