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

#include "Solver.h"
Solver::Solver(FILE* pInFile, FILE* pOutFile)
{
	mInFile = pInFile;
	mOutFile = pOutFile;
}

Solver::~Solver()
{

}