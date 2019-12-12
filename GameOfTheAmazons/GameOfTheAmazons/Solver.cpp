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

	mBestAI = nullptr;
	mBestPlayer = nullptr;

	isSatisfied = false;
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
	
}

void Solver::Solve()
{

	Search(mCurrentBoard, 0, 0);
	if(mBestAI != nullptr)
	{
		std::cout << "\n Best Move for AI has Scope: " << mBestAI->Scope;
	}
	
	
	
}

int Solver::Search(Board *pBoard, int depthTo, int currDepth)
{
	bool isMax = false;
	

	if (currDepth % 2)
	{
		isMax = true;
		int currScope = pBoard->FindScope(BOARD_BLACK);
		if (currScope == 0)
		{
			int temp = pBoard->FindScope(BOARD_WHITE);
			if (temp == 0)
			{
				return -1;
			}
			else
			{
				return -temp;
			}
		}
		if (depthTo != 0)
		{
			if (depthTo == currDepth)
			{
				return currScope;
			}
		}
	}
	else
	{
		int currScope = pBoard->FindScope(BOARD_WHITE);
		if (currScope == 0)
		{
			int temp = pBoard->FindScope(BOARD_BLACK);
			if (temp == 0)
			{
				return 1;
			}
			else
			{
				return temp;
			}
			return 0;
		}
		if (depthTo != 0)
		{
			if (depthTo == currDepth)
			{
				return currScope;
			}
		}
	}
	Board *tBoard = new Board(pBoard);
	MoveClass* mMove;
	if (isMax)
	{
		mMove = GetBestMove(pBoard, BOARD_BLACK, depthTo, currDepth);
		//GetBestMove(pBoard, BOARD_BLACK);
		if (currDepth == 0)
		{
			mBestAI = new MoveClass(mMove);
		}
	}
	else
	{
		mMove = GetBestMove(pBoard, BOARD_WHITE, depthTo, currDepth);
		//GetBestMove(pBoard, BOARD_WHITE);
	}
	return 0;
}

MoveClass* Solver::GetBestMove(Board *pBoard, BoardIDs currPlayer, int depthTo, int currDepth)
{
	int mNumRows = pBoard->mNumRows;
	int mNumCols = pBoard->mNumCols;
	MoveClass *temp = new MoveClass(0,0,0,0,0,0,currPlayer);
	bool isMoveSet = false;
	
	for (int counter1 = 0; counter1 < mNumRows; ++counter1)
	{
		for (int counter2 = 0; counter2 < mNumCols; ++counter2)
		{
			if (pBoard->mBoardVec->at(counter1).at(counter2) == currPlayer)
			{
				//Calculate Scope
				int newX, newY;
				bool hitWall;

				//Check in the upwards direction
				newX = counter1;
				newY = counter2 - 1;
				hitWall = false;
				while (newY >= 0 && !hitWall)
				{
					if (pBoard->mBoardVec->at(newX).at(newY) == BOARD_CLEAR)
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
							if (pBoard->mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{ 
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
							}
							else if (pBoard->mBoardVec->at(shootX).at(shootY) == currPlayer && shootX == counter1 && shootY == counter2)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
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
						while (shootY < mNumRows && !shitWall)
						{
							if (pBoard->mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
							}
							else if (pBoard->mBoardVec->at(shootX).at(shootY) == currPlayer && shootX == counter1 && shootY == counter2)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
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
							if (pBoard->mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
							}
							else if (pBoard->mBoardVec->at(shootX).at(shootY) == currPlayer && shootX == counter1 && shootY == counter2)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
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
						while (shootX < mNumCols && !shitWall)
						{
							if (pBoard->mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
							}
							else if (pBoard->mBoardVec->at(shootX).at(shootY) == currPlayer && shootX == counter1 && shootY == counter2)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
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
							if (pBoard->mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
							}
							else if (pBoard->mBoardVec->at(shootX).at(shootY) == currPlayer && shootX == counter1 && shootY == counter2)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
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
						while (shootX >= 0 && shootY < mNumRows && !shitWall)
						{
							if (pBoard->mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
							}
							else if (pBoard->mBoardVec->at(shootX).at(shootY) == currPlayer && shootX == counter1 && shootY == counter2)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
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
						while (shootX < mNumCols && shootY < mNumRows && !shitWall)
						{
							if (pBoard->mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
							}
							else if (pBoard->mBoardVec->at(shootX).at(shootY) == currPlayer && shootX == counter1 && shootY == counter2)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
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
						while (shootX < mNumCols && shootY >= 0 && !shitWall)
						{
							if (pBoard->mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
							}
							else if (pBoard->mBoardVec->at(shootX).at(shootY) == currPlayer && shootX == counter1 && shootY == counter2)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
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
				while (newY < mNumRows && !hitWall)
				{
					if (pBoard->mBoardVec->at(newX).at(newY) == BOARD_CLEAR)
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
							if (pBoard->mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
							}
							else if (pBoard->mBoardVec->at(shootX).at(shootY) == currPlayer && shootX == counter1 && shootY == counter2)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
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
						while (shootY < mNumRows && !shitWall)
						{
							if (pBoard->mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
							}
							else if (pBoard->mBoardVec->at(shootX).at(shootY) == currPlayer && shootX == counter1 && shootY == counter2)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
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
							if (pBoard->mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
							}
							else if (pBoard->mBoardVec->at(shootX).at(shootY) == currPlayer && shootX == counter1 && shootY == counter2)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
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
						while (shootX < mNumCols && !shitWall)
						{
							if (pBoard->mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
							}
							else if (pBoard->mBoardVec->at(shootX).at(shootY) == currPlayer && shootX == counter1 && shootY == counter2)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
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
							if (pBoard->mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
							}
							else if (pBoard->mBoardVec->at(shootX).at(shootY) == currPlayer && shootX == counter1 && shootY == counter2)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
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
						while (shootX >= 0 && shootY < mNumRows && !shitWall)
						{
							if (pBoard->mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
							}
							else if (pBoard->mBoardVec->at(shootX).at(shootY) == currPlayer && shootX == counter1 && shootY == counter2)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
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
						while (shootX < mNumCols && shootY < mNumRows && !shitWall)
						{
							if (pBoard->mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
							}
							else if (pBoard->mBoardVec->at(shootX).at(shootY) == currPlayer && shootX == counter1 && shootY == counter2)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
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
						while (shootX < mNumCols && shootY >= 0 && !shitWall)
						{
							if (pBoard->mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
							}
							else if (pBoard->mBoardVec->at(shootX).at(shootY) == currPlayer && shootX == counter1 && shootY == counter2)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
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
				while (newX < mNumCols && !hitWall)
				{
					if (pBoard->mBoardVec->at(newX).at(newY) == BOARD_CLEAR)
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
							if (pBoard->mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
							}
							else if (pBoard->mBoardVec->at(shootX).at(shootY) == currPlayer && shootX == counter1 && shootY == counter2)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
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
						while (shootY < mNumRows && !shitWall)
						{
							if (pBoard->mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
							}
							else if (pBoard->mBoardVec->at(shootX).at(shootY) == currPlayer && shootX == counter1 && shootY == counter2)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
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
							if (pBoard->mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
							}
							else if (pBoard->mBoardVec->at(shootX).at(shootY) == currPlayer && shootX == counter1 && shootY == counter2)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
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
						while (shootX < mNumCols && !shitWall)
						{
							if (pBoard->mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
							}
							else if (pBoard->mBoardVec->at(shootX).at(shootY) == currPlayer && shootX == counter1 && shootY == counter2)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
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
							if (pBoard->mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
							}
							else if (pBoard->mBoardVec->at(shootX).at(shootY) == currPlayer && shootX == counter1 && shootY == counter2)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
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
						while (shootX >= 0 && shootY < mNumRows && !shitWall)
						{
							if (pBoard->mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
							}
							else if (pBoard->mBoardVec->at(shootX).at(shootY) == currPlayer && shootX == counter1 && shootY == counter2)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
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
						while (shootX < mNumCols && shootY < mNumRows && !shitWall)
						{
							if (pBoard->mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
							}
							else if (pBoard->mBoardVec->at(shootX).at(shootY) == currPlayer && shootX == counter1 && shootY == counter2)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
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
						while (shootX < mNumCols && shootY >= 0 && !shitWall)
						{
							if (pBoard->mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
							}
							else if (pBoard->mBoardVec->at(shootX).at(shootY) == currPlayer && shootX == counter1 && shootY == counter2)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
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
					if (pBoard->mBoardVec->at(newX).at(newY) == BOARD_CLEAR)
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
							if (pBoard->mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
							}
							else if (pBoard->mBoardVec->at(shootX).at(shootY) == currPlayer && shootX == counter1 && shootY == counter2)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
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
						while (shootY < mNumRows && !shitWall)
						{
							if (pBoard->mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
							}
							else if (pBoard->mBoardVec->at(shootX).at(shootY) == currPlayer && shootX == counter1 && shootY == counter2)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
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
							if (pBoard->mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
							}
							else if (pBoard->mBoardVec->at(shootX).at(shootY) == currPlayer && shootX == counter1 && shootY == counter2)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
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
						while (shootX < mNumCols && !shitWall)
						{
							if (pBoard->mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
							}
							else if (pBoard->mBoardVec->at(shootX).at(shootY) == currPlayer && shootX == counter1 && shootY == counter2)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
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
							if (pBoard->mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
							}
							else if (pBoard->mBoardVec->at(shootX).at(shootY) == currPlayer && shootX == counter1 && shootY == counter2)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
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
						while (shootX >= 0 && shootY < mNumRows && !shitWall)
						{
							if (pBoard->mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
							}
							else if (pBoard->mBoardVec->at(shootX).at(shootY) == currPlayer && shootX == counter1 && shootY == counter2)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
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
						while (shootX < mNumCols && shootY < mNumRows && !shitWall)
						{
							if (pBoard->mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
							}
							else if (pBoard->mBoardVec->at(shootX).at(shootY) == currPlayer && shootX == counter1 && shootY == counter2)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
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
						while (shootX < mNumCols && shootY >= 0 && !shitWall)
						{
							if (pBoard->mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
							}
							else if (pBoard->mBoardVec->at(shootX).at(shootY) == currPlayer && shootX == counter1 && shootY == counter2)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
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
				while (newX >= 0 && newY >= 0 && !hitWall)
				{
					if (pBoard->mBoardVec->at(newX).at(newY) == BOARD_CLEAR)
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
							if (pBoard->mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
							}
							else if (pBoard->mBoardVec->at(shootX).at(shootY) == currPlayer && shootX == counter1 && shootY == counter2)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
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
						while (shootY < mNumRows && !shitWall)
						{
							if (pBoard->mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
							}
							else if (pBoard->mBoardVec->at(shootX).at(shootY) == currPlayer && shootX == counter1 && shootY == counter2)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
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
							if (pBoard->mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
							}
							else if (pBoard->mBoardVec->at(shootX).at(shootY) == currPlayer && shootX == counter1 && shootY == counter2)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
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
						while (shootX < mNumCols && !shitWall)
						{
							if (pBoard->mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
							}
							else if (pBoard->mBoardVec->at(shootX).at(shootY) == currPlayer && shootX == counter1 && shootY == counter2)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
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
							if (pBoard->mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
							}
							else if (pBoard->mBoardVec->at(shootX).at(shootY) == currPlayer && shootX == counter1 && shootY == counter2)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
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
						while (shootX >= 0 && shootY < mNumRows && !shitWall)
						{
							if (pBoard->mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
							}
							else if (pBoard->mBoardVec->at(shootX).at(shootY) == currPlayer && shootX == counter1 && shootY == counter2)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
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
						while (shootX < mNumCols && shootY < mNumRows && !shitWall)
						{
							if (pBoard->mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
							}
							else if (pBoard->mBoardVec->at(shootX).at(shootY) == currPlayer && shootX == counter1 && shootY == counter2)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
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
						while (shootX < mNumCols && shootY >= 0 && !shitWall)
						{
							if (pBoard->mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
							}
							else if (pBoard->mBoardVec->at(shootX).at(shootY) == currPlayer && shootX == counter1 && shootY == counter2)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
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
				while (newX < mNumCols && newY < mNumRows && !hitWall)
				{
					if (pBoard->mBoardVec->at(newX).at(newY) == BOARD_CLEAR)
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
							if (pBoard->mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
							}
							else if (pBoard->mBoardVec->at(shootX).at(shootY) == currPlayer && shootX == counter1 && shootY == counter2)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
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
						while (shootY < mNumRows && !shitWall)
						{
							if (pBoard->mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
							}
							else if (pBoard->mBoardVec->at(shootX).at(shootY) == currPlayer && shootX == counter1 && shootY == counter2)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
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
							if (pBoard->mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
							}
							else if (pBoard->mBoardVec->at(shootX).at(shootY) == currPlayer && shootX == counter1 && shootY == counter2)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
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
						while (shootX < mNumCols && !shitWall)
						{
							if (pBoard->mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
							}
							else if (pBoard->mBoardVec->at(shootX).at(shootY) == currPlayer && shootX == counter1 && shootY == counter2)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
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
							if (pBoard->mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
							}
							else if (pBoard->mBoardVec->at(shootX).at(shootY) == currPlayer && shootX == counter1 && shootY == counter2)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
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
						while (shootX >= 0 && shootY < mNumRows && !shitWall)
						{
							if (pBoard->mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
							}
							else if (pBoard->mBoardVec->at(shootX).at(shootY) == currPlayer && shootX == counter1 && shootY == counter2)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
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
						while (shootX < mNumCols && shootY < mNumRows && !shitWall)
						{
							if (pBoard->mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
							}
							else if (pBoard->mBoardVec->at(shootX).at(shootY) == currPlayer && shootX == counter1 && shootY == counter2)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
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
						while (shootX < mNumCols && shootY >= 0 && !shitWall)
						{
							if (pBoard->mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
							}
							else if (pBoard->mBoardVec->at(shootX).at(shootY) == currPlayer && shootX == counter1 && shootY == counter2)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
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
				while (newX >= 0 && newY < mNumRows && !hitWall)
				{
					if (pBoard->mBoardVec->at(newX).at(newY) == BOARD_CLEAR)
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
							if (pBoard->mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
							}
							else if (pBoard->mBoardVec->at(shootX).at(shootY) == currPlayer && shootX == counter1 && shootY == counter2)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
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
						while (shootY < mNumRows && !shitWall)
						{
							if (pBoard->mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
							}
							else if (pBoard->mBoardVec->at(shootX).at(shootY) == currPlayer && shootX == counter1 && shootY == counter2)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
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
							if (pBoard->mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
							}
							else if (pBoard->mBoardVec->at(shootX).at(shootY) == currPlayer && shootX == counter1 && shootY == counter2)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
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
						while (shootX < mNumCols && !shitWall)
						{
							if (pBoard->mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
							}
							else if (pBoard->mBoardVec->at(shootX).at(shootY) == currPlayer && shootX == counter1 && shootY == counter2)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
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
							if (pBoard->mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
							}
							else if (pBoard->mBoardVec->at(shootX).at(shootY) == currPlayer && shootX == counter1 && shootY == counter2)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
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
						while (shootX >= 0 && shootY < mNumRows && !shitWall)
						{
							if (pBoard->mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
							}
							else if (pBoard->mBoardVec->at(shootX).at(shootY) == currPlayer && shootX == counter1 && shootY == counter2)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
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
						while (shootX < mNumCols && shootY < mNumRows && !shitWall)
						{
							if (pBoard->mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
							}
							else if (pBoard->mBoardVec->at(shootX).at(shootY) == currPlayer && shootX == counter1 && shootY == counter2)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
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
						while (shootX < mNumCols && shootY >= 0 && !shitWall)
						{
							if (pBoard->mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
							}
							else if (pBoard->mBoardVec->at(shootX).at(shootY) == currPlayer && shootX == counter1 && shootY == counter2)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
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
				while (newX < mNumCols && newY >= 0 && !hitWall)
				{
					if (pBoard->mBoardVec->at(newX).at(newY) == BOARD_CLEAR)
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
							if (pBoard->mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
							}
							else if (pBoard->mBoardVec->at(shootX).at(shootY) == currPlayer && shootX == counter1 && shootY == counter2)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
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
						while (shootY < mNumRows && !shitWall)
						{
							if (pBoard->mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
							}
							else if (pBoard->mBoardVec->at(shootX).at(shootY) == currPlayer && shootX == counter1 && shootY == counter2)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
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
							if (pBoard->mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
							}
							else if (pBoard->mBoardVec->at(shootX).at(shootY) == currPlayer && shootX == counter1 && shootY == counter2)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
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
						while (shootX < mNumCols && !shitWall)
						{
							if (pBoard->mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
							}
							else if (pBoard->mBoardVec->at(shootX).at(shootY) == currPlayer && shootX == counter1 && shootY == counter2)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
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
							if (pBoard->mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
							}
							else if (pBoard->mBoardVec->at(shootX).at(shootY) == currPlayer && shootX == counter1 && shootY == counter2)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
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
						while (shootX >= 0 && shootY < mNumRows && !shitWall)
						{
							if (pBoard->mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
							}
							else if (pBoard->mBoardVec->at(shootX).at(shootY) == currPlayer && shootX == counter1 && shootY == counter2)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
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
						while (shootX < mNumCols && shootY < mNumRows && !shitWall)
						{
							if (pBoard->mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
							}
							else if (pBoard->mBoardVec->at(shootX).at(shootY) == currPlayer && shootX == counter1 && shootY == counter2)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
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
						while (shootX < mNumCols && shootY >= 0 && !shitWall)
						{
							if (pBoard->mBoardVec->at(shootX).at(shootY) == BOARD_CLEAR)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
							}
							else if (pBoard->mBoardVec->at(shootX).at(shootY) == currPlayer && shootX == counter1 && shootY == counter2)
							{
								MoveClass* mt = new MoveClass(counter1, counter2, newX, newY, shootX, shootY, currPlayer);
								Board* tBoard = new Board(pBoard);
								tBoard->UpdateBoard(mt);
								mt->Scope = Search(tBoard, depthTo, currDepth + 1);
								if (!isMoveSet)
								{
									isMoveSet = true;
									temp->mOrigX = mt->mOrigX;
									temp->mOrigY = mt->mOrigY;
									temp->mNewX = mt->mNewX;
									temp->mNewY = mt->mNewY;
									temp->mShootX = mt->mShootX;
									temp->mShootY = mt->mShootY;
									temp->Scope = mt->Scope;
								}
								else
								{
									if (currPlayer == BOARD_BLACK)
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
									else
									{
										if (temp->Scope < mt->Scope)
										{
											temp->mOrigX = mt->mOrigX;
											temp->mOrigY = mt->mOrigY;
											temp->mNewX = mt->mNewX;
											temp->mNewY = mt->mNewY;
											temp->mShootX = mt->mShootX;
											temp->mShootY = mt->mShootY;
											temp->Scope = mt->Scope;
										}
									}
								}
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


	return temp;
}