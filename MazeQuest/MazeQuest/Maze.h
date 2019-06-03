#pragma once
#include <iostream>
#include <string>
#include "Point.h"
using namespace std;

class Maze {
private:
	int* map; //���� 1���� ��ķ� �����Ҵ�
	int nSize; //���� n������
	int mSize; //���� m������
	mPoint entrance; //�Ա� (�� ���α׷����� �׻� 0,1�� �ɰ���
public:
	//����Ʈ���� 
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

	int numofBrunch(const mPoint &pos) const;	//�б����� ����
	bool isValid(const int& x, const int& y) const;	//������ ��ġ�ΰ�
	bool isEnd(mPoint &pos) const;	//�� ��ġ�� ������
	bool canView(const mPoint &pos) const;	//�� �� �ִ� ���ΰ�
	bool isCrossRoad(const mPoint &pos) const; //���������� üũ
	bool isDeadEnd(const mPoint &pos) const; //���� ������ üũ
	bool isRoad(const int& x, const int& y) const; //x, y�� ������
	bool isRoad(const mPoint &pos) const; //mPoint�� �̿��� �������̵�
	friend ostream& operator<< (ostream& os, const Maze &m);
};