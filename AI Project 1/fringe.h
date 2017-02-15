#pragma once

//#ifndef Fringe_h

#include <iostream>
#include <string> // Naming files
#include <fstream> // Read and write txt files
#include <stdlib.h> // For rand function

using namespace std;

struct node {
public:
	float ter;  // Can be '0'- blocked '1'-regular '2'-hard 'a'-regular w/ HGWY 'b'-hard w/ HGWY
	char terC;
	int xCord; // Horiontal position in grid
	int yCord; // Vertical position in grid
	double gVal;
	double hVal;
	double fVal;
	double cVal;
	node* next = NULL;
	node* prev = NULL;
	node * nextI = NULL;


	void setNode(char ter, int xCord, int yCord, node * goalNode, double w, int heur);
	void setStart(char ter, int xCord, int yCord, node * goalNode, double w, int heur);
	void setGoal(char ter, int xCord, int yCord, double w);
	void calcG();
	double getG();
	void calcC();
	double getC();
	void calcH(node * goalNode, double w, int heur);
	double getH();
	void calcF();
	double getF();
	bool node::updateVer();

	void setI(int x, int y, node * prevI);

};

class Fringe
{

public:
	node *head;
	double w;
	Fringe();
	void remove(node * curNode);
	node * pop();
	void insert(node * curNode, double key);
	~Fringe();
};
#pragma once
