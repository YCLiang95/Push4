#include "stdafx.h"
#include <iostream>
#pragma once

const int direction[8][2] = { {1, 0}, {-1, 0}, {0, 1}, {0, -1}, {-1, -1}, {1, -1} , {-1, 1}, {1, 1} };

	board::board() {
		for (int i = 0; i < 7; i++)
			for (int j = 0; j < 7; j++)
				a[i][j] = 0;
	}

	board board::push(int d, int r, int side) {
		board result = board();
		for (int i = 0; i < 7; i++)
			for (int j = 0; j < 7; j++)
				result.a[i][j] = a[i][j];
		if (d == 0)
		{
			for (int i = 6; i > 0; i--)
				result.a[i][r] = result.a[i - 1][r];
			result.a[0][r] = side;
		}
		else if (d == 1) {
			for (int i = 0; i < 6; i++)
				result.a[r][i] = result.a[r][i + 1];
			result.a[r][6] = side;
		}
		else if (d == 2) {
			for (int i = 0; i < 6; i++)
				result.a[i][r] = result.a[i + 1][r];
			result.a[6][r] = side;
		}
		else {
			for (int i = 6; i > 0; i--)
				result.a[r][i] = result.a[r][i - 1];
			result.a[r][0] = side;
		}
		return result;
	};

	bool board::checkV(int side) {
		for (int i = 0; i < 7; i++)
			for (int j = 0; j < 7; j++)
				if (a[i][j] == side){
					for (int k = 0; k < 8; k++) {
						int x = i;
						int y = j;
						bool valid = true;
						int de = 0;
						while (valid) {
							de++;
							if (de == 4) {
								return true;
							}
							x += direction[k][0];
							y += direction[k][1];
							valid = (x >= 0) && (x < 7) && (y >= 0) && (y < 7) && (a[x][y] == a[i][j]);
						}
					}
				}
		return false;
	}

	board board::invert() {
		board t = board();
		for (int i = 0; i < 7; i++)
			for (int j = 0; j < 7; j++)
				t.a[i][j] -= a[i][j];
		return t;
	}

	void board::print() {
		for (int i = 0; i < 7; i++) {
			for (int j = 0; j < 7; j++)
				std::cout << a[i][j] + 1 << " ";
			std::cout << std::endl;
		}
	}