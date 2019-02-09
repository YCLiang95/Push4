#include "stdafx.h"
#pragma once

class board {
public:
	int a[7][7];

	board();

	board push(int d, int r, int side);

	bool checkV(int side);

	board invert();

	void print();
};