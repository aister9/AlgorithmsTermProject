#pragma once
#include <iostream>
#include "Maze.h"
#include "Mouse.h"
#include <stdlib.h>

int main() {
	Maze m("Maze1.txt");

	cout << m;

	cout << "Mouse Energy Allocation : " << m.getMapSize() * 2 << endl;
	Mouse player(m.getMapSize() * 2);
	cout << player;
	player.search(m);
	cout << player;

	system("pause");
}