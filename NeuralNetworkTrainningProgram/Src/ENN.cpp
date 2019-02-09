#include "stdafx.h"
#include <cstdlib>
#include <iostream>

	ENN::ENN(double c1[49][50], double c2[50][50],double c3[50]){
		for (int i = 0; i < 49; i++)
			for (int j = 0; j < 50; j++)
				this->c1[i][j] = c1[i][j];

		for (int i = 0; i < 50; i++)
			for (int j = 0; j < 50; j++)
				this->c2[i][j] = c2[i][j];

		for (int i = 0; i < 50; i++)
			this->c3[i] = c3[i];
	};
	
	ENN::ENN() {
		for (int i = 0; i < 49; i++)
			for (int j = 0; j < 50; j++)
				c1[i][j] = randomCreation();
		for (int i = 0; i < 50; i++)
			for (int j = 0; j < 50; j++)
				c2[i][j] = randomCreation();

		for (int i = 0; i < 50; i++)
			c3[i] = randomCreation();
	};

	//double evulate(board input) { return evulate(input.a); };

	double ENN::evulate(int input[7][7]) {
		int a[49];
		for (int i = 0; i < 7; i++)
			for (int j = 0; j < 7; j++)
				a[i * 7 + j] = input[i][j];
		return evulate(a);
	};

	double ENN::evulate(int input[49]){
		double n1[50] = {};
		double n2[50] = {};
		double n3 = 0;

		for (int i = 0; i < 49; i++)
			for (int j = 0; j < 50; j++)
				n1[i] += input[i] * c1[i][j];

		for (int i = 0; i < 50; i++)
			for (int j = 0; j < 50; j++)
				n2[i] += n1[i] * c2[i][j];

		for (int i = 0; i < 50; i++)
				n3 += n2[i] * c3[i];

		return n3;
	};

	ENN ENN::breed(){
		double bc1[49][50];
		double bc2[50][50];
		double bc3[50];

		for (int i = 0; i < 49; i++)
			for (int j = 0; j < 50; j++)
				bc1[i][j] = mutate(c1[i][j]);

		for (int i = 0; i < 50; i++)
			for (int j = 0; j < 50; j++)
				bc2[i][j] = mutate(c2[i][j]);

		for (int i = 0; i < 50; i++)
			bc3[i] = mutate(c3[i]);

		return ENN(bc1, bc2, bc3);
	};

	double ENN::mutate(double ibp) {
		int r = rand() % 100;
		if (r > 80)
			if (r < 88)
				return ibp * (randomCreation() + 1.5f);
			else if (r < 96)
				return ibp + (randomCreation() * 2.0f) * (ibp / (abs (ibp)));
			else if (r < 98)
				return -ibp;
			else return randomCreation();
		return ibp;
	};

	double ENN::randomCreation() {
		
		return -1.0f + ((double)rand() / RAND_MAX) * 2.0f;
	}

	void ENN::print(std::ostream& f) {
		for (int i = 0; i < 49; i++)
			for (int j = 0; j < 50; j++)
				f << c1[i][j] << std::endl;

		for (int i = 0; i < 50; i++)
			for (int j = 0; j < 50; j++)
				f << c2[i][j] << std::endl;

		for (int i = 0; i < 50; i++)
			f << c3[i] << std::endl;
	}