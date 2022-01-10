// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Module:			Gameplay Programming
// Assignment1:		Cheats:Enabled
// Student Name:	Eugene Long - [Engine Designer]
// Student No.:		S10193060J
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// ===========================================================================
// File Utility Class Specification
// ===========================================================================

#ifndef _FILEUTILS_H
#define _FILEUTILS_H

// import minimal windows headers
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// import necessary headers
#include "../error.h"
#include "../common.h"
#include "strutils.h"
#include <iostream>
#include <fstream>
#include <string>
#include <ShlObj.h>


// class specification

class FileUtils
{
private:

	// memoized game directory path state
	static std::string gameDirectory;


	// get path to game directory path under Documents
	static std::string getGameDirectoryPath();

public:

	// read text from file
	static std::string readFromFile(std::string filename);

	// write text to file
	static void writeToFile(std::string	filename, std::string text);

};


#endif // !_FILEUTILS_H