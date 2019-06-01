#pragma once
#include <iostream>
#include "Maze.h"
#include "Mouse.h"
#include <stdlib.h>

int main() {
	Maze m("Maze1.txt");

	cout << "Mouse Energy Allocation : " << m.getMapSize() * 2 << endl;
	cout << m;
	while (true) {
		char c = getchar();
		if (c == ' ') break;
	}
	Mouse player(m.getMapSize() * 2);
	cout << player;
	if (player.search(m)) {
		player.visionMazeData(m);
		cout << player;
		cout << " Mouse Exit " << endl;
	}
	else {
		cout << player;
		cout << " can`t found way " << endl;
	}

	system("pause");
}