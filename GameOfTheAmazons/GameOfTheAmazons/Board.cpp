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
	for (unsigned int i = 0; i < mNumRows; ++i)
	{
		for (unsigned int j = 0; j < mNumCols; ++j)
		{
			//copy each board position
			int temp = pBoard->mBoardVec->at(i).at(j);
			mBoardVec->at(i).at(j) = temp;
		}
	}
}

Board::~Board()
{
	for (unsigned int i = 0; i < mNumRows; ++i)
	{
		mBoardVec[i].clear();
	}
	mBoardVec->clear();
}

void Board::UpdateBoard(MoveClass* pMove)
{
	//update board based on move, here it is assumed that the move is valid.
	mBoardVec->at(pMove->mOrigX).at(pMove->mOrigY) = BOARD_CLEAR;
	mBoardVec->at(pMove->mNewX).at(pMove->mNewY) = pMove->mPlayerId;
	mBoardVec->at(pMove->mShootX).at(pMove->mShootY) = BOARD_WALL;
}

void Board::PrintBoard()
{
	std::cout << "\n\n The board is : ";
	for (unsigned int i = 0; i < mNumRows; ++i)
	{
		std::cout << std::endl;
		for (unsigned int j = 0; j < mNumCols; ++j)
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

int Board::FindScope(BoardIDs player)
{
	int ScopeCounter = 0;					//initialize the scope to 0
	for (int counter1 = 0; counter1 < (int)mNumRows; ++counter1)
	{
		for (int counter2 = 0; counter2 < (int)mNumCols; ++counter2)
		{
			if (mBoardVec->at(counter1).at(counter2) == player)
			{
				//if a piece for the particular player has been found
				//Calculate Scope
				int newX, newY;
				bool hitWall;

				//Check in the upwards direction
				newX = counter1;
				newY = counter2 - 1;
				hitWall = false;
				while (newY >= 0 && !hitWall)
				{
					if (mBoardVec->at(newX).at(newY) == BOARD_CLEAR)
					{
						//Calculate shooting scope
						int shootX, shootY;
						bool shitWall;

						//check in the upwards direction
						shootX = newX;
						shootY = newY - 1;
						shitWall = false;
						while (shootY >= 0 && !shitWall)
						{
							if (mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								ScopeCounter++;
							}
							else if(mBoardVec->at(shootX).at(shootY) == player && shootX == counter1 && shootY == counter2)
							{
								ScopeCounter++;
							}
							else
							{
								shitWall = true;
							}
							shootY--;
						}

						//calculate shooting in downwards
						shootX = newX;
						shootY = newY + 1;
						shitWall = false;
						while (shootY < (int)mNumRows && !shitWall)
						{
							if (mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								ScopeCounter++;
							}
							else if (mBoardVec->at(shootX).at(shootY) == player && shootX == counter1 && shootY == counter2)
							{
								ScopeCounter++;
							}
							else
							{
								shitWall = true;
							}
							shootY++;
						}

						//calculate shooting in left
						shootX = newX - 1;
						shootY = newY;
						shitWall = false;
						while (shootX >= 0 && !shitWall)
						{
							if (mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								ScopeCounter++;
							}
							else if (mBoardVec->at(shootX).at(shootY) == player && shootX == counter1 && shootY == counter2)
							{
								ScopeCounter++;
							}
							else
							{
								shitWall = true;
							}
							shootX--;
						}

						//calculate shooting in right
						shootX = newX + 1;
						shootY = newY;
						shitWall = false;
						while (shootX < (int)mNumCols && !shitWall)
						{
							if (mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								ScopeCounter++;
							}
							else if (mBoardVec->at(shootX).at(shootY) == player && shootX == counter1 && shootY == counter2)
							{
								ScopeCounter++;
							}
							else
							{
								shitWall = true;
							}
							shootX++;
						}

						//calculate shooting in the -- direction
						shootX = newX - 1;
						shootY = newY - 1;
						shitWall = false;
						while (shootX >= 0 && shootY >= 0 && !shitWall)
						{
							if (mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								ScopeCounter++;
							}
							else if (mBoardVec->at(shootX).at(shootY) == player && shootX == counter1 && shootY == counter2)
							{
								ScopeCounter++;
							}
							else
							{
								shitWall = true;
							}
							shootX--;
							shootY--;
						}

						//calculate shooting in the -+ direction
						shootX = newX - 1;
						shootY = newY + 1;
						shitWall = false;
						while (shootX >= 0 && shootY < (int)mNumRows && !shitWall)
						{
							if (mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								ScopeCounter++;
							}
							else if (mBoardVec->at(shootX).at(shootY) == player && shootX == counter1 && shootY == counter2)
							{
								ScopeCounter++;
							}
							else
							{
								shitWall = true;
							}
							shootX--;
							shootY++;
						}

						//calculate shooting in the ++ direction
						shootX = newX + 1;
						shootY = newY + 1;
						shitWall = false;
						while (shootX < (int)mNumCols && shootY < (int)mNumRows && !shitWall)
						{
							if (mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								ScopeCounter++;
							}
							else if (mBoardVec->at(shootX).at(shootY) == player && shootX == counter1 && shootY == counter2)
							{
								ScopeCounter++;
							}
							else
							{
								shitWall = true;
							}
							shootX++;
							shootY++;
						}

						//calculate shooting in the +- direction
						shootX = newX + 1;
						shootY = newY - 1;
						shitWall = false;
						while (shootX < (int)mNumCols && shootY >= 0 && !shitWall)
						{
							if (mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								ScopeCounter++;
							}
							else if (mBoardVec->at(shootX).at(shootY) == player && shootX == counter1 && shootY == counter2)
							{
								ScopeCounter++;
							}
							else
							{
								shitWall = true;
							}
							shootX++;
							shootY--;
						}

					}
					else
					{
						hitWall = true;
					}
					newY--;
				}

				//Check in the Downwards direction
				newX = counter1;
				newY = counter2 + 1;
				hitWall = false;
				while (newY < (int)mNumRows && !hitWall)
				{
					if (mBoardVec->at(newX).at(newY) == BOARD_CLEAR)
					{
						//Calculate shooting scope
						int shootX, shootY;
						bool shitWall;

						//check in the upwards direction
						shootX = newX;
						shootY = newY - 1;
						shitWall = false;
						while (shootY >= 0 && !shitWall)
						{
							if (mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								ScopeCounter++;
							}
							else if (mBoardVec->at(shootX).at(shootY) == player && shootX == counter1 && shootY == counter2)
							{
								ScopeCounter++;
							}
							else
							{
								shitWall = true;
							}
							shootY--;
						}

						//calculate shooting in downwards
						shootX = newX;
						shootY = newY + 1;
						shitWall = false;
						while (shootY < (int)mNumRows && !shitWall)
						{
							if (mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								ScopeCounter++;
							}
							else if (mBoardVec->at(shootX).at(shootY) == player && shootX == counter1 && shootY == counter2)
							{
								ScopeCounter++;
							}
							else
							{
								shitWall = true;
							}
							shootY++;
						}

						//calculate shooting in left
						shootX = newX - 1;
						shootY = newY;
						shitWall = false;
						while (shootX >= 0 && !shitWall)
						{
							if (mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								ScopeCounter++;
							}
							else if (mBoardVec->at(shootX).at(shootY) == player && shootX == counter1 && shootY == counter2)
							{
								ScopeCounter++;
							}
							else
							{
								shitWall = true;
							}
							shootX--;
						}

						//calculate shooting in right
						shootX = newX + 1;
						shootY = newY;
						shitWall = false;
						while (shootX < (int)mNumCols && !shitWall)
						{
							if (mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								ScopeCounter++;
							}
							else if (mBoardVec->at(shootX).at(shootY) == player && shootX == counter1 && shootY == counter2)
							{
								ScopeCounter++;
							}
							else
							{
								shitWall = true;
							}
							shootX++;
						}

						//calculate shooting in the -- direction
						shootX = newX - 1;
						shootY = newY - 1;
						shitWall = false;
						while (shootX >= 0 && shootY >= 0 && !shitWall)
						{
							if (mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								ScopeCounter++;
							}
							else if (mBoardVec->at(shootX).at(shootY) == player && shootX == counter1 && shootY == counter2)
							{
								ScopeCounter++;
							}
							else
							{
								shitWall = true;
							}
							shootX--;
							shootY--;
						}

						//calculate shooting in the -+ direction
						shootX = newX - 1;
						shootY = newY + 1;
						shitWall = false;
						while (shootX >= 0 && shootY < (int)mNumRows && !shitWall)
						{
							if (mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								ScopeCounter++;
							}
							else if (mBoardVec->at(shootX).at(shootY) == player && shootX == counter1 && shootY == counter2)
							{
								ScopeCounter++;
							}
							else
							{
								shitWall = true;
							}
							shootX--;
							shootY++;
						}

						//calculate shooting in the ++ direction
						shootX = newX + 1;
						shootY = newY + 1;
						shitWall = false;
						while (shootX < (int)mNumCols && shootY < (int)mNumRows && !shitWall)
						{
							if (mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								ScopeCounter++;
							}
							else if (mBoardVec->at(shootX).at(shootY) == player && shootX == counter1 && shootY == counter2)
							{
								ScopeCounter++;
							}
							else
							{
								shitWall = true;
							}
							shootX++;
							shootY++;
						}

						//calculate shooting in the +- direction
						shootX = newX + 1;
						shootY = newY - 1;
						shitWall = false;
						while (shootX < (int)mNumCols && shootY >= 0 && !shitWall)
						{
							if (mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								ScopeCounter++;
							}
							else if (mBoardVec->at(shootX).at(shootY) == player && shootX == counter1 && shootY == counter2)
							{
								ScopeCounter++;
							}
							else
							{
								shitWall = true;
							}
							shootX++;
							shootY--;
						}
					}
					else
					{
						hitWall = true;
					}
					newY++;
				}

				//Check in Right Direction
				newX = counter1 + 1;
				newY = counter2;
				hitWall = false;
				while (newX < (int)mNumCols && !hitWall)
				{
					if (mBoardVec->at(newX).at(newY) == BOARD_CLEAR)
					{
						//Calculate shooting scope
						int shootX, shootY;
						bool shitWall;

						//check in the upwards direction
						shootX = newX;
						shootY = newY - 1;
						shitWall = false;
						while (shootY >= 0 && !shitWall)
						{
							if (mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								ScopeCounter++;
							}
							else if (mBoardVec->at(shootX).at(shootY) == player && shootX == counter1 && shootY == counter2)
							{
								ScopeCounter++;
							}
							else
							{
								shitWall = true;
							}
							shootY--;
						}

						//calculate shooting in downwards
						shootX = newX;
						shootY = newY + 1;
						shitWall = false;
						while (shootY < (int)mNumRows && !shitWall)
						{
							if (mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								ScopeCounter++;
							}
							else if (mBoardVec->at(shootX).at(shootY) == player && shootX == counter1 && shootY == counter2)
							{
								ScopeCounter++;
							}
							else
							{
								shitWall = true;
							}
							shootY++;
						}

						//calculate shooting in left
						shootX = newX - 1;
						shootY = newY;
						shitWall = false;
						while (shootX >= 0 && !shitWall)
						{
							if (mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								ScopeCounter++;
							}
							else if (mBoardVec->at(shootX).at(shootY) == player && shootX == counter1 && shootY == counter2)
							{
								ScopeCounter++;
							}
							else
							{
								shitWall = true;
							}
							shootX--;
						}

						//calculate shooting in right
						shootX = newX + 1;
						shootY = newY;
						shitWall = false;
						while (shootX < (int)mNumCols && !shitWall)
						{
							if (mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								ScopeCounter++;
							}
							else if (mBoardVec->at(shootX).at(shootY) == player && shootX == counter1 && shootY == counter2)
							{
								ScopeCounter++;
							}
							else
							{
								shitWall = true;
							}
							shootX++;
						}

						//calculate shooting in the -- direction
						shootX = newX - 1;
						shootY = newY - 1;
						shitWall = false;
						while (shootX >= 0 && shootY >= 0 && !shitWall)
						{
							if (mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								ScopeCounter++;
							}
							else if (mBoardVec->at(shootX).at(shootY) == player && shootX == counter1 && shootY == counter2)
							{
								ScopeCounter++;
							}
							else
							{
								shitWall = true;
							}
							shootX--;
							shootY--;
						}

						//calculate shooting in the -+ direction
						shootX = newX - 1;
						shootY = newY + 1;
						shitWall = false;
						while (shootX >= 0 && shootY < (int)mNumRows && !shitWall)
						{
							if (mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								ScopeCounter++;
							}
							else if (mBoardVec->at(shootX).at(shootY) == player && shootX == counter1 && shootY == counter2)
							{
								ScopeCounter++;
							}
							else
							{
								shitWall = true;
							}
							shootX--;
							shootY++;
						}

						//calculate shooting in the ++ direction
						shootX = newX + 1;
						shootY = newY + 1;
						shitWall = false;
						while (shootX < (int)mNumCols && shootY < (int)mNumRows && !shitWall)
						{
							if (mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								ScopeCounter++;
							}
							else if (mBoardVec->at(shootX).at(shootY) == player && shootX == counter1 && shootY == counter2)
							{
								ScopeCounter++;
							}
							else
							{
								shitWall = true;
							}
							shootX++;
							shootY++;
						}

						//calculate shooting in the +- direction
						shootX = newX + 1;
						shootY = newY - 1;
						shitWall = false;
						while (shootX < (int)mNumCols && shootY >= 0 && !shitWall)
						{
							if (mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								ScopeCounter++;
							}
							else if (mBoardVec->at(shootX).at(shootY) == player && shootX == counter1 && shootY == counter2)
							{
								ScopeCounter++;
							}
							else
							{
								shitWall = true;
							}
							shootX++;
							shootY--;
						}
					}
					else
					{
						hitWall = true;
					}
					newX++;
				}

				//Check in the Left Direction
				newX = counter1 - 1;
				newY = counter2;
				hitWall = false;
				while (newX >= 0 && !hitWall)
				{
					if (mBoardVec->at(newX).at(newY) == BOARD_CLEAR)
					{
						//Calculate shooting scope
						int shootX, shootY;
						bool shitWall;

						//check in the upwards direction
						shootX = newX;
						shootY = newY - 1;
						shitWall = false;
						while (shootY >= 0 && !shitWall)
						{
							if (mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								ScopeCounter++;
							}
							else if (mBoardVec->at(shootX).at(shootY) == player && shootX == counter1 && shootY == counter2)
							{
								ScopeCounter++;
							}
							else
							{
								shitWall = true;
							}
							shootY--;
						}

						//calculate shooting in downwards
						shootX = newX;
						shootY = newY + 1;
						shitWall = false;
						while (shootY < (int)mNumRows && !shitWall)
						{
							if (mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								ScopeCounter++;
							}
							else if (mBoardVec->at(shootX).at(shootY) == player && shootX == counter1 && shootY == counter2)
							{
								ScopeCounter++;
							}
							else
							{
								shitWall = true;
							}
							shootY++;
						}

						//calculate shooting in left
						shootX = newX - 1;
						shootY = newY;
						shitWall = false;
						while (shootX >= 0 && !shitWall)
						{
							if (mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								ScopeCounter++;
							}
							else if (mBoardVec->at(shootX).at(shootY) == player && shootX == counter1 && shootY == counter2)
							{
								ScopeCounter++;
							}
							else
							{
								shitWall = true;
							}
							shootX--;
						}

						//calculate shooting in right
						shootX = newX + 1;
						shootY = newY;
						shitWall = false;
						while (shootX < (int)mNumCols && !shitWall)
						{
							if (mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								ScopeCounter++;
							}
							else if (mBoardVec->at(shootX).at(shootY) == player && shootX == counter1 && shootY == counter2)
							{
								ScopeCounter++;
							}
							else
							{
								shitWall = true;
							}
							shootX++;
						}

						//calculate shooting in the -- direction
						shootX = newX - 1;
						shootY = newY - 1;
						shitWall = false;
						while (shootX >= 0 && shootY >= 0 && !shitWall)
						{
							if (mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								ScopeCounter++;
							}
							else if (mBoardVec->at(shootX).at(shootY) == player && shootX == counter1 && shootY == counter2)
							{
								ScopeCounter++;
							}
							else
							{
								shitWall = true;
							}
							shootX--;
							shootY--;
						}

						//calculate shooting in the -+ direction
						shootX = newX - 1;
						shootY = newY + 1;
						shitWall = false;
						while (shootX >= 0 && shootY < (int)mNumRows && !shitWall)
						{
							if (mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								ScopeCounter++;
							}
							else if (mBoardVec->at(shootX).at(shootY) == player && shootX == counter1 && shootY == counter2)
							{
								ScopeCounter++;
							}
							else
							{
								shitWall = true;
							}
							shootX--;
							shootY++;
						}

						//calculate shooting in the ++ direction
						shootX = newX + 1;
						shootY = newY + 1;
						shitWall = false;
						while (shootX < (int)mNumCols && shootY < (int)mNumRows && !shitWall)
						{
							if (mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								ScopeCounter++;
							}
							else if (mBoardVec->at(shootX).at(shootY) == player && shootX == counter1 && shootY == counter2)
							{
								ScopeCounter++;
							}
							else
							{
								shitWall = true;
							}
							shootX++;
							shootY++;
						}

						//calculate shooting in the +- direction
						shootX = newX + 1;
						shootY = newY - 1;
						shitWall = false;
						while (shootX < (int)mNumCols && shootY >= 0 && !shitWall)
						{
							if (mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								ScopeCounter++;
							}
							else if (mBoardVec->at(shootX).at(shootY) == player && shootX == counter1 && shootY == counter2)
							{
								ScopeCounter++;
							}
							else
							{
								shitWall = true;
							}
							shootX++;
							shootY--;
						}
					}
					else
					{
						hitWall = true;
					}
					newX--;
				}

				//Check in the -- Direction
				newX = counter1 - 1;
				newY = counter2 - 1;
				hitWall = false;
				while (newX >= 0 && newY>=0 && !hitWall)
				{
					if (mBoardVec->at(newX).at(newY) == BOARD_CLEAR)
					{
						//Calculate shooting scope
						int shootX, shootY;
						bool shitWall;

						//check in the upwards direction
						shootX = newX;
						shootY = newY - 1;
						shitWall = false;
						while (shootY >= 0 && !shitWall)
						{
							if (mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								ScopeCounter++;
							}
							else if (mBoardVec->at(shootX).at(shootY) == player && shootX == counter1 && shootY == counter2)
							{
								ScopeCounter++;
							}
							else
							{
								shitWall = true;
							}
							shootY--;
						}

						//calculate shooting in downwards
						shootX = newX;
						shootY = newY + 1;
						shitWall = false;
						while (shootY < (int)mNumRows && !shitWall)
						{
							if (mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								ScopeCounter++;
							}
							else if (mBoardVec->at(shootX).at(shootY) == player && shootX == counter1 && shootY == counter2)
							{
								ScopeCounter++;
							}
							else
							{
								shitWall = true;
							}
							shootY++;
						}

						//calculate shooting in left
						shootX = newX - 1;
						shootY = newY;
						shitWall = false;
						while (shootX >= 0 && !shitWall)
						{
							if (mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								ScopeCounter++;
							}
							else if (mBoardVec->at(shootX).at(shootY) == player && shootX == counter1 && shootY == counter2)
							{
								ScopeCounter++;
							}
							else
							{
								shitWall = true;
							}
							shootX--;
						}

						//calculate shooting in right
						shootX = newX + 1;
						shootY = newY;
						shitWall = false;
						while (shootX < (int)mNumCols && !shitWall)
						{
							if (mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								ScopeCounter++;
							}
							else if (mBoardVec->at(shootX).at(shootY) == player && shootX == counter1 && shootY == counter2)
							{
								ScopeCounter++;
							}
							else
							{
								shitWall = true;
							}
							shootX++;
						}

						//calculate shooting in the -- direction
						shootX = newX - 1;
						shootY = newY - 1;
						shitWall = false;
						while (shootX >= 0 && shootY >= 0 && !shitWall)
						{
							if (mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								ScopeCounter++;
							}
							else if (mBoardVec->at(shootX).at(shootY) == player && shootX == counter1 && shootY == counter2)
							{
								ScopeCounter++;
							}
							else
							{
								shitWall = true;
							}
							shootX--;
							shootY--;
						}

						//calculate shooting in the -+ direction
						shootX = newX - 1;
						shootY = newY + 1;
						shitWall = false;
						while (shootX >= 0 && shootY < (int)mNumRows && !shitWall)
						{
							if (mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								ScopeCounter++;
							}
							else if (mBoardVec->at(shootX).at(shootY) == player && shootX == counter1 && shootY == counter2)
							{
								ScopeCounter++;
							}
							else
							{
								shitWall = true;
							}
							shootX--;
							shootY++;
						}

						//calculate shooting in the ++ direction
						shootX = newX + 1;
						shootY = newY + 1;
						shitWall = false;
						while (shootX < (int)mNumCols && shootY < (int)mNumRows && !shitWall)
						{
							if (mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								ScopeCounter++;
							}
							else if (mBoardVec->at(shootX).at(shootY) == player && shootX == counter1 && shootY == counter2)
							{
								ScopeCounter++;
							}
							else
							{
								shitWall = true;
							}
							shootX++;
							shootY++;
						}

						//calculate shooting in the +- direction
						shootX = newX + 1;
						shootY = newY - 1;
						shitWall = false;
						while (shootX < (int)mNumCols && shootY >= 0 && !shitWall)
						{
							if (mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								ScopeCounter++;
							}
							else if (mBoardVec->at(shootX).at(shootY) == player && shootX == counter1 && shootY == counter2)
							{
								ScopeCounter++;
							}
							else
							{
								shitWall = true;
							}
							shootX++;
							shootY--;
						}
					}
					else
					{
						hitWall = true;
					}
					newX--;
					newY--;
				}

				//Check in the ++ Direction
				newX = counter1 + 1;
				newY = counter2 + 1;
				hitWall = false;
				while (newX < (int)mNumCols && newY < (int)mNumRows && !hitWall)
				{
					if (mBoardVec->at(newX).at(newY) == BOARD_CLEAR)
					{
						//Calculate shooting scope
						int shootX, shootY;
						bool shitWall;

						//check in the upwards direction
						shootX = newX;
						shootY = newY - 1;
						shitWall = false;
						while (shootY >= 0 && !shitWall)
						{
							if (mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								ScopeCounter++;
							}
							else if (mBoardVec->at(shootX).at(shootY) == player && shootX == counter1 && shootY == counter2)
							{
								ScopeCounter++;
							}
							else
							{
								shitWall = true;
							}
							shootY--;
						}

						//calculate shooting in downwards
						shootX = newX;
						shootY = newY + 1;
						shitWall = false;
						while (shootY < (int)mNumRows && !shitWall)
						{
							if (mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								ScopeCounter++;
							}
							else if (mBoardVec->at(shootX).at(shootY) == player && shootX == counter1 && shootY == counter2)
							{
								ScopeCounter++;
							}
							else
							{
								shitWall = true;
							}
							shootY++;
						}

						//calculate shooting in left
						shootX = newX - 1;
						shootY = newY;
						shitWall = false;
						while (shootX >= 0 && !shitWall)
						{
							if (mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								ScopeCounter++;
							}
							else if (mBoardVec->at(shootX).at(shootY) == player && shootX == counter1 && shootY == counter2)
							{
								ScopeCounter++;
							}
							else
							{
								shitWall = true;
							}
							shootX--;
						}

						//calculate shooting in right
						shootX = newX + 1;
						shootY = newY;
						shitWall = false;
						while (shootX < (int)mNumCols && !shitWall)
						{
							if (mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								ScopeCounter++;
							}
							else if (mBoardVec->at(shootX).at(shootY) == player && shootX == counter1 && shootY == counter2)
							{
								ScopeCounter++;
							}
							else
							{
								shitWall = true;
							}
							shootX++;
						}

						//calculate shooting in the -- direction
						shootX = newX - 1;
						shootY = newY - 1;
						shitWall = false;
						while (shootX >= 0 && shootY >= 0 && !shitWall)
						{
							if (mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								ScopeCounter++;
							}
							else if (mBoardVec->at(shootX).at(shootY) == player && shootX == counter1 && shootY == counter2)
							{
								ScopeCounter++;
							}
							else
							{
								shitWall = true;
							}
							shootX--;
							shootY--;
						}

						//calculate shooting in the -+ direction
						shootX = newX - 1;
						shootY = newY + 1;
						shitWall = false;
						while (shootX >= 0 && shootY < (int)mNumRows && !shitWall)
						{
							if (mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								ScopeCounter++;
							}
							else if (mBoardVec->at(shootX).at(shootY) == player && shootX == counter1 && shootY == counter2)
							{
								ScopeCounter++;
							}
							else
							{
								shitWall = true;
							}
							shootX--;
							shootY++;
						}

						//calculate shooting in the ++ direction
						shootX = newX + 1;
						shootY = newY + 1;
						shitWall = false;
						while (shootX < (int)mNumCols && shootY < (int)mNumRows && !shitWall)
						{
							if (mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								ScopeCounter++;
							}
							else if (mBoardVec->at(shootX).at(shootY) == player && shootX == counter1 && shootY == counter2)
							{
								ScopeCounter++;
							}
							else
							{
								shitWall = true;
							}
							shootX++;
							shootY++;
						}

						//calculate shooting in the +- direction
						shootX = newX + 1;
						shootY = newY - 1;
						shitWall = false;
						while (shootX < (int)mNumCols && shootY >= 0 && !shitWall)
						{
							if (mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								ScopeCounter++;
							}
							else if (mBoardVec->at(shootX).at(shootY) == player && shootX == counter1 && shootY == counter2)
							{
								ScopeCounter++;
							}
							else
							{
								shitWall = true;
							}
							shootX++;
							shootY--;
						}
					}
					else
					{
						hitWall = true;
					}
					newX++;
					newY++;
				}

				//Check in the -+ Direction
				newX = counter1 - 1;
				newY = counter2 + 1;
				hitWall = false;
				while (newX >= 0 && newY < (int)mNumRows && !hitWall)
				{
					if (mBoardVec->at(newX).at(newY) == BOARD_CLEAR)
					{
						//Calculate shooting scope
						int shootX, shootY;
						bool shitWall;

						//check in the upwards direction
						shootX = newX;
						shootY = newY - 1;
						shitWall = false;
						while (shootY >= 0 && !shitWall)
						{
							if (mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								ScopeCounter++;
							}
							else if (mBoardVec->at(shootX).at(shootY) == player && shootX == counter1 && shootY == counter2)
							{
								ScopeCounter++;
							}
							else
							{
								shitWall = true;
							}
							shootY--;
						}

						//calculate shooting in downwards
						shootX = newX;
						shootY = newY + 1;
						shitWall = false;
						while (shootY < (int)mNumRows && !shitWall)
						{
							if (mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								ScopeCounter++;
							}
							else if (mBoardVec->at(shootX).at(shootY) == player && shootX == counter1 && shootY == counter2)
							{
								ScopeCounter++;
							}
							else
							{
								shitWall = true;
							}
							shootY++;
						}

						//calculate shooting in left
						shootX = newX - 1;
						shootY = newY;
						shitWall = false;
						while (shootX >= 0 && !shitWall)
						{
							if (mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								ScopeCounter++;
							}
							else if (mBoardVec->at(shootX).at(shootY) == player && shootX == counter1 && shootY == counter2)
							{
								ScopeCounter++;
							}
							else
							{
								shitWall = true;
							}
							shootX--;
						}

						//calculate shooting in right
						shootX = newX + 1;
						shootY = newY;
						shitWall = false;
						while (shootX < (int)mNumCols && !shitWall)
						{
							if (mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								ScopeCounter++;
							}
							else if (mBoardVec->at(shootX).at(shootY) == player && shootX == counter1 && shootY == counter2)
							{
								ScopeCounter++;
							}
							else
							{
								shitWall = true;
							}
							shootX++;
						}

						//calculate shooting in the -- direction
						shootX = newX - 1;
						shootY = newY - 1;
						shitWall = false;
						while (shootX >= 0 && shootY >= 0 && !shitWall)
						{
							if (mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								ScopeCounter++;
							}
							else if (mBoardVec->at(shootX).at(shootY) == player && shootX == counter1 && shootY == counter2)
							{
								ScopeCounter++;
							}
							else
							{
								shitWall = true;
							}
							shootX--;
							shootY--;
						}

						//calculate shooting in the -+ direction
						shootX = newX - 1;
						shootY = newY + 1;
						shitWall = false;
						while (shootX >= 0 && shootY < (int)mNumRows && !shitWall)
						{
							if (mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								ScopeCounter++;
							}
							else if (mBoardVec->at(shootX).at(shootY) == player && shootX == counter1 && shootY == counter2)
							{
								ScopeCounter++;
							}
							else
							{
								shitWall = true;
							}
							shootX--;
							shootY++;
						}

						//calculate shooting in the ++ direction
						shootX = newX + 1;
						shootY = newY + 1;
						shitWall = false;
						while (shootX < (int)mNumCols && shootY < (int)mNumRows && !shitWall)
						{
							if (mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								ScopeCounter++;
							}
							else if (mBoardVec->at(shootX).at(shootY) == player && shootX == counter1 && shootY == counter2)
							{
								ScopeCounter++;
							}
							else
							{
								shitWall = true;
							}
							shootX++;
							shootY++;
						}

						//calculate shooting in the +- direction
						shootX = newX + 1;
						shootY = newY - 1;
						shitWall = false;
						while (shootX < (int)mNumCols && shootY >= 0 && !shitWall)
						{
							if (mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								ScopeCounter++;
							}
							else if (mBoardVec->at(shootX).at(shootY) == player && shootX == counter1 && shootY == counter2)
							{
								ScopeCounter++;
							}
							else
							{
								shitWall = true;
							}
							shootX++;
							shootY--;
						}
					}
					else
					{
						hitWall = true;
					}
					newX--;
					newY++;
				}

				//Check in the +- Direction
				newX = counter1 + 1;
				newY = counter2 - 1;
				hitWall = false;
				while (newX < (int)mNumCols && newY >= 0 && !hitWall)
				{
					if (mBoardVec->at(newX).at(newY) == BOARD_CLEAR)
					{
						//Calculate shooting scope
						int shootX, shootY;
						bool shitWall;

						//check in the upwards direction
						shootX = newX;
						shootY = newY - 1;
						shitWall = false;
						while (shootY >= 0 && !shitWall)
						{
							if (mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								ScopeCounter++;
							}
							else if (mBoardVec->at(shootX).at(shootY) == player && shootX == counter1 && shootY == counter2)
							{
								ScopeCounter++;
							}
							else
							{
								shitWall = true;
							}
							shootY--;
						}

						//calculate shooting in downwards
						shootX = newX;
						shootY = newY + 1;
						shitWall = false;
						while (shootY < (int)mNumRows && !shitWall)
						{
							if (mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								ScopeCounter++;
							}
							else if (mBoardVec->at(shootX).at(shootY) == player && shootX == counter1 && shootY == counter2)
							{
								ScopeCounter++;
							}
							else
							{
								shitWall = true;
							}
							shootY++;
						}

						//calculate shooting in left
						shootX = newX - 1;
						shootY = newY;
						shitWall = false;
						while (shootX >= 0 && !shitWall)
						{
							if (mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								ScopeCounter++;
							}
							else if (mBoardVec->at(shootX).at(shootY) == player && shootX == counter1 && shootY == counter2)
							{
								ScopeCounter++;
							}
							else
							{
								shitWall = true;
							}
							shootX--;
						}

						//calculate shooting in right
						shootX = newX + 1;
						shootY = newY;
						shitWall = false;
						while (shootX < (int)mNumCols && !shitWall)
						{
							if (mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								ScopeCounter++;
							}
							else if (mBoardVec->at(shootX).at(shootY) == player && shootX == counter1 && shootY == counter2)
							{
								ScopeCounter++;
							}
							else
							{
								shitWall = true;
							}
							shootX++;
						}

						//calculate shooting in the -- direction
						shootX = newX - 1;
						shootY = newY - 1;
						shitWall = false;
						while (shootX >= 0 && shootY >= 0 && !shitWall)
						{
							if (mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								ScopeCounter++;
							}
							else if (mBoardVec->at(shootX).at(shootY) == player && shootX == counter1 && shootY == counter2)
							{
								ScopeCounter++;
							}
							else
							{
								shitWall = true;
							}
							shootX--;
							shootY--;
						}

						//calculate shooting in the -+ direction
						shootX = newX - 1;
						shootY = newY + 1;
						shitWall = false;
						while (shootX >= 0 && shootY < (int)mNumRows && !shitWall)
						{
							if (mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								ScopeCounter++;
							}
							else if (mBoardVec->at(shootX).at(shootY) == player && shootX == counter1 && shootY == counter2)
							{
								ScopeCounter++;
							}
							else
							{
								shitWall = true;
							}
							shootX--;
							shootY++;
						}

						//calculate shooting in the ++ direction
						shootX = newX + 1;
						shootY = newY + 1;
						shitWall = false;
						while (shootX < (int)mNumCols && shootY < (int)mNumRows && !shitWall)
						{
							if (mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								ScopeCounter++;
							}
							else if (mBoardVec->at(shootX).at(shootY) == player && shootX == counter1 && shootY == counter2)
							{
								ScopeCounter++;
							}
							else
							{
								shitWall = true;
							}
							shootX++;
							shootY++;
						}

						//calculate shooting in the +- direction
						shootX = newX + 1;
						shootY = newY - 1;
						shitWall = false;
						while (shootX < (int)mNumCols && shootY >= 0 && !shitWall)
						{
							if (mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								ScopeCounter++;
							}
							else if (mBoardVec->at(shootX).at(shootY) == player && shootX == counter1 && shootY == counter2)
							{
								ScopeCounter++;
							}
							else
							{
								shitWall = true;
							}
							shootX++;
							shootY--;
						}
					}
					else
					{
						hitWall = true;
					}
					newX++;
					newY--;
				}
			}
		}
	}
	return ScopeCounter;
}