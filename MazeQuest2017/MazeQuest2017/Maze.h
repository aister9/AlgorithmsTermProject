#pragma once
#include <iostream>
#include <string>
#include "Point.h"
using namespace std;

class Maze {
private:
	int* map;
	int nSize;
	int mSize;
	mPoint entrance;
public:
	Maze();
	Maze(string filename);
	Maze(int* _map, int nsize, int msize);
	int getMapSize() const;
	void setN(const int& nSize);
	void setM(const int& mSize);
	int getN() const;
	int getM() const;
	mPoint getEntrance() const;
	int getMapDataPos(const mPoint& pos) const;
	int* getMap() const;
	int numofBrunch(const mPoint &pos) const;
	bool isValid(const int& x, const int& y) const;
	bool isEnd(mPoint &pos) const;
	bool canView(const mPoint &pos) const;
	bool isCrossRoad(const mPoint &pos) const;
	bool isDeadEnd(const mPoint &pos) const;
	friend ostream& operator<< (ostream& os, const Maze &m);
};