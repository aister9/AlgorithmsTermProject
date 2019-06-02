#pragma once
#include <stack>
#include <queue>
#include "Maze.h"
#include <Windows.h>
#include <list>
#include <math.h>
#include "PointTree.h"

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
	string telp;
	PointTree *map;
public:
	Mouse(const int &energy);
	void moveToPos(const mPoint& targetPoint);
	void teleport();
	void step();
	
	void initMazeData(const int& mazeSize);
	void setMazeData(const Maze &md) ;
	void visionMazeData(const Maze& md);

	PointTree* getMapTree() const { return map; }

	bool search(const Maze& md) ;
	bool search2(const Maze& md);

	friend ostream& operator<<(ostream& os, const Mouse& m);
};

ostream& operator<<(ostream& os, const Mouse& m) {
	system("cls");
	os << " Mouse Property " << endl << "- energy : ";
	os << m.energy << " / mana - " << m.mana << " / Current Steps - " << m.stepNum << " / Remaining energy - " << m.energy - m.stepNum << " / Current Position - " << m.curPos << endl;
	os << m.mazeData;
	os << m.telp;
	return os;
}

Mouse::Mouse(const int &energy):energy(energy){
	mana = 0.0;
	stepNum = 0;
	curPos = mPoint(1, 0);
	map = new PointTree(new mPoint(curPos));
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
	visitNum[curPos.getY() * mazeData.getN() + curPos.getX()] += 1;
	visitPos.push_back(curPos);
	if (mazeData.isCrossRoad(curPos))
		if(visitNum[curPos.getY() * mazeData.getN() + curPos.getX()] < mazeData.numofBrunch(curPos))
		crossPos.push(curPos);
	mazeData.getMap()[curPos.getY() * mazeData.getN() + curPos.getX()] = 0;
	curPos.setXY(targetPoint);
	step();
}
void Mouse::teleport() {
	mPoint p = crossPos.top();
	crossPos.pop();
	stepNum--;
	mana -= 5.1;
	mazeData.getMap()[curPos.getY() * mazeData.getN() + curPos.getX()] = 0;
	telp += "Teleport " + curPos.toString() + " to " + p.toString() + "\n";
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
			if (energy - stepNum < 1)return false;
		}
	}
}


bool Mouse::search2(const Maze& md) {
	this->setMazeData(md); //쥐가 md로 들어감
	visionMazeData(md); //쥐가 들어갔으므로 맵을 밝힌다.

	int xPos = curPos.getX(); int yPos = curPos.getY();

	// Init Frist Node - because)) 입구는 (0, 1)로 고정이고, 다음 위치는 무조건 (0,2)하나뿐이다. 
	mPoint *down = new mPoint(xPos, yPos + 1);
	map->setChild1(new PointTree(down));
	map->getChild1()->setParent(map);

	PointTree* curNode = map->getChild1();

	while (curNode != nullptr) {
		mPoint *curPoint = curNode->getData();
		int xPos = curPoint->getX(); int yPos = curPoint->getY();
		if(curPos!=*curPoint)moveToPos(*curPoint);
		visionMazeData(md);
		cout << *this;
		cout << "Parent : " << *curNode->getParent()->getData();
		cout << "Current : " << *curPoint << endl;
		mPoint *down = new mPoint(xPos, yPos + 1);
		mPoint *up = new mPoint(xPos, yPos - 1);
		mPoint *left = new mPoint(xPos - 1, yPos);
		mPoint *right = new mPoint(xPos + 1, yPos);
		bool back = false;

		if (md.isEnd(curPos)) return true;
		if (energy - stepNum < 1) return false;

		queue<PointTree *>childList;
		if (md.isRoad(right->getX(), right->getY()) && *right != *curNode->getParent()->getData() && (curNode->search(*right)) == nullptr) {
			PointTree* rightChild = new PointTree(right);
			rightChild->setParent(curNode);
			childList.push(rightChild);
			cout << "Next(right) : " << childList.back();
		}
		if (md.isRoad(down->getX(), down->getY()) && *down != *curNode->getParent()->getData() && (curNode->search(*down)) == nullptr) {
			PointTree* downChild = new PointTree(down);
			downChild->setParent(curNode);
			childList.push(downChild);
			cout << "Next(down) : " << childList.back();
		}
		if (md.isRoad(left->getX(), left->getY()) && *left != *curNode->getParent()->getData() && (curNode->search(*left)) == nullptr) {
			PointTree* leftChild = new PointTree(left);
			leftChild->setParent(curNode);
			childList.push(leftChild);
			cout << "Next(left) : " << childList.back();
		}
		if (md.isRoad(up->getX(), up->getY()) && *up != *curNode->getParent()->getData() && (curNode->search(*up)) == nullptr) {
			PointTree* upChild = new PointTree(up);
			upChild->setParent(curNode);
			childList.push(upChild);
			cout << "Next(up) : " << childList.back();
		}

		if (curNode->getChild1() != nullptr) {
			if (curNode->getChild1()->isVisit) {
				back = true;
			}
			else {
				curNode->isVisit = true;
				curNode = curNode->getChild1();
				continue;
			}
		}
		else if(!childList.empty()){
			curNode->setChild1(childList.front());
			childList.pop();
		}
		if (curNode->getChild2() != nullptr) {
			if (curNode->getChild2()->isVisit) {
				back = true;
			}
			else {
				curNode->isVisit = true;
				curNode = curNode->getChild2();
				continue;
			}
		}
		else if (!childList.empty()) {
			curNode->setChild2(childList.front());
			childList.pop();
		}
		if (curNode->getChild3() != nullptr) {
			if (curNode->getChild3()->isVisit) {
				back = true;
			}
			else {
				curNode->isVisit = true;
				curNode = curNode->getChild3();
				continue;
			}
		}
		else if (!childList.empty()) {
			curNode->setChild3(childList.front());
			childList.pop();
		}
		if (curNode->isLeaf()) {
			curNode->isVisit = true;
			if (mana > 5.0) {
				mPoint p = crossPos.top();
				crossPos.pop();
				stepNum--;
				mana -= 5.1;
				PointTree* target = map->search(p);
				telp += "Teleport " + curPos.toString() + " to " + p.toString() + "\n";
				curNode = target;
				moveToPos(*target->getData());
			}
			else curNode = curNode->getParent();
			continue;
		}
		curNode->isVisit = true;
		if(back)curNode = curNode->getParent();

		//while (true) {
		//	char c = getchar();
		//	if (c == ' ') break;
		//}
	}

	cout << "Map Tree : ";
	cout << map << endl;
	return false;
}