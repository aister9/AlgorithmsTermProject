#pragma once
#include "Maze.h"
#include <fstream>
#include <vector>

ostream& operator<< (ostream& os, const Maze& m) {
	os << "Maze Size " << m.nSize << "*" << m.mSize << endl;
	for (int i = 0; i < m.mSize; i++) {
		for (int j = 0; j < m.nSize; j++) {
			os << m.map[i * m.nSize + j] << " ";
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
		n = newString.length()/2+1;
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
			map[i*nSize + j] = (tmp[j*2] == '0') ? 0 : 1;
		}
	}


	inFile.close();
}