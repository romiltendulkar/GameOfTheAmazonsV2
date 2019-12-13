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
	while (!isSatisfied)
	{
		mCurrentBoard->PrintBoard();
		Search(mCurrentBoard, 0, 0);
		if (mBestAI != nullptr)
		{
			std::cout << "\n Best Move for AI has Scope: " << mBestAI->Scope;
			std::cout << "\n The best move for player is from (" << mBestAI->mOrigX << "," << mBestAI->mOrigY
				<< ") to (" << mBestAI->mNewX << " ," << mBestAI->mNewY
				<< ") shooting at " << " (" << mBestAI->mShootX << " ," << mBestAI->mShootY << ")";
		}
		if (mBestPlayer != nullptr)
		{
			std::cout << "\n Best Move for Player has Scope: " << mBestPlayer->Scope;
			std::cout << "\n The best move for player is from (" << mBestPlayer->mOrigX << "," << mBestPlayer->mOrigY
				<< ") to (" << mBestPlayer->mNewX << " ," << mBestPlayer->mNewY
				<< ") shooting at " << " (" << mBestPlayer->mShootX << " ," << mBestPlayer->mShootY << ")";
		}
		char input;
		std::cout << "\n Do you want to make a move? Press Y to confirm: ";
		std::cin >> input;
		if (input == 'y' || input == 'Y')
		{
			char insideInput;
			std::cout << "\n Do you want to make best move for player? Press Y to confirm: ";
			std::cin >> insideInput;
			if (insideInput == 'y' || insideInput == 'Y')
			{
				mCurrentBoard->UpdateBoard(mBestAI);
				mCurrentBoard->UpdateBoard(mBestPlayer);
			}
			else
			{

				mCurrentBoard->UpdateBoard(mBestAI);
				mCurrentBoard->PrintBoard();
				bool moveable = false;
				int oX, oY, nX, nY, sX, sY;

				while (!moveable)
				{
					std::cout << "\Enter x position of piece to move:";
					std::cin >> oX;
					std::cout << "\Enter y position of piece to move:";
					std::cin >> oY;
					if (mCurrentBoard->mBoardVec->at(oX).at(oY) == BOARD_WHITE)
					{
						moveable = true;
					}
					else
					{
						std::cout << "\n Enter valid position!";
					}
				}
				moveable = false;
				while (!moveable)
				{
					std::cout << "\Enter new x position :";
					std::cin >> nX;
					std::cout << "\Enter new y position :";
					std::cin >> nY;
					bool hitwall = false;
					int cx = oX, cy = oY;
					if (nX == oX && nY == oY)
					{
						std::cout << "\n Enter valid position!";
					}
					else
					{
						while (!hitwall)
						{
							if (nX >= oX)
							{
								if (cx < nX)
								{
									cx++;
								}
							}
							else
							{

								if (cx > nX)
								{
									cx--;
								}
							}
							if (nY >= oY)
							{
								if (cy< nY)
								{
									cy++;
								}
							}
							else
							{

								if (cy > nY)
								{
									cy--;
								}
							}
							if (mCurrentBoard->mBoardVec->at(cx).at(cy) == BOARD_WALL)
							{
								hitwall == true;
							}
							else
							{
								if (cx == nX && cy == nY)
								{
									if (mCurrentBoard->mBoardVec->at(cx).at(cy) == BOARD_CLEAR)
									{
										moveable = true;
										hitwall = true;
									}
								}
							}
						}
						if (!moveable)
						{
							std::cout << "\n Enter valid position!";
						}
					}
					
				}
				
				moveable = false;
				while (!moveable)
				{
					std::cout << "\Enter x position to shoot:";
					std::cin >> sX;
					std::cout << "\Enter y position to shoot:";
					std::cin >> sY;
					bool hitwall = false;
					int cx = nX, cy = nY;
					if (sX == nX && sY == nY)
					{
						std::cout << "\n Enter valid position!";
					}
					else if (sX == oX && sY == oY)
					{
						moveable = true;
					}
					else
					{
						while (!hitwall)
						{
							if (sX >= nX)
							{
								if (cx < sX)
								{
									cx++;
								}
							}
							else
							{

								if (cx > sX)
								{
									cx--;
								}
							}
							if (sY >= sY)
							{
								if (cy < sY)
								{
									cy++;
								}
							}
							else
							{

								if (cy > sY)
								{
									cy--;
								}
							}
							if (mCurrentBoard->mBoardVec->at(cx).at(cy) == BOARD_WALL)
							{
								hitwall == true;
							}
							else
							{
								if (cx == sX && cy == sY)
								{
									if (mCurrentBoard->mBoardVec->at(cx).at(cy) == BOARD_CLEAR)
									{
										moveable = true;
										hitwall = true;
									}
								}
							}
						}
						if (!moveable)
						{
							std::cout << "\n Enter valid position!";
						}
					}

				}

				moveable = false;
				MoveClass* mMove = new MoveClass(oX, oY, nX, nY, sX, sY,BOARD_WHITE);
				mCurrentBoard->UpdateBoard(mMove);

			}
		}
		else
		{
			isSatisfied = true;
		}
	}
}

int Solver::Search(Board *pBoard, int depthTo, int currDepth)
{
	bool isMax = false;
	

	if ((currDepth % 2) == 0)
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
		mMove = new MoveClass(GetBestMove(pBoard, BOARD_BLACK, depthTo, currDepth));
		//GetBestMove(pBoard, BOARD_BLACK);
		if (currDepth == 0)
		{
			mBestAI = new MoveClass(mMove);
			Board* temp = new Board(pBoard);
			temp->UpdateBoard(mMove);
			BSearch(temp, depthTo, 1);
		}
		//std::cout << "\n Scope is: " << mMove->Scope;
		return mMove->Scope;
		
	}
	else
	{
		mMove = new MoveClass(GetBestMove(pBoard, BOARD_WHITE, depthTo, currDepth));
		return mMove->Scope;
	}
	//return 0;
}

int Solver::BSearch(Board* pBoard, int depthTo, int currDepth)
{
	bool isMax = false;


	if ((currDepth % 2) == 0)
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
		}
		if (depthTo != 0)
		{
			if (depthTo == currDepth)
			{
				return currScope;
			}
		}
	}
	Board* tBoard = new Board(pBoard);
	MoveClass* mMove;
	if (isMax)
	{
		mMove = new MoveClass(GetBestMove(pBoard, BOARD_BLACK, depthTo, currDepth));

		return mMove->Scope;

	}
	else
	{
		mMove = new MoveClass(GetBestMove(pBoard, BOARD_WHITE, depthTo, currDepth));
		if (currDepth == 1)
		{
			mBestPlayer = new MoveClass(mMove);
		}
		return mMove->Scope;
	}
	//return 0;
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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
										if (temp->Scope > mt->Scope)
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