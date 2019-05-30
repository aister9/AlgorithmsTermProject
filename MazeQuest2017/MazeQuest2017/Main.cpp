#pragma once
#include <iostream>
#include "Maze.h"
#include "Mouse.h"
#include <stdlib.h>

int main() {
	Maze m("Maze1.txt");

	cout << "Mouse Energy Allocation : " << m.getMapSize() * 2 << endl;
	Mouse player(m.getMapSize() * 2);
	cout << player;
	if (player.search(m)) {}
	else {
		cout << " can`t found way " << endl;
		cout << player;
	}

	system("pause");
}