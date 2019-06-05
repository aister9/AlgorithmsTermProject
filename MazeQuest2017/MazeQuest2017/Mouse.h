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
	int energy; // 총 에너지량
	double mana; //보유 마나량
	int stepNum; //현재까지 걸음 수
	mPoint curPos; // 현재 위치
	list<mPoint> visitPos; //방문했던 위치들
	stack<mPoint> crossPos; //교차로 위치를 스택 구조로 저장
	Maze mazeData; //쥐가 기억하는 맵 데이터
	int *visitNum; //해당 포지션에 몇번 방문했는지를 저장하는 배열
	int maxXpos = 0; // 지금까지 방문 했던 장소중 가장 큰 X 값
	int maxYpos = 0; // 지금까지 방문 했던 장소중 가장 큰 Y 값
	int direction = 0; //진행 방향
	int chk = 0; //해당 위치에서 방향을 체크한 횟수
	string telp; //텔레포트 로그를 스트링으로 만든 것
	PointTree *map; // 쥐가 이동하여 짜는 지도를 트리형태로 구현 한 것
public:
	Mouse(const int &energy); //생성자
	void moveToPos(const mPoint& targetPoint); //targetPoint로 이동함
	void teleport(); //crossPos의 top으로 텔레포트함
	void step(); //한 걸음 진행했다고 알려줌
	
	void initMazeData(const int& mazeSize); //mazeSize로 쥐가 가지고 있는 맵 데이터를 초기화한다
	void setMazeData(const Maze &md) ; //md로 쥐의 맵을 설정함( 쥐가 md라는 맵으로 들어감)
	void visionMazeData(const Maze& md); //해당 위치에 있을때 md로부터 데이터를 읽어와 맵을 밝힘

	PointTree* getMapTree() const { return map; } //Map Tree를 반환

	bool search(const Maze& md) ; //실패한 서치 방식
	bool search2(const Maze& md); //DFS를 구현함

	friend ostream& operator<<(ostream& os, const Mouse& m); //Print() override
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

		//이거는 해당 위치에 연결되어 있는 길이 있는지 반환해서 있으면 큐에 넣고 없으면 그냥 지나감
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

		//차일드 노드가 null이면 연결하고, 아니면 방문했는지를 체크하고 이동함
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
		//막다른길이면( 막다른 길이면 그 노드는 무조건 leaf 노드가 될것임)
		if (curNode->isLeaf()) {
			curNode->isVisit = true;
			//텔레포트 할 수 있는 조건을 만족함?
			if (mana > 5.0) {
				mPoint p = crossPos.top();
				if (curPos.distance(p) > 3) {
					crossPos.pop();
					stepNum--;
					mana -= 5.1;
					PointTree* target = map->search(p);
					telp += "Teleport " + curPos.toString() + " to " + p.toString() + "\n";
					curNode = target;
					moveToPos(*target->getData());
				}
				else curNode = curNode->getParent();
			}
			//아니면 걍 부모님노드로간다
			else curNode = curNode->getParent();
			continue;
		}
		curNode->isVisit = true;
		//현재 상태가 돌아가야하는 상태면 부모님노드로 이동
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