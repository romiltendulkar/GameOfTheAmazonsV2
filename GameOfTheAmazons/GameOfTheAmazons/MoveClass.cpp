//////////////////////////////////////////////////////////////
/**
@file MoveClass.cpp
@author Romil Tendulkar
@date 12/10/2019
@brief This class will be used to send moves inside the project.
The move will hold the board and the movement that will take place.
*/
//////////////////////////////////////////////////////////////

#include "MoveClass.h"

MoveClass::MoveClass(int OrigX, int OrigY, int NewX, int NewY, int ShootX, int ShootY, BoardIDs player)
{
	mOrigX = OrigX;
	mOrigY = OrigY;

	mNewX = NewX;
	mNewY = NewY;

	mShootX = ShootX;
	mShootY = ShootY;

	mPlayerId = player;
}

MoveClass::~MoveClass()
{

}
