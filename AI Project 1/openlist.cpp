#include <iostream>
#include <string> // Naming files
#include <fstream> // Read and write txt files
#include <stdlib.h> // For rand function
#include <math.h>
#include "openlist.h"
#include "fringe.h"



OpenList::OpenList()
{
	node * headI = new node;
	this->headI = nullptr;
};



void OpenList::insertI(node * curI)
{
	node * ptr = new node;
	ptr = this->headI;
	if (ptr == nullptr) {
		this->headI = curI;
		return;
	}
	while (ptr->nextI != nullptr) {
		ptr = ptr->nextI;
	};
	ptr->nextI = curI;
	return;
}

bool OpenList::isOn(node * curI) {
	node * ptr = new node;
	ptr = this->headI;
	while (ptr != nullptr) {
		if (ptr->xCord == curI->xCord &&ptr->yCord == curI->yCord) {
			return true;
		}
		ptr = ptr->nextI;
	};
	return false;

};

node* OpenList::findI(node * curI) {
	node * ptr = new node;
	ptr = this->headI;
	do {
		if (ptr->xCord == curI->xCord &&ptr->yCord == curI->yCord) {
			return ptr;
		}
		ptr = ptr->nextI;
	} while (ptr != nullptr);
	cout << " error: says its on the OL but cant find it" << endl;
	return NULL;
}

void node::setI(int x, int y, node * prevI)
{
	this->prev = prevI;
	this->xCord = x;
	this->yCord = y;
	this->nextI = NULL;
}
