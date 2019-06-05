#pragma once
#include <iostream>
#include <string>
using namespace std;

class mPoint {
private:
	int x;
	int y;
	int visitNum;
public:
	mPoint(const int& x = 0, const int& y = 0) : x(x), y(y), visitNum(0) {}
	void setXY(const int &x, const int &y) { this->x = x; this->y = y; }
	void setXY(const mPoint &mp) { x = mp.getX(); y = mp.getY(); }
	int getX() const { return x; }
	int getY() const { return y; }
	
	friend ostream& operator<<(ostream& os, const mPoint& p) {
		os << "( " << p.getX() << ", " << p.getY() << " )";
		return os;
	}

	bool operator==(const mPoint &p) {
		return x == p.getX() && y == p.getY();
	}
	bool operator!=(const mPoint &p) {
		return x != p.getX() || y != p.getY();
	}

	string toString() {
		return "( " + to_string(x) + ", " + to_string(y) + " )";
	}

	int distance(const mPoint& p) { return abs(x - p.getX()) + abs(y - p.getY()); }
};