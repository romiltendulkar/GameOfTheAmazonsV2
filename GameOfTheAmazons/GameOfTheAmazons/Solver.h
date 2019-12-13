
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

	int Search(Board *pBoard,int depthTo, int currDepth);
	int BSearch(Board* pBoard, int depthTo, int currDepth);
	MoveClass* GetBestMove(Board *pBoard, BoardIDs currPlayer, int depthTo, int currDepth);

public:
	bool isSatisfied;
	
	Board* mCurrentBoard;			//Current Board.
	
	MoveClass* mBestAI;				//Best Move AI can make on the current Board.
	MoveClass* mBestPlayer;			//Best Move Player can make in response to the AI.
private:
	FILE* mInFile;					//Pointer to the file containing the starting board.
	FILE* mOutFile;					//Pointer to the output file incase we wanna print our moves.
};

