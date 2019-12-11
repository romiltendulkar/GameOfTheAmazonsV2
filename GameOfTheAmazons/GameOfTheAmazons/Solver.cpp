//////////////////////////////////////////////////////////////
/**
@file Solver.cpp
@author Romil Tendulkar
@date 12/06/2019
@brief This class will be used to Solve the board
	based on the files given. Taking input from one
	and writing to another file
*/
//////////////////////////////////////////////////////////////
#include <algorithm>


#include "Solver.h"
Solver::Solver(FILE* pInFile, FILE* pOutFile)
{
	mInFile = pInFile;
	mOutFile = pOutFile;
	
	mCurrentBoard = nullptr;
	mCurrentBestAI = nullptr;
	mCurrentBestPlayer = nullptr;

	mBestAI = nullptr;
	mBestPlayer = nullptr;
}

Solver::~Solver()
{

}

void Solver::MakeBoard()
{
	int nRows, nCols;

	fscanf_s(mInFile, "%i", &nRows);
	fscanf_s(mInFile, "%i", &nCols);

	mCurrentBoard = new Board(nRows, nCols);

	for (int i = 0; i < nRows; ++i)
	{
		for (int j = 0; j < nCols; ++j)
		{
			int temp;
			fscanf_s(mInFile, "%i", &temp);	
			mCurrentBoard->mBoardVec->at(i).at(j) = temp;
		}
	}

	//mCurrentBoard->PrintBoard();
	std::cout<<"\nThe scope is: "<<mCurrentBoard->FindScope(BOARD_BLACK);
}

void Solver::Solve()
{
	Search(mCurrentBoard, 6, 0);
}

int Solver::Search(Board pBoard, int depthTo, int currDepth)
{
	bool isMax = false;

	if (currDepth % 2)
	{
		isMax = true;
	}
	if (isMax)
	{
		GetBestMove(pBoard, BOARD_BLACK);
	}
	else
	{
		GetBestMove(pBoard, BOARD_WHITE);
	}
	return 0;
}

int Solver::GetBestMove(Board pBoard, int currPlayer)
{
	return 0;
}