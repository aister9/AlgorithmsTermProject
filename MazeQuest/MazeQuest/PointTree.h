#pragma once
#include "Point.h"

class PointTree {
private:
	PointTree *parent;
	PointTree *child1;
	PointTree *child2;
	PointTree *child3;
	mPoint *data;
public:
	bool isVisit = false; //방문 했었는지
	PointTree() : parent(nullptr), child1(nullptr), child2(nullptr), child3(nullptr), data(nullptr) {}
	PointTree(mPoint *p) {
		PointTree();
		data = p;
	}
	void setParent(PointTree *pt);
	void setChild1(PointTree *pt);
	void setChild2(PointTree *pt);
	void setChild3(PointTree *pt);
	PointTree* getParent() { return parent; }
	PointTree* getChild1() { return child1; }
	PointTree* getChild2() { return child2; }
	PointTree* getChild3() { return child3; }
	mPoint* getData() { return data; }
	PointTree* search(const mPoint &p);
	int numofchild();
	bool isLeaf() { return numofchild() == 0; }

	friend ostream& operator<<(ostream& os, PointTree *pt);
};

ostream& operator<<(ostream& os, PointTree *pt) {
	if (pt != nullptr) {
		os << *(pt->getData());
		os << endl;
		os << pt->getChild1();
		os << pt->getChild2();
		os << pt->getChild3();
	}
	return os;
}

void PointTree::setParent(PointTree *pt) { parent = pt; }
void PointTree::setChild1(PointTree *pt) {	child1 = pt; }
void PointTree::setChild2(PointTree *pt) {  child2 = pt; }
void PointTree::setChild3(PointTree *pt) {  child3 = pt; }

PointTree* PointTree::search(const mPoint &p) {
	PointTree *res = nullptr;

	if (*data == p) return this;
	if (child1 != nullptr && res == nullptr) res = this->child1->search(p);
	if (child2 != nullptr && res == nullptr) res = this->child2->search(p);
	if (child3 != nullptr && res == nullptr) res = this->child3->search(p);

	return res;
}
int PointTree::numofchild() {
	int num = 0;
	if (child1 != nullptr) num++;
	if (child2 != nullptr) num++;
	if (child3 != nullptr) num++;

	return num;
}