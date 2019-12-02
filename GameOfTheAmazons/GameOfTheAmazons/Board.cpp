//////////////////////////////////////////////////////////////
/**
@file Board.cpp
@author Romil Tendulkar
@date 12/02/2019
@brief This class will be used to store the board and the positions on it.
*/
//////////////////////////////////////////////////////////////

#include "Board.h"
#include "MoveClass.h"

Board::Board()
{
	mNumCols = 10;
	mNumRows = 10;
}

Board::Board(unsigned int nRows, unsigned int nCols)
{
	mNumCols = nCols;
	mNumRows = nRows;
}

Board::Board(Board* pBoard)
{
	mNumCols = pBoard->mNumCols;
	mNumRows = pBoard->mNumRows;
}

Board::~Board()
{

}

void Board::ValidateMove(MoveClass* pMove)
{

}

void Board::UpdateBoard(MoveClass* pMove)
{

}