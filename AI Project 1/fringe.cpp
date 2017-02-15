#include <iostream>
#include <string> // Naming files
#include <fstream> // Read and write txt files
#include <stdlib.h> // For rand function
#include <math.h>

#include "fringe.h"

Fringe::Fringe()
{
	node * head = new node;
	this->head = nullptr;
	head->prev = head;
}


void Fringe::remove(node * curNode) {
	node * ptr = head;
	if (ptr == curNode) {
		head = head->next;
		head->prev = head;
	}
	else {
		do {
			ptr->prev = ptr;
			ptr = ptr->next;
			if (ptr == curNode) { curNode->prev->next = curNode->next; }
		} while (ptr != nullptr);
	} if (ptr == nullptr) {
		cout << "ERROR no node to remove" << endl;
	}
}
node *Fringe::pop() {
	node * ptr = new node;
	ptr = head;
	if (ptr->next == NULL) {        // only one node on list
		head = head->next;
		return ptr;
	}
	if (ptr->fVal == ptr->next->fVal) {      // if lowest two nodes have the same f val use the one with the highest g val (cost from the start)
		if (ptr->gVal > ptr->next->gVal) {          // first node getting popped

		}
		else {                                   // second node getting popped
			ptr = ptr->next;
			head->next = ptr->next;
			return ptr;
		}
	}
	head = head->next;
	return ptr;

}

void Fringe::insert(node * curNode, double key) {

	node * ptr1 = new node;
	ptr1 = this->head;
	double keyF = curNode->fVal;
	if (ptr1 == NULL) {         // setting the start node
		head = curNode;
		head->next = NULL;
	}
	else {
		node * ptr2 = new node;                    // will be ptr1's prev node
		ptr2 = ptr1;
		while (keyF > ptr1->fVal && ptr1->next != NULL) {    // cycle through to find the the placement of the key or the last node in the queue
			ptr2 = ptr1;
			ptr1 = ptr1->next;

		}
		if (ptr1->next == NULL && keyF > ptr1->fVal) {     // if the current key is the highest in the queue insert the node at the end of the list
			ptr1->next = curNode;
		}
		else if (ptr1 == head) {    // if the current key is the lowest
			curNode->next = head;
			head = curNode;
		}
		else {                       //inserting in to the middle of the queue
			ptr2->next = curNode;
			curNode->next = ptr1;
		}
	}
}
void node::setNode(char ter, int xCord, int yCord, node * goalNode, double w, int heur) {
	char terC = ter;
	switch (ter) {
	case'0':
		cout << "error-blocked cell" << endl;
		break;
	case '1':
		this->ter = 1;
		break;
	case '2':
		this->ter = 2;
		break;
	case 'a':
		this->ter = 0.25;
		break;
	case 'b':
		this->ter = 0.5;
		break;
	}

	this->xCord = xCord;
	this->yCord = yCord;
	calcC();
	calcH(goalNode, w, heur);
	calcG();
	calcF();
}
void node::setStart(char ter, int xCord, int yCord, node * goalNode, double w, int heur) {
	char terC = ter;
	switch (ter) {
	case '0':
		cout << "error-blocked cell on start" << endl;
		break;
	case '1':
		this->ter = 1;
		break;
	case '2':
		this->ter = 2;
		break;
	case 'a':
		this->ter = 0.25;
		break;
	case 'b':
		this->ter = 0.5;
		break;
	}

	this->xCord = xCord;
	this->yCord = yCord;
	calcH(goalNode, w, heur);
	this->gVal = 0;
	calcF();
	cVal = 0;
}

void node::setGoal(char ter, int xCord, int yCord, double w) {
	char terC = ter;
	switch (ter) {
	case'0':
		cout << "error-blocked cell on end" << endl;
		break;
	case '1':
		this->ter = 1;
		break;
	case '2':
		this->ter = 2;
		break;
	case 'a':
		this->ter = 0.25;
		break;
	case 'b':
		this->ter = 0.5;
		break;
	}

	this->xCord = xCord;
	this->yCord = yCord;
	this->hVal = 0;
}
void node::calcG() {               // Sum of costs from start to cur node
	gVal = prev->getG() + getC();
}

double node::getG() {
	return gVal;
}
void node::calcC() {               // cost from prev node to current node

	int xDis = prev->xCord - xCord;
	int yDis = prev->yCord - yCord;
	double sum = xDis*xDis + yDis*yDis;

	double dis = sqrt(sum);

	if (((prev->terC != 'a' || prev->terC != 'b') && (terC != 'a' || terC != 'b')) || (dis == sqrt(2))) {       //diag move or one highway or no highways

		double adjTer = ter;
		double adjTerpr = prev->ter;
		if (prev->terC == 'a') { adjTerpr = 1; }
		else if (prev->terC == 'b') { adjTerpr = 2; }

		if (terC == 'a') { adjTer = 1; }
		else if (terC == 'b') { adjTer = 2; }
		double terMult = (adjTer + adjTerpr) / 2;
		double o = 1.5;
		if (terMult == o && dis == sqrt(2)) {
			cVal = 0.5*(sqrt(2) + sqrt(8));
		}
		else { cVal = terMult*dis; }

	}
	else {
		double terMult = (prev->ter + ter) / 2;
		cVal = terMult*dis;
	}
}

double node::getC() {
	return cVal;
}
void node::calcH(node * goalNode, double w, int heur) {
	if (heur == 1) {                                                                // Distance from goal to current node (1)
		int xDis = goalNode->xCord - xCord;
		int yDis = goalNode->yCord - yCord;
		float sum = xDis*xDis + yDis*yDis;
		hVal = sqrt(sum)*w;
	}
	else if (heur == 2) {                                                            //  Manhattan
		hVal = w*(abs(goalNode->xCord - xCord) + abs(goalNode->yCord - yCord));
	}
	else if (heur == 3) {                                                           //  No Blocked Cells(3)

		int diagM = abs(goalNode->xCord - xCord);
		int horizM = abs(goalNode->xCord - diagM);
		double sum = sqrt(2)*diagM + horizM;
		hVal = w* sum;
	}
	else if (heur == 4) {                                                       //  Horizontal cells are easier to traverse than vertical cells(4)
		int xDis = goalNode->xCord - xCord;
		int yDis = goalNode->yCord - yCord;
		float sum = 0.5*xDis*xDis + yDis*yDis;
		hVal = sqrt(sum)*w;
	}
	else if (heur == 5) {                                                      // Midpoint (5)
		int xDis = goalNode->xCord - xCord;
		int yDis = goalNode->yCord - yCord;
		float sum = xDis*xDis + yDis*yDis;
		float dis = sqrt(sum);
		if (dis >= 50) {
			if (abs(goalNode->xCord - xCord - 50) > abs(goalNode->yCord - yCord - 50)) {
				hVal = abs(goalNode->xCord - xCord - 50)*w;
			}
			else
				hVal = abs(goalNode->yCord - yCord - 50)*w;
		}
		else if (dis >= 25) {
			if (abs(goalNode->xCord - xCord - 25) > abs(goalNode->yCord - yCord - 25)) {
				hVal = abs(goalNode->xCord - xCord - 25)*w;
			}
			else
				hVal = abs(goalNode->yCord - yCord - 25)*w;
		}
		else {
			if (abs(goalNode->xCord - xCord - 5) > abs(goalNode->yCord - yCord - 5)) {
				hVal = abs(goalNode->xCord - xCord - 5)*w;
			}
			else
				hVal = abs(goalNode->yCord - yCord - 5)*w;
		}
	}
	else
		cout << " error incorrect choice for heuristic" << endl;
}

double node::getH() {
	return hVal;
}

void node::calcF() { fVal = hVal + gVal; }

double node::getF() { return fVal; }


bool node::updateVer() {
	if (prev->gVal + cVal < gVal) {
		gVal = prev->gVal + cVal;
		return true;
	}
	return false;
}
