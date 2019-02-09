// Push4ENN.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <thread>

int row[7] = {0, 1, 2, 3, 2, 1, 0};
ENN* population;
int fitness[960];
int progress = 0;
int currentGlobleFitness;
int GlobleFitness = 0;
//public double step_size = 1.0f;

double evuluate(board b, int laststep, int depth, ENN p, int side) {
	if (depth == 3) {
		//if (b.checkV(1)) return std::numeric_limits<double>::max();
		//else if  (b.checkV(-1)) return std::numeric_limits<double>::min();
		if (side == 1) return p.evulate(b.a);
		else return p.evulate(b.invert().a);
	}

	double min = std::numeric_limits<double>::max();
	for (int j = 0; j < 4; j++)
		for (int k = 0; k < 7; k++)
			if (row[k] != laststep)
			{
				double t = evuluate(b.push(j, k, -side), row[k], 3, p, side);
				if (t < min)
					min = t;
			}
	return min;
}

int dual(ENN p1, ENN p2){
	board game = board();
	int laststep = 8;
	for (int i = 0; i < 25; i++)
	{
		double max = std::numeric_limits<double>::min();
		int step = 0;
		int d = 0;

		for (int j = 0; j < 4; j++)
			for (int k = 0; k < 7; k++)
				if (row[k] != laststep){
					double t = evuluate(game.push(j, k, 1), row[k], 2, p1, 1);
					if (t > max) {
						max = t;
						step = k;
						d = j;
					}
				}

		laststep = row[step];
		game = game.push(d, step, 1);
		if (game.checkV(1)) {
			return 5;
		}
		if (game.checkV(-1)) {
			return -3;
		}


		max = std::numeric_limits<double>::min();
		step = 0;
		d = 0;

		for (int j = 0; j < 4; j++)
			for (int k = 0; k < 7; k++)
				if (row[k] != laststep) {
					double t = evuluate(game.push(j, k, -1), row[k], 2, p2, -1);
					if (t > max) {
						max = t;
						step = k;
						d = j;
					}
				}

		laststep = row[step];
		game = game.push(d, step, -1);
		if (game.checkV(1)) {
			return 5;
		}
		if (game.checkV(-1)) {
			return -3;
		}

	}

	return 1;
}

//double stepControl() {
//	if (currentGlobleFitness - GlobleFitness > 10)
//		step_size = 1.0f;
//	else
//		return step_size < 3.0f ? step_size + 0.2f : 3.0f;
//}

void cal_thread(int start, int end, int id) {
	for (int j = start; j < end; j++) {
		for (int k = 0; k < 80; k++) {
			int t = dual(population[j], population[k * 12 + id]);
			fitness[j] += t;
			if (t == 5) t = -3; else if (t == -3) t = 5;
			fitness[k * 12 + id] += t;
			progress++;
		}
		if (id == 1) std::cout << "progress: " << progress << "/76800" << std::endl;
	}
}

int main()
{
	std::cout << "Generating:"  << std::endl;
	population = new ENN[960];
	for (int i = 0; i < 960; i++) {
		std::cout << "Generating:" << i << std::endl;
		population[i] = ENN();
	}

	std::ofstream file;

	file.open("Record.txt", std::ios::out);
	file << "Start:" << std::endl;
	file.close();

	for (int i = 0; i < 100; i++)
	{
		std::cout << "Generation: " << i << std::endl;
		progress = 0;

		for (int j = 0; j < 960; j++) {
			fitness[j] = 0;
		}

		std::thread th[12];
		for (int i = 0; i < 12; i++)
			th[i] = std::thread(&cal_thread, i*80, i*80 + 80, i);

		for (int i = 0; i < 12; i++)
			th[i].join();

		currentGlobleFitness = fitness[959];
		std::cout << "Globle Fitness: " << currentGlobleFitness << " / " << GlobleFitness << std::endl;

		file.open("Record.txt", std::ios::out || std::ios::ate);
		file << "Globle Fitness: " << currentGlobleFitness << " / " << GlobleFitness << std::endl;
		file.close();
		//step_size = stepControl();

		for (int j = 0; j < 959; j++)
			for (int k = j + 1; k < 960; k++)
				if (fitness[j] < fitness[k]) {
					int t = fitness[j];
					fitness[j] = fitness[k];
					fitness[k] = t;
					ENN t2 = population[j];
					population[j] = population[k];
					population[k] = t2;
				}

		file.open(std::to_string(i) + "fitest G.txt", std::ios::out);
		population[0].print(file);
		file.close();

		file.open(std::to_string(i) + "G.txt", std::ios::out);
		for (int j = 1; j < 11; j ++)
		population[j].print(file);
		file.close();

		ENN t = population[0];
		for (int j = 0; j < 480; j++) {
			population[j + 480] = population[j].breed();
			population[j] = population[j].breed();
		}
		population[959] = t;

		GlobleFitness = fitness[0];

	}

	delete[] population;
    return 0;
}

