#pragma once
#include <iostream>
using namespace std;

class mPoint {
private:
	int x;
	int y;
	int visitNum;
public:
	mPoint(const int& x = 0, const int& y = 0) : x(x), y(y), visitNum(0) {}
	int getX() const { return x; }
	int getY() const { return y; }
	
	friend ostream& operator<<(ostream& os, const mPoint& p) {
		os << "( " << p.getX() << ", " << p.getY() << " )";
		return os;
	}
};
