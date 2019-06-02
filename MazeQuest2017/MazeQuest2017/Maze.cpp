#pragma once
#include "Maze.h"
#include <fstream>
#include <vector>

ostream& operator<< (ostream& os, const Maze& m) {
	os << "Maze Size " << m.nSize << "*" << m.mSize << endl;
	for (int i = 0; i < m.mSize; i++) {
		for (int j = 0; j < m.nSize; j++) {
			string s = (m.map[i * m.nSize + j] == 0) ? "¡¡" : "¡á";
			if (m.map[i * m.nSize + j] == -1) s = "£¿";
			else if (m.map[i * m.nSize + j] == 'p') s = "¨Ù";
			os << s;
		}
		os << endl;
	}
	return os;
}

Maze::Maze() {
	map = nullptr;
	nSize = 0;
	mSize = 0;
}

Maze::Maze(string filename) {
	ifstream inFile(filename);

	vector<string> mapvector;
	int n = 0;
	int m = 0;
	while (!inFile.eof()) {
		string newString;
		getline(inFile, newString);
		n = newString.length() / 2 + 1;
		mapvector.push_back(newString);
		m++;
	}
	nSize = n;
	mSize = m;
	entrance = mPoint(1, 0);

	map = new int[nSize * mSize];

	for (int i = 0; i < mSize; i++) {
		string tmp = mapvector.at(i);
		for (int j = 0; j < nSize; j++) {
			map[i * nSize + j] = (tmp[j * 2] == '0') ? 0 : 1;
		}
	}


	inFile.close();
}

Maze::Maze(int* _map, int nsize, int msize) {
	nSize = nsize;
	mSize = msize;
	entrance = mPoint(1, 0);

	map = new int[nSize * mSize];

	for (int i = 0; i < mSize; i++) {
		for (int j = 0; j < nSize; j++) {
			map[i * nSize + j] = _map[i * nSize + j];
		}
	}
}


void Maze::setN(const int& nSize) { this->nSize = nSize; }
void Maze::setM(const int& mSize) { this->mSize = mSize; }
int* Maze::getMap() const { return map; }
int Maze::getM()const { return mSize; }
mPoint Maze::getEntrance() const { return entrance; }
int Maze::getN()const { return nSize; }
int Maze::getMapSize()const {
	return nSize * mSize;
}
int Maze::getMapDataPos(const mPoint& pos) const {
	return map[nSize * pos.getY() + pos.getX()];
}
bool Maze::isValid(const int& x, const int& y) const {
	if (x < 0) return false;
	if (y < 0) return false;
	if (x >= nSize) return false;
	if (y >= mSize) return false;

	return true;
}
bool Maze::isEnd(mPoint &pos) const {
	int x = pos.getX(); int y = pos.getY();
	
	if (pos == entrance) return false;

	if (!isValid(x - 1, y)) { return true; }
	if (!isValid(x, y - 1)) { return true; }
	if (!isValid(x + 1, y)) { return true; }
	if (!isValid(x, y - 1)) { return true; }

	return false;
}
bool Maze::canView(const mPoint &pos) const {
	return (map[nSize * pos.getY() + pos.getX()] == 0) || (map[nSize * pos.getY() + pos.getX()] == 1);
}
bool Maze::isRoad(const int& x, const int& y) const {
	if (!isValid(x, y)) return false;
	return (map[nSize * y + x] == 0);
}
bool Maze::isRoad(const mPoint &pos) const {
	return isRoad(pos.getX(), pos.getY());
}
bool Maze::isCrossRoad(const mPoint &pos) const {
	int x = pos.getX(); int y = pos.getY();
	
	int s = 0;
	if (isValid(x - 1, y)) { s += map[nSize * y + (x - 1)]; }
	if (isValid(x, y-1)) { s += map[nSize * (y-1) + x]; }
	if (isValid(x + 1, y)) { s += map[nSize * y + (x + 1)]; }
	if (isValid(x, y-1)) { s += map[nSize * (y+1) + x]; }

	return s < 2;
}
int Maze::numofBrunch(const mPoint &pos) const {
	int x = pos.getX(); int y = pos.getY();

	int s = 4;
	if (isValid(x - 1, y)) { s -= map[nSize * y + (x - 1)]; }
	if (isValid(x, y - 1)) { s -= map[nSize * (y - 1) + x]; }
	if (isValid(x + 1, y)) { s -= map[nSize * y + (x + 1)]; }
	if (isValid(x, y - 1)) { s -= map[nSize * (y + 1) + x]; }

	return s;
}

bool Maze::isDeadEnd(const mPoint &pos) const {
	int x = pos.getX(); int y = pos.getY();

	int s = 0;
	if (isValid(x - 1, y)) { s += map[nSize * y + (x - 1)]; }
	if (isValid(x, y - 1)) { s += map[nSize * (y - 1) + x]; }
	if (isValid(x + 1, y)) { s += map[nSize * y + (x + 1)]; }
	if (isValid(x, y - 1)) { s += map[nSize * (y + 1) + x]; }

	return s == 3;
}