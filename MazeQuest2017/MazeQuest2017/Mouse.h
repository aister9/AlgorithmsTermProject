#pragma once
#include <stack>
#include <queue>
#include "Maze.h"
#include <Windows.h>
#include <list>
#include <math.h>

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
	stack<mPoint> crossPos;
	Maze mazeData;
	int *visitNum;
	int maxXpos = 0;
	int maxYpos = 0;
	int direction = 0;
	int chk = 0;
public:
	Mouse(const int &energy);
	void moveToPos(const mPoint& targetPoint);
	void teleport();
	void step();
	
	void initMazeData(const int& mazeSize);
	void setMazeData(const Maze &md) ;
	void visionMazeData(const Maze& md);

	bool search(const Maze& md) ;

	friend ostream& operator<<(ostream& os, const Mouse& m);
};

ostream& operator<<(ostream& os, const Mouse& m) {
	system("cls");
	os << " Mouse Property " << endl << "- energy : ";
	os << m.energy << " / mana - " << m.mana << " / Current Steps - " << m.stepNum << " / Remaining energy - " << m.energy - m.stepNum << " / Current Position - " << m.curPos << endl;
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
	visitNum = new int[mazeSize];
	memset(visitNum, 0, mazeSize * sizeof(int));
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
	if (md.canView(mPoint(x-1, y)) && md.isValid(x-1, y)) {
		mazeData.getMap()[y * mazeData.getN() + (x-1)] = md.getMapDataPos(mPoint(x - 1, y));
	}
	if (md.canView(mPoint(x + 1, y)) && md.isValid(x+1, y)) {
		mazeData.getMap()[y * mazeData.getN() + (x+1)] = md.getMapDataPos(mPoint(x + 1, y));
	}
	if (md.canView(mPoint(x, y-1)) && md.isValid(x, y-1)) {
		mazeData.getMap()[(y-1) * mazeData.getN() + x] = md.getMapDataPos(mPoint(x , y-1));
	}
	if (md.canView(mPoint(x, y+1)) && md.isValid(x, y+1)) {
		mazeData.getMap()[(y+1) * mazeData.getN() + x] = md.getMapDataPos(mPoint(x, y+1));
	}
}

void Mouse::step() {
	stepNum++;
	mana += 0.1;
}

void Mouse::moveToPos(const mPoint& targetPoint) {
	visitPos.push_back(curPos);
	if (mazeData.isCrossRoad(curPos) && (visitNum[curPos.getY() * mazeData.getN() + curPos.getX()] < mazeData.numofBrunch(curPos))) crossPos.push(curPos);
	curPos.setXY(targetPoint);
	step();
}
void Mouse::teleport() {
	mPoint p = crossPos.top();
	crossPos.pop();
	stepNum--;
	mana -= 5.1;
	mazeData.getMap()[curPos.getY() * mazeData.getN() + curPos.getX()] = 0;
	cout << "Teleport " << curPos << " to " << p << endl;
	moveToPos(p);
}

bool Mouse::search(const Maze& md)  {
	this->setMazeData(md);
	visionMazeData(md);
	visitPos.push_back(curPos);

	if (energy - stepNum < 1)return false;
	else {
		while (!visitPos.empty()) {
			visionMazeData(md);
			cout << *this;
			cout << visitPos.back() << direction;
			int xPos = curPos.getX(); int yPos = curPos.getY();
			maxYpos = max(yPos, maxYpos);
			maxXpos = max(xPos, maxXpos);
			visitNum[yPos * mazeData.getN() * xPos]++;

			if (md.isCrossRoad(curPos)) { // 해당위치가 갈림길일때
				if (xPos == mazeData.getEntrance().getX() && mazeData.getMapDataPos(mPoint(xPos - 1, yPos)) == 0) {
					direction = 2;
					//입구와 같은 라인인데 왼쪽이 열려있으면
				}
				if (xPos == maxXpos && mazeData.getMapDataPos(mPoint(xPos + 1, yPos)) == 0) {
					direction = 0;
					//지금까지 방문한 곳중 가장 오른쪽인데, 오른쪽이 열려있으면.
				}
			}
			
			switch (direction) {
			case 0:
				if (mazeData.getMapDataPos(mPoint(xPos+1, yPos)) == 0 && !search_list(visitPos, mPoint(xPos+1, yPos))) {
					moveToPos(mPoint(xPos + 1, yPos));
					chk = 0;
				}
				else {
					direction++;
					}
				break;
			case 1:
				if (mazeData.getMapDataPos(mPoint(xPos, yPos+1)) == 0 && !search_list(visitPos, mPoint(xPos, yPos+1))) {
					moveToPos(mPoint(xPos, yPos+1));
					chk = 0;
					}
				else {
					direction++;
				}
				break;
			case 2:
				if (mazeData.getMapDataPos(mPoint(xPos-1, yPos)) == 0 && !search_list(visitPos, mPoint(xPos-1, yPos))) {
					moveToPos(mPoint(xPos - 1, yPos));
					chk = 0;
				}
				else {
					direction++;
					}
				break;
			case 3:
				if (mazeData.getMapDataPos(mPoint(xPos, yPos-1)) == 0 && !search_list(visitPos, mPoint(xPos, yPos-1))) {
					moveToPos(mPoint(xPos, yPos-1));
					chk = 0;
					}
				else {
					direction=0;
				}
				break;
			}
			if (md.isDeadEnd(curPos)) {
				if(mana>=5)teleport();
				else {
					if (chk < 4) chk++; //모든 경로를 다 탐색하지 않았으면
					else {
						chk = 0;
						mPoint l = visitPos.back();
						mPoint tmp = visitPos.back();
						queue<mPoint> tq;
						while (tmp != crossPos.top()) {
							tq.push(tmp);
							visitPos.pop_back();
							tmp = visitPos.back();
						}
						crossPos.pop();
						visitPos.pop_back();
						while (!tq.empty()) {
							moveToPos(tq.front());
							tq.pop();
							visionMazeData(md);
							cout << *this;
						}
					}
				}
				visionMazeData(md);
				cout << "Dead END" << endl;
			}
			else {
				if (chk < 4) chk++; //모든 경로를 다 탐색하지 않았으면
				else {
					chk = 0;
					mPoint l = visitPos.back();
					mPoint tmp = visitPos.back();
					queue<mPoint> tq;
					while (tmp != crossPos.top()) {
						tq.push(tmp);
						visitPos.pop_back();
						tmp = visitPos.back();
					}
					crossPos.pop();
					visitPos.pop_back();
					while (!tq.empty()) {
						moveToPos(tq.front());
						tq.pop();
						visionMazeData(md);
						cout << *this;
					}
				}
			}

			if (md.isEnd(curPos)) {
				return true;
			}
		}
	}
}
