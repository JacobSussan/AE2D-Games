#include "AEErrors.h"

#include <cstdlib>
#include <iostream>
#include <SDL/SDL.h>
#include <fstream>
#include <time.h>
#include <stdio.h>

// Saves an error message to a file, prints it, then exits the game.
//-------------------------------------------------------------------
void AE::fatalError(std::string errorString)
{
	std::ofstream out;
	out.open("ErrorLog.txt", std::fstream::app);

	time_t curtime;
	time(&curtime);

	// output time and error to file
	out << ctime(&curtime);
	out << errorString + "\n\n";

	// output time and error to console
	std::cout << ctime(&curtime) << std::endl;
	std::cout << errorString << "\n\n\n";
	
	out.close();
	SDL_Quit();
	exit(1);
}
