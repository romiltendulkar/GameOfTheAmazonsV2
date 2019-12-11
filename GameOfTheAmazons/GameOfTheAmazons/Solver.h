
//////////////////////////////////////////////////////////////
/**
@file Solver.h
@author Romil Tendulkar
@date 12/06/2019
@brief This class will be used to Solve the board 
	based on the files given. Taking input from one
	and writing to another file
*/
//////////////////////////////////////////////////////////////
#pragma once

#include <iostream>

#include "Board.h"
#include "MoveClass.h"

class Solver
{
public:
	Solver(FILE* pInFile, FILE* pOutFile);
	~Solver();
	void MakeBoard();
	void Solve();


public:
	Board* mCurrentBestAI;			//Storing the Board after the AI makes its best move.
	Board* mCurrentBestPlayer;		//Storing the Board after the player makes his best move.
	Board* mCurrentBoard;			//Current Board just incase.
private:
	FILE* mInFile;					//Pointer to the file containing the starting board.
	FILE* mOutFile;					//Pointer to the output file incase we wanna print our moves.
};

