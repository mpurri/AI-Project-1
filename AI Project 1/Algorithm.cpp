#include "fringe.h"
#include "openlist.h"
#include "Algorithm.h"

#include <iostream>
#include <fstream> // Read and write txt files
#include <stdlib.h> // For rand function
#include <vector> 
#include <utility> // pair

#define ROW 120
#define COL 160

void Search_Algorithm::findPath(char letterMap[][COL], int xStart, int yStart, int xGoal, int yGoal, int mNum, int inNum) {
	int heur = heuristic;
	double we = weight;
	Fringe * fringe = new Fringe;
	node * startNode = new node;
	node * goalNode = new node;
	OpenList * ol = new OpenList;
	goalNode->setGoal(letterMap[xGoal][yGoal], xGoal, yGoal, we);
	startNode->setStart(letterMap[xStart][yStart], xStart, yStart, goalNode, we, heur);
	startNode->gVal = 0;
	startNode->calcH(goalNode, we, heur);
	startNode->prev = startNode;
	node * rtI = new node;
	rtI->setI(startNode->xCord, startNode->yCord, startNode);
	ol->insertI(rtI);
	fringe->insert(startNode, startNode->getG() + startNode->getH());
	node * curS = new node;
	cout << "Finding Path..." << endl;
	while (fringe != nullptr) {
		curS = fringe->pop();
		// cout << " current node-> x-cord: " << curS->xCord << " y-coord: " << curS->yCord << endl;
		if (curS->xCord == goalNode->xCord && curS->yCord == goalNode->yCord) {

			goalNode->prev = curS;
			goalNode->calcC();
			goalNode->calcG();
			goalNode->calcF();
			node * ptr = goalNode->prev;
			ptr->calcH(goalNode, we, heur);
			/*cout << "**path found**" << endl;
			cout << "(" << goalNode->xCord << ", " << goalNode->yCord <<") f: "<< goalNode->fVal<<" g: "<< goalNode->gVal<<endl;
			do {
			ptr = ptr->prev;
			cout << "(" << ptr->xCord << ", " << ptr->yCord << ") f: " << ptr->fVal << " g: " << ptr->gVal << endl;

			} while (ptr != startNode);*/
			cout << "** path found ! **" << endl;
			cout << "Printing to pathfile..." << endl;

			ofstream pathFile;
			string pfile = "path_" + to_string(mNum) + '_' + to_string(inNum) + '_' + to_string(we) + '_' + to_string(heur);
			pathFile.open(pfile + ".txt");
			pathFile << goalNode->xCord << " " << goalNode->yCord << " " << goalNode->fVal << " " << goalNode->gVal << " 0" << endl;
			do {
				ptr = ptr->prev;
				pathFile << ptr->xCord << " " << ptr->yCord << " " << ptr->fVal << " " << ptr->gVal << " " << ptr->hVal << endl;

			} while (ptr != startNode);
			pathFile.close();
			cout << "Path File created." << endl;

			node * ptr1 = fringe->head;
			ptr1->calcH(goalNode, we, heur);
			ofstream fringeFile;
			string ffile = "fringe_" + to_string(mNum) + '_' + to_string(inNum) + '_' + to_string(we) + '_' + to_string(heur);;
			fringeFile.open(ffile + ".txt");
			cout << "Printing to fringefile..." << endl;
			fringeFile << ptr1->xCord << " " << ptr1->yCord << " " << ptr1->fVal << " " << ptr1->gVal << " " << ptr1->hVal << endl;
			while (ptr1->next != NULL) {
				ptr1 = ptr1->next;
				fringeFile << ptr1->xCord << " " << ptr1->yCord << " " << ptr1->fVal << " " << ptr1->gVal << " " << ptr1->hVal << endl;

			};
			fringeFile.close();
			cout << "Fringe File created." << endl;

			node * ptr2 = ol->headI;
			ptr2->calcH(goalNode, we, heur);
			ofstream olFile;
			string olfile = "openlist_" + to_string(mNum) + '_' + to_string(inNum) + '_' + to_string(we) + '_' + to_string(heur);;
			olFile.open(olfile + ".txt");
			cout << "Printing to openlist file..." << endl;
			olFile << ptr2->prev->xCord << " " << ptr2->prev->yCord << " " << ptr2->prev->fVal << " " << ptr2->prev->gVal << " " << ptr2->prev->hVal << endl;
			while (ptr2->nextI != NULL) {
				ptr2 = ptr2->nextI;
				ptr2->calcH(goalNode, we, heur);
				olFile << ptr2->prev->xCord << " " << ptr2->prev->yCord << " " << ptr2->prev->fVal << " " << ptr2->prev->gVal << " " << ptr2->prev->hVal << endl;

			};
			olFile.close();
			cout << "Open list File created." << endl;

			cout << "Path Finder closing." << endl;
			return;
		}
		else {
			if (letterMap[curS->xCord + 1][curS->yCord] != '0' &&curS->xCord + 1 <= ROW - 1) {                                        // |--
				node * rtI = new node;
				rtI->setI(curS->xCord + 1, curS->yCord, curS);
				if (!ol->isOn(rtI)) {
					node * rtNode = new node;
					rtNode->prev = curS;
					rtI->prev = curS;
					rtNode->setNode(letterMap[curS->xCord + 1][curS->yCord], curS->xCord + 1, curS->yCord, goalNode, we, heur);
					ol->insertI(rtI);
					fringe->insert(rtNode, rtNode->getG() + rtNode->getH());
				}
				else {
					if (rtI->updateVer()) {
						fringe->remove(rtI);
						fringe->insert(rtI, rtI->getG() + rtI->getH());
					}

				}
			}
			if (letterMap[curS->xCord + 1][curS->yCord - 1] != '0'&&curS->xCord + 1 <= ROW - 1 && curS->yCord - 1 >= 0) {                                     //  "\_"
				node * rtdwI = new node;
				rtdwI->setI(curS->xCord + 1, curS->yCord - 1, curS);
				if (!ol->isOn(rtdwI)) {
					node * rtdwNode = new node;
					rtdwNode->prev = curS;
					rtdwI->prev = curS;
					rtdwNode->setNode(letterMap[curS->xCord + 1][curS->yCord - 1], curS->xCord + 1, curS->yCord - 1, goalNode, we, heur);
					ol->insertI(rtdwI);
					fringe->insert(rtdwNode, rtdwNode->getG() + rtdwNode->getH());
				}
				else {
					if (rtdwI->updateVer()) {
						fringe->remove(rtdwI);
						fringe->insert(rtdwI, rtdwI->getG() + rtdwI->getH());
					}
				}
			}

			if (letterMap[curS->xCord][curS->yCord - 1] != '0' && curS->yCord - 1 >= 0) {                                       // |_
				node * dwI = new node;
				dwI->setI(curS->xCord, curS->yCord - 1, curS);
				if (!ol->isOn(dwI)) {
					node * dwNode = new node;
					dwNode->prev = curS;
					dwI->prev = curS;
					dwNode->setNode(letterMap[curS->xCord][curS->yCord - 1], curS->xCord, curS->yCord - 1, goalNode, we, heur);
					ol->insertI(dwI);
					fringe->insert(dwNode, dwNode->getG() + dwNode->getH());
				}
				else {
					if (dwI->updateVer()) {
						fringe->remove(dwI);
						fringe->insert(dwI, dwI->getG() + dwI->getH());
					}
				}
			}

			if (letterMap[curS->xCord - 1][curS->yCord - 1] != '0'&&curS->xCord - 1 >= 0 && curS->yCord - 1 >= 0) {                                   // _/
				node * lfdwI = new node;
				lfdwI->setI(curS->xCord - 1, curS->yCord - 1, curS);
				if (!ol->isOn(lfdwI)) {
					node * lfdwNode = new node;
					lfdwNode->prev = curS;
					lfdwI->prev = curS;
					lfdwNode->setNode(letterMap[curS->xCord - 1][curS->yCord - 1], curS->xCord - 1, curS->yCord - 1, goalNode, we, heur);
					ol->insertI(lfdwI);
					fringe->insert(lfdwNode, lfdwNode->getG() + lfdwNode->getH());

				}
				else {
					if (lfdwI->updateVer()) {
						fringe->remove(lfdwI);
						fringe->insert(lfdwI, lfdwI->getG() + lfdwI->getH());
					}
				}
			}

			if (letterMap[curS->xCord - 1][curS->yCord] != '0'&&curS->xCord - 1 >= 0) {                                      // --|.
				node * lfI = new node;
				lfI->setI(curS->xCord - 1, curS->yCord, curS);
				if (!ol->isOn(lfI)) {
					node * lfNode = new node;
					lfNode->prev = curS;
					lfI->prev = curS;
					lfNode->setNode(letterMap[curS->xCord - 1][curS->yCord], curS->xCord - 1, curS->yCord, goalNode, we, heur);
					ol->insertI(lfI);
					fringe->insert(lfNode, lfNode->getG() + lfNode->getH());

				}
				else {
					if (lfI->updateVer()) {
						fringe->remove(lfI);
						fringe->insert(lfI, lfI->getG() + lfI->getH());
					}
				}
			}

			if (letterMap[curS->xCord - 1][curS->yCord + 1] != '0'&& curS->xCord - 1 >= 0 && curS->yCord + 1 <= COL - 1) {                             // --"\"
				node * lftpI = new node;
				lftpI->setI(curS->xCord - 1, curS->yCord + 1, curS);
				if (!ol->isOn(lftpI)) {
					node * lftpNode = new node;
					lftpNode->prev = curS;
					lftpI->prev = curS;
					lftpNode->setNode(letterMap[curS->xCord - 1][curS->yCord + 1], curS->xCord - 1, curS->yCord + 1, goalNode, we, heur);
					ol->insertI(lftpI);
					fringe->insert(lftpNode, lftpNode->getG() + lftpNode->getH());

				}
				else {
					if (lftpI->updateVer()) {
						fringe->remove(lftpI);
						fringe->insert(lftpI, lftpI->getG() + lftpI->getH());
					}
				}
			}

			if (letterMap[curS->xCord][curS->yCord + 1] != '0'&& curS->yCord + 1 <= COL - 1) {                            //  ^|^
				node * tpI = new node;
				tpI->setI(curS->xCord, curS->yCord + 1, curS);
				if (!ol->isOn(tpI)) {
					node * tpNode = new node;
					tpNode->prev = curS;
					tpI->prev = curS;
					tpNode->setNode(letterMap[curS->xCord][curS->yCord + 1], curS->xCord, curS->yCord + 1, goalNode, we, heur);
					ol->insertI(tpI);
					fringe->insert(tpNode, tpNode->getG() + tpNode->getH());

				}
				else {
					if (tpI->updateVer()) {
						fringe->remove(tpI);
						fringe->insert(tpI, tpI->getG() + tpI->getH());
					}
				}
			}

			if (letterMap[curS->xCord + 1][curS->yCord + 1] != '0'&&curS->xCord + 1 <= ROW - 1 && curS->yCord + 1 <= COL - 1) {                                  // /--
				node * rttpI = new node;
				rttpI->setI(curS->xCord + 1, curS->yCord + 1, curS);
				if (!ol->isOn(rttpI)) {
					node * rttpNode = new node;
					rttpNode->prev = curS;
					rttpI->prev = curS;
					rttpNode->setNode(letterMap[curS->xCord + 1][curS->yCord + 1], curS->xCord + 1, curS->yCord + 1, goalNode, we, heur);
					ol->insertI(rttpI);
					fringe->insert(rttpNode, rttpNode->getG() + rttpNode->getH());
				}
				else {
					if (rttpI->updateVer()) {
						fringe->remove(rttpI);
						fringe->insert(rttpI, rttpI->getG() + rttpI->getH());
					}
				}
			}
			//				cout << "expanded" << endl;
			/*node * ptr = fringe->head;
			cout << "**current fringe**" << endl;
			do {
			cout << " node-> " << ptr->xCord << ", " << ptr->yCord << " f value: "<< ptr->fVal << endl;
			ptr = ptr->next;
			} while (ptr->next != NULL);*/
		}
	}

	cout << "No path found" << endl;
	return;
};

void A_Star::setHeuristic(int heur) {
	A_Star::heuristic = heur;
}

void Weighted_A_Star::setWeight(int wei) {
	Weighted_A_Star::weight = wei;
}
