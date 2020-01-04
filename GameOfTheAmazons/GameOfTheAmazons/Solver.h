
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
	/**
		Constructor for the class opens the file stream.

		\param pInFile | The pointer to the file stream of the input file.
		\param pOutFile | The pointer to the file stream of the output file.
	*/
	Solver(FILE* pInFile, FILE* pOutFile);
	~Solver();

	//////////////////////////////////////////////////////////////
	// Functions
	//////////////////////////////////////////////////////////////

	/**
		Reads the input stream and writes to the board.
	*/
	void MakeBoard();

	/**
		Calls the search function to find solution.
		Also has the interactive elements for player movement.
	*/
	void Solve();

	/**
		returns the scope of the current board position.
		if not in a terminal state then the best move is found.

		\param pBoard | the pointer to the data for the board position.
		\param depthTo | the depth to which the bruteforce is to be performed.
		\param currDepth | the current depth for the recursion. 

		\return | The scope of the board position is returned.
	*/
	int Search(Board *pBoard,int depthTo, int currDepth);

	/**
		Used to store the human players best possible move.

		\param pBoard | the pointer to the data for the board position.
		\param depthTo | the depth to which the bruteforce is to be performed.
		\param currDepth | the current depth for the recursion.

		\return | The scope of the board position is returned.
	*/
	int BSearch(Board* pBoard, int depthTo, int currDepth);

	/**
		This function finds the best move for the player that is going to play.

		\param pBoard | the pointer to the current board position.
		\param currPlayer | the enum for the player whose move this is.
		\param depthTo | the depth to which recursion is to be performed.
		\param currDepth | The current depth of the recursion.

		return | The pointer to the Best move found.
	*/
	MoveClass* GetBestMove(Board *pBoard, BoardIDs currPlayer, int depthTo, int currDepth);

public:
	bool isSatisfied;				//used for player interaction.
	
	Board* mCurrentBoard;			//Current Board.
	
	MoveClass* mBestAI;				//Best Move AI can make on the current Board.
	MoveClass* mBestPlayer;			//Best Move Player can make in response to the AI.
private:
	FILE* mInFile;					//Pointer to the file containing the starting board.
	FILE* mOutFile;					//Pointer to the output file incase we wanna print our moves.
};

