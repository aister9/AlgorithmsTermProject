#pragma once
#include <stack>
#include <queue>
#include "Maze.h"
#include <Windows.h>
#include <list>

template<class T>
bool search_list(list<T> list, T item) {
	for (T t : list) {
		if (t == item) return true;
	}
	return false;
}

class Mouse {
private:
	int energy;
	double mana;
	int stepNum;
	mPoint curPos;
	list<mPoint> visitPos;
	Maze mazeData;
	int direction = 0;
public:
	Mouse(const int &energy);
	void moveToPos(const mPoint& targetPoint);
	void step();
	
	void initMazeData(const int& mazeSize);
	void setMazeData(const Maze &md) ;
	void visionMazeData(const Maze& md);

	bool search(const Maze& md) ;

	friend ostream& operator<<(ostream& os, const Mouse& m);
};

ostream& operator<<(ostream& os, const Mouse& m) {
	system("cls");
	os << " Mouse Property - energy : " << m.energy << " / mana - " << m.mana << " / Current Steps - " << m.stepNum << " / Remaining energy - " << m.energy - m.stepNum<<" / Current Position - " << m.curPos << endl;
	cout << m.mazeData;
	return os;
}

Mouse::Mouse(const int &energy):energy(energy){
	mana = 0.0;
	stepNum = 0;
	curPos = mPoint(1, 0);
}

void Mouse::initMazeData(const int& mazeSize) {
	memset(mazeData.getMap(), -1, mazeData.getMapSize() * sizeof(int));
}
void Mouse::setMazeData(const Maze& md)  {
	mazeData = Maze(md.getMap(), md.getN(), md.getM());
	initMazeData(mazeData.getMapSize());
}
void Mouse::visionMazeData(const Maze& md)  {
	int x = curPos.getX(); int y = curPos.getY();
	if (mazeData.getMapDataPos(curPos) != 'p') {
		mazeData.getMap()[y * mazeData.getN() + x] = 'p';
	}
	if (!mazeData.canView(mPoint(x-1, y)) && md.isValid(x-1, y)) {
		mazeData.getMap()[y * mazeData.getN() + (x-1)] = md.getMapDataPos(mPoint(x - 1, y));
	}
	if (!mazeData.canView(mPoint(x + 1, y)) && md.isValid(x+1, y)) {
		mazeData.getMap()[y * mazeData.getN() + (x+1)] = md.getMapDataPos(mPoint(x + 1, y));
	}
	if (!mazeData.canView(mPoint(x, y-1)) && md.isValid(x, y-1)) {
		mazeData.getMap()[(y-1) * mazeData.getN() + x] = md.getMapDataPos(mPoint(x , y-1));
	}
	if (!mazeData.canView(mPoint(x, y+1)) && md.isValid(x, y+1)) {
		mazeData.getMap()[(y+1) * mazeData.getN() + x] = md.getMapDataPos(mPoint(x, y+1));
	}
}

void Mouse::step() {
	stepNum++;
	mana += 0.1;
}

void Mouse::moveToPos(const mPoint& targetPoint) {
	visitPos.push_back(curPos);
	curPos.setXY(targetPoint);
	step();
}


bool Mouse::search(const Maze& md)  {
	this->setMazeData(md);
	visionMazeData(md);
	visitPos.push_back(curPos);

	if (energy - stepNum < 1)return false;
	else {
		while (!visitPos.empty()) {
			int xPos = curPos.getX(); int yPos = curPos.getY();
			switch (direction) {
			case 0:
				if (mazeData.getMapDataPos(mPoint(xPos+1, yPos)) == 0 && !search_list(visitPos, mPoint(xPos+1, yPos))) {
					moveToPos(mPoint(xPos + 1, yPos));
				}
				else {
					if(!search_list(visitPos, mPoint(xPos, yPos+1)))direction = 1;
					else if (!search_list(visitPos, mPoint(xPos-1, yPos)))direction = 2;
					else if (!search_list(visitPos, mPoint(xPos, yPos-1)))direction = 3;
				}
				break;
			case 1:
				if (mazeData.getMapDataPos(mPoint(xPos, yPos+1)) == 0 && !search_list(visitPos, mPoint(xPos, yPos+1))) {
					moveToPos(mPoint(xPos, yPos+1));
					}
				   else {
					if (!search_list(visitPos, mPoint(xPos - 1, yPos)))direction = 2;
					else if (!search_list(visitPos, mPoint(xPos, yPos - 1)))direction = 3;
					else if (!search_list(visitPos, mPoint(xPos - 1, yPos)))	direction = 0;
					}
				break;
			case 2:
				if (mazeData.getMapDataPos(mPoint(xPos-1, yPos)) == 0 && !search_list(visitPos, mPoint(xPos-1, yPos))) {
					moveToPos(mPoint(xPos - 1, yPos));
				}
				else {
					direction = 3;
				}
				break;
			case 3:
				if (mazeData.getMapDataPos(mPoint(xPos, yPos-1)) == 0 && !search_list(visitPos, mPoint(xPos, yPos-1))) {
					moveToPos(mPoint(xPos, yPos-1));
					}
				else {
					visitPos.pop_back();
					direction=0;
				}
				break;
			}
			visionMazeData(md);
			cout << *this;
			cout << visitPos.back() << direction;
		}
	}
}
