#pragma once


#include <iostream>
#include <string> // Naming files
#include <fstream> // Read and write txt files
#include <stdlib.h> // For rand function
# include "fringe.h"
using namespace std;


class OpenList {
public:
	OpenList();
	void insertI(node * curI);
	bool isOn(node * curI);
	node * findI(node * curI);
	node * headI;

};
