//////////////////////////////////////////////////////////////
/**
@file Board.cpp
@author Romil Tendulkar
@date 12/02/2019
@brief This class will be used to store the board and the positions on it.
*/
//////////////////////////////////////////////////////////////

#include <iostream>
#include "Board.h"
#include "MoveClass.h"

Board::Board()
{
	mNumCols = 10;
	mNumRows = 10;
	mBoardVec = new std::vector<std::vector<int>>(mNumRows, std::vector<int>(mNumCols));
}

Board::Board(unsigned int nRows, unsigned int nCols)
{
	mNumCols = nCols;
	mNumRows = nRows;
	mBoardVec = new std::vector<std::vector<int>>(mNumRows, std::vector<int>(mNumCols));
}

Board::Board(Board* pBoard)
{
	mNumCols = pBoard->mNumCols;
	mNumRows = pBoard->mNumRows;
	mBoardVec = new std::vector<std::vector<int>>(mNumRows, std::vector<int>(mNumCols));
	for (int i = 0; i < mNumRows; ++i)
	{
		for (int j = 0; j < mNumCols; ++j)
		{
			mBoardVec->at(i).at(j) = pBoard->mBoardVec->at(i).at(j);
		}
	}
}

Board::~Board()
{
	for (int i = 0; i < mNumRows; ++i)
	{
		mBoardVec[i].clear();
	}
	mBoardVec->clear();
}

void Board::UpdateBoard(MoveClass* pMove)
{

}

void Board::PrintBoard()
{
	std::cout << "\n\n The board is : ";
	for (int i = 0; i < mNumRows; ++i)
	{
		std::cout << std::endl;
		for (int j = 0; j < mNumCols; ++j)
		{
			std::vector <int> temporaryVec = mBoardVec->at(i);
			int temp = temporaryVec[j];

			switch (temp)
			{
				case BOARD_CLEAR:
					std::cout << " .";
					break;
				case BOARD_WALL:
					std::cout << " X";
					break;
				case BOARD_BLACK:
					std::cout << " B";
					break;
				case BOARD_WHITE:
					std::cout << " W";
					break;
			}
		}
	}
}