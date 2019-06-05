#pragma once
#include <iostream>
#include <string>
#include "Point.h"
using namespace std;

class Maze {
private:
	int* map; //맵을 1차원 행렬로 동적할당
	int nSize; //맵의 n사이즈
	int mSize; //맵의 m사이즈
	mPoint entrance; //입구 (이 프로그램에선 항상 0,1이 될거임
public:
	//컨스트럭터 
	Maze();
	Maze(string filename);
	Maze(int* _map, int nsize, int msize);

	//getter and setter
	int getMapSize() const;
	void setN(const int& nSize);
	void setM(const int& mSize);
	int getN() const;
	int getM() const;
	mPoint getEntrance() const;
	int getMapDataPos(const mPoint& pos) const;
	int* getMap() const;

	int numofBrunch(const mPoint &pos) const;	//분기점의 개수
	bool isValid(const int& x, const int& y) const;	//가능한 위치인가
	bool isEnd(mPoint &pos) const;	//그 위치가 끝인지
	bool canView(const mPoint &pos) const;	//볼 수 있는 곳인가
	bool isCrossRoad(const mPoint &pos) const; //교차로인지 체크
	bool isDeadEnd(const mPoint &pos) const; //막힌 길인지 체크
	bool isRoad(const int& x, const int& y) const; //x, y가 길인지
	bool isRoad(const mPoint &pos) const; //mPoint를 이용한 오버라이드
	friend ostream& operator<< (ostream& os, const Maze &m);
};