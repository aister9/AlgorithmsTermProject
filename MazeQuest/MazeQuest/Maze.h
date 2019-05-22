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
	friend ostream& operator<< (ostream& os, const Maze &m);
};