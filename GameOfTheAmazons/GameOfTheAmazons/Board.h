//////////////////////////////////////////////////////////////
/**
@file Board.h
@author Romil Tendulkar
@date 12/02/2019
@brief This class will be used to store the board and the positions on it.
*/
//////////////////////////////////////////////////////////////

#pragma once

#include <vector>

class MoveClass;
//////////////////////////////////////////////////////////////
// Enum
//////////////////////////////////////////////////////////////
enum BoardIDs
{
	BOARD_CLEAR,	// Nothing on the board at this spot
	BOARD_WALL,		// A wall exists on this spot (These are also created when the player shoots)
	BOARD_WHITE,	// A piece of the White Player exists on this spot
	BOARD_BLACK		// A piece of the Black Player exists on this spot
};

class Board
{
	//////////////////////////////////////////////////////////////
	// Functions
	//////////////////////////////////////////////////////////////
public:
	/**
		Default Constructor for the board. This will create and
		initialize to Clear a 10 x 10 board
	*/
	Board();

	/**	
		This will Create and initialize a board of M x N size.
		This should be a positive number

		\param Rows | The number of Rows on the board
	*/
	Board(unsigned int Rows,unsigned int Cols);

	/**
		This will create a new copy of the board from the board passed.
		\param pBoard | The pointer to the board that is to be copied.
	*/
	Board(Board* pBoard);

	/**
		Destructor for the Board class this will clear and then delete the board.
	*/
	~Board();



	/**
		This will update the board based on the move made.
		The move will have been validated by this point.

		\param pMove | The pointer to the move that is made on the board.
	*/
	void UpdateBoard(MoveClass* pMove);
	
	/**
		This function will print the current board position.
	*/
	void PrintBoard();

	/**	
		Find the scope for a player on the current board.

		\param player | enumerator for the player whose scope is to be found.

		\return | the scope of the player is returned as an integer.
	*/
	int FindScope(BoardIDs player);

	bool ValidMove(int oX, int oY, int nX, int nY, int sX, int sY);

	//////////////////////////////////////////////////////////////
	// Variables
	//////////////////////////////////////////////////////////////
public:
	unsigned int mNumRows, mNumCols;			// Number of Rows and Columns on the Board
	std::vector<std::vector<int>>* mBoardVec;	// The vector that holds the data for the Board 
};