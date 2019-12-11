//////////////////////////////////////////////////////////////
/**
@file Application.cpp
@author Romil Tendulkar
@date 12/02/2019
@brief This file is basically the main for the project and 
the project will run through this
*/
//////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////
// C++ includes
//////////////////////////////////////////////////////////////
#include<iostream>

//////////////////////////////////////////////////////////////
// Project includes
//////////////////////////////////////////////////////////////
#include "Solver.h"

int main()
{
	char filename[256];
	FILE* inFile, * outFile;

	do
	{
		//Here we will initialize the file stream for the file containing our board
		std::cout << "\nEnter the name of the file containing the board: ";
		std::cin.getline(filename, 256);
		fopen_s(&inFile, filename, "r");
		if (inFile == NULL)
		{
			std::cout << "\nPlease enter a valid filename";
		}
	} while (inFile == nullptr);

	fopen_s(&outFile, "OutputFile.txt", "w+");
	if (outFile == NULL)
	{
		//Error handling for output file
		std::cout << "\nFailed to create an output file ";
	}

	Solver* mySolver = new Solver(inFile, outFile);
	mySolver->MakeBoard();

	//close both  the files
	fclose(inFile);
	fclose(outFile);
}