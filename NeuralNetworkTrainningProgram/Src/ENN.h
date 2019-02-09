#pragma once
#include "stdafx.h"
#include <cstdlib>
#pragma once


class ENN {

	double c1[49][50];
	double c2[50][50];
	double c3[50];

public:

	ENN(double c1[49][50], double c2[50][50], double c3[50]);
	ENN();

	double evulate(int input[7][7]);

	double evulate(int input[49]);

	ENN breed();

	void print(std::ostream& f);

private:
	double mutate(double ibp);
	double randomCreation();

};