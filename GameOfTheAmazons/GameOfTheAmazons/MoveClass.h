//////////////////////////////////////////////////////////////
/**
@file MoveClass.h
@author Romil Tendulkar
@date 12/02/2019
@brief This class will be used to send moves inside the project.
The move will hold the board and the movement that will take place.
*/
//////////////////////////////////////////////////////////////

#pragma once
#include "Board.h"

class MoveClass
{
	//////////////////////////////////////////////////////////////
	// Functions
	//////////////////////////////////////////////////////////////
public:
	MoveClass(int OrigX, int OrigY, int NewX, int NewY, int ShootX, int ShootY, BoardIDs player);
	~MoveClass();

	MoveClass(MoveClass* pMove);

	//////////////////////////////////////////////////////////////
	// Variables
	//////////////////////////////////////////////////////////////

public:
	
	int mOrigX, mOrigY;			//The points on the board that the moving piece was before the move
	int mNewX, mNewY;			//The points on the board that the moving piece is after the move
	int mShootX, mShootY;		//The points on the board that the moving piece will shoot at
	BoardIDs mPlayerId;			//The ID of the player making the move

	int Scope = 0;
};