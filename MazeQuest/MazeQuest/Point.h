#pragma once


class mPoint {
private:
	int x;
	int y;
	int visitNum;
public:
	mPoint(const int& x = 0, const int& y = 0) : x(x), y(y), visitNum(0) {}
};