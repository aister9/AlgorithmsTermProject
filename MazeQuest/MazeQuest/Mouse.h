#pragma once
#include <stack>
#include <queue>
#include "Maze.h"
#include <Windows.h>

class Mouse {
private:
	int energy;
	double mana;
	int stepNum;
	mPoint curPos;
	stack<mPoint> visitPos;
	Maze mazeData;
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
	if (mazeData.getMapDataPos(curPos) == -1) {
		mazeData.getMap()[y * mazeData.getN() + x] = md.getMapDataPos(curPos);
	}
	if (mazeData.getMapDataPos(mPoint(x-1, y)) == -1 && md.isValid(x-1, y)) {
		mazeData.getMap()[y * mazeData.getN() + (x-1)] = md.getMapDataPos(mPoint(x - 1, y));
	}
	if (mazeData.getMapDataPos(mPoint(x+1, y)) == -1 && md.isValid(x+1, y)) {
		mazeData.getMap()[y * mazeData.getN() + (x+1)] = md.getMapDataPos(mPoint(x + 1, y));
	}
	if (mazeData.getMapDataPos(mPoint(x, y-1)) == -1 && md.isValid(x, y-1)) {
		mazeData.getMap()[(y-1) * mazeData.getN() + x] = md.getMapDataPos(mPoint(x , y-1));
	}
	if (mazeData.getMapDataPos(mPoint(x, y+1)) == -1 && md.isValid(x, y+1)) {
		mazeData.getMap()[(y+1) * mazeData.getN() + x] = md.getMapDataPos(mPoint(x, y+1));
	}
}

void Mouse::step() {
	stepNum++;
	mana += 0.1;
}

void Mouse::moveToPos(const mPoint& targetPoint) {
	visitPos.push(curPos);
	curPos = targetPoint;
	step();
}


bool Mouse::search(const Maze& md)  {
	this->setMazeData(md);
	visionMazeData(md);

	if (energy - stepNum < 1)return false;
	else return true;
}