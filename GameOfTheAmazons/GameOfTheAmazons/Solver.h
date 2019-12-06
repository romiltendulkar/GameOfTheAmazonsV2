
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
	Solver(FILE* pInFile, FILE* pOutFile);
	~Solver();

private:
	FILE* mInFile;
	FILE* mOutFile;
};

