#Author 		Yecheng Liang 010010481
#Group Member 	Wenxiang Hu 009034246

#Please place neural data file with this python program
#Neural network program and source code written by me in C++ should also included in the package, sloppy coding and no commenting
#The neural network was not fully trained
#I made serveral mistakes with the trainning program and wasted 40 hours worth of tranining, only restart training the very morning before due date

import sys;
import math;
import os;

#Think Tree depth
#The higher the depth, the longer it take
thinkDepth = 4;

#Neural network
#Neural network data was stored in a seprate file (5000 lines)
#Both data file and tranning program should be included in the submission
nerualNetworkDataFile = "G86fitest.txt";

#True to clear screen after every turn
#Might not work for Linux/OSX
clearScreen = False;
clCommand = "cls";
#clCommand = "clear";	##Linux/OSX

#board class
#have one variable, a , which is the array of board
#1 stands for AI player, -1 stands for human player, 0 for empty space
class board:
	#initlize the board with all empty
	def  __init__(self):
		self.a = [[0 for i in range(7)] for j in range(7)];	
	#return a new board that is, the self push from direction of d and row/column of r
	#para	d, the direction it was pushed, reference to directionDirector()
	#para	r, the row/column it was pushed, reference to rcDirector()
	#para 	side, the side that pushed, 1 for AI player and -1 for human player
	def push(self,
			 d, r,
			 side):
		n = board();
		n.a =  [[self.a[j][i] for i in range(7)] for j in range(7)];
		if (d == 0):
			for i in range(6):
				n.a[6 - i][r] = n.a[6 - i - 1][r];
			n.a[0][r] = side;
		elif (d == 1):
			for i in range(6):
				n.a[r][i] = n.a[r][i + 1];
			n.a[r][6] = side;
		elif (d == 2):
			for i in range(6):
				n.a[i][r] = n.a[i + 1][r];
			n.a[6][r] = side;
		else:
			for i in range(6):
				n.a[r][6 - i] = n.a[r][6 - i - 1];
			n.a[r][0] = side;
		return n;
	#check if this board reach an ending status
	def gameOver(self, side):
		d = [[1, 0], [-1, 0], [0, 1], [0, -1], [1, 1], [-1, 1], [1, -1], [-1, -1]];
		for i in range(7):
			for j in range(7):
				if(self.a[i][j] == side):
					for k in range(8):
						x = i;
						y = j;
						depth = 0;
						while ((x >= 0) and (y >= 0) and (x < 7) and (y < 7) and self.a[x][y] == side):
							depth += 1;
							if (depth == 4):
								return True;
							x += d[k][0];
							y += d[k][1];
		return False;
	
	#print the board
	def	printBoard(self):
		symb = ['X', 'E', 'O'];
		symb2 = ['A', 'B', 'C', 'D', 'c', 'b', 'a'];
		print("          N      ");
		print("    A B C D c b a");
		for i in range(7):
			print("   +-+-+-+-+-+-+-+");
			if (i == 3):
				s = "W " + symb2[i];
			else:
				s = "  " + symb2[i];
			for j in range(7):
				s += "|" + symb[self.a[i][j] + 1];
			if (i == 3):
				s += "|" + symb2[i] + ' E';
			else:
				s += "|" + symb2[i];
			print(s);
		print("   +-+-+-+-+-+-+-+");
		print("    A B C D c b a");
		print("          S      ");

#neural network class, with 49 input, 50 neural on first layer, 50 neural on second layer and 1 neural for output
class NN:
	#read neural network data from file
	#para file, file name of the neural network data file
	def  __init__(self, file):
		#connection from input to first layer
		self.c1 = [[0 for i in range(50)] for j in range(49)];
		#connections from first layer to second layer
		self.c2 = [[0 for i in range(50)] for j in range(50)] 
		#connections from second layer to output
		self.c3 = [0 for i in range(50)];
		try:
			f = open(file, 'r');
		except (OSError, IOError) as e:
			print("Neural Network Data File Not Found!");
			print("Please place the neural network data with the python program");
			raw_input("Press enter to close");
			sys.exit();	
		for i in range(49):
			for j in range(50):
				self.c1[i][j] = float(f.readline());
		for i in range(50):
			for j in range(50):
				self.c2[i][j] = float(f.readline());
		for i in range(50):
			self.c3[i] = float(f.readline());
		f.close();
	#make nerual network to evulate the favorability of the board
	#para, gameBoard, the board that needs to be evuluated
	def	evuluate(self, gameBoard):
		n1 = [0 for i in range(50)];
		n2 = [0 for i in range(50)];
		n3 = 0;
		for i in range(7):
			for j in range(7):
				for k in range(50):
					n1[k] += gameBoard.a[i][j] * self.c1[i * 7 + j][k];
		for i in range(50):
			for j in range(50):
				n2[j] += n1[i] * self.c2[i][j];
		for i in range(50):
			n3 += n2[i] * self.c3[i];
		return n3;

#Minimax
#AI player evulate the cureent board, predict possible future situations and make a move
#para gameBoard, board that need to be evulate
#para depth, current evualting depth, start with 0
#para lastStep, for same letter rule
#para maximizing, True for maximizer, False for minimizer
#para alpha, beta for pruning
def evuluate (gameBoard,
			  depth, lastStep,
			  maximizing,
			  alpha, beta):
	global pushBot;
	global thinkDepth;
	dr = ['N', 'E', 'S', 'W'];
	rc = ['A', 'B' ,'C', 'D', 'c', 'b', 'a'];
	ls = [0, 1, 2, 3, 2, 1, 0];
	#evulate leaf node at the set depth
	#A win is infinte favorable
	#I should trust nerual network on this one, but iterate 7x7 board is actually faster than 5000 connections of float calculation
	if (gameBoard.gameOver(1)):
		return float("inf") - 1, None, None;
	#A loss is infinte unfavorable, but still better than doing nothing
	elif (gameBoard.gameOver(-1)):
		return -float("inf") + 1, None, None;
	#evulate with neural network
	elif (depth == thinkDepth):
		return pushBot.evuluate(gameBoard), None, None;
	#minimizer player
	if (not maximizing):
		mini = float("inf");
		for i in range(4):
			for j in range(7):
				if (ls[j] != lastStep):
					t, __, __ = evuluate(gameBoard.push(i, j, -1), depth + 1, ls[j], True, alpha, beta);
					if (t <= mini):
						mini = t;
						saveDirection = i;
						savedStep = j;
						beta = min (beta, mini);
						#alpha cut
						if (mini <= alpha):
							return mini, gameBoard.push(saveDirection, savedStep, -1), ls[savedStep];
		return mini, gameBoard.push(saveDirection, savedStep, -1), ls[savedStep];
	else:
		maxim = -float("inf");
		for i in range(4):
			for j in range(7):
				if (ls[j] != lastStep):
					t, __, __  = evuluate(gameBoard.push(i, j, 1), depth + 1, ls[j], False, alpha, beta);
					if (t >= maxim):
						maxim = t;
						saveDirection = i;
						savedStep = j;
						alpha = max (alpha, maxim);
						#beta cut
						if (beta <= maxim):
							return maxim, gameBoard.push(saveDirection, savedStep, 1), ls[savedStep];
		if (depth == 0):
			print("Player O moved:" + dr[saveDirection] + rc[savedStep]);
		return maxim, gameBoard.push(saveDirection, savedStep, 1), ls[savedStep];

#match direction with an interger		
def	directionDirector(d):
	return {
		'N':	0,
		'E':	1,
		'S':	2,
		'W':	3
	}.get(d, -1);

#match the row/column with an integer
def	rcDirector(d):
	return {
		'A':	0,
		'B':	1,
		'C':	2,
		'D':	3,
		'c':	4,
		'b':	5,
		'a':	6
	}.get(d, -1);

#check if the game is overed, if so, displayed the final message
def	checkGameOver(board):
	global currentTurn;
	playerO = board.gameOver(1);
	playerX = board.gameOver(-1);
	if (currentTurn >= 30):
		print("Draw! Exceed time limit!");
		return True;
	elif (playerO and playerX):
		print("Draw! Both player Win!");
		return True;
	elif (playerO):
		print("Player O Wins! You Lose!");
		return True;
	elif (playerX):
		print("You Win! Player O Loses!");
		return True;
	else:
		return False;

#ask player to input next move
#para	board, the current board
#para	lastStep, for same letter rule
#return a new board which is after the move
def playerTurn(board, lastStep):
	global clearScreen;
	global clCommand;
	ls = [0, 1, 2, 3, 2, 1, 0];
	while True:
		inp = raw_input("Please enter a move (direction letter from N, E, W, S followed by a row or column letter):");
		if (len(inp) != 2):
			print("Invalid Input: Input length");
		elif (rcDirector(inp[1]) == -1):
			print("Invalid Input: Row/Column");
		elif (directionDirector(inp[0]) == -1):
			print("Invalid Input: Move direction");
		elif (ls[rcDirector(inp[1])] != lastStep):
			break;
		else:
			print("Invalid move: voliate same letter rule");
	if (clearScreen):
		os.	system(clCommand);
	print("Player X moved:" + inp);
	return board.push(directionDirector(inp[0]), rcDirector(inp[1]), -1), ls[rcDirector(inp[1])];

#main function	
def	main():
	global currentTurn;
	inp = raw_input('Would you like to go first (y/n)?')
	game = board();
	lastStep = 8;
	isGameOver = False;
	if (inp == 'n'):
		print("Player O is considering...");
		confidence, game, lastStep = evuluate(game, 0, lastStep, True, -float("inf"), float("inf"));
	while (not isGameOver):
		currentTurn += 1;
		print("Turn:" + str(currentTurn));
		game.printBoard();
		game, lastStep = playerTurn(game, lastStep);
		isGameOver = checkGameOver(game);
		if (isGameOver):
			break;
		print("Player O is considering...");
		confidence, game, lastStep = evuluate(game, 0, lastStep, True, -float("inf"), float("inf"));
		isGameOver = checkGameOver(game);
	game.printBoard();
	raw_input("Press enter to close");
	sys.exit();
	
currentTurn = 0;		
pushBot = NN(nerualNetworkDataFile);
main();