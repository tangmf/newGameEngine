// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Module:			Gameplay Programming
// Assignment1:		Cheats:Enabled
// Student Name:	Eugene Long - [Engine Designer]
// Student No.:		S10193060J
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// ===========================================================================
// File Utility Class Specification
// ===========================================================================

// include specification
#include "fileutils.h"


// ===========================================================================
// memoized game directory path state
// ===========================================================================
std::string FileUtils::gameDirectory = "";

// ===========================================================================
// get path to game directory path under Documents
// ===========================================================================
std::string FileUtils::getGameDirectoryPath()
{
	// use memoized state where possible,
	if (gameDirectory == "") {

		// else retrieve and store game directory path

		PWSTR pPath;

		// retrieve actual user document directory path - this is needed because
		// user document paths can vary between devices and windows versions.
		// this is the best practice to get an accurate filepath to Documents.
		SHGetKnownFolderPath(
			FOLDERID_Documents,
			KF_FLAG_DEFAULT,
			NULL,
			&pPath
		);

		// save path as string
		std::string dirpath = StringUtilities::UTF8ws2s(std::wstring(pPath));

		// deallocate pPath
		CoTaskMemFree(pPath);

		// calculate and store final game directory filepath
		gameDirectory = dirpath + "\\" + StorageConstants::GAME_DIRECTORY;

		// ensure game directory exists
		CreateDirectoryA(gameDirectory.c_str(), NULL);
	}

	// return game directory path
	return gameDirectory;
}

// ===========================================================================
// read text from file
// ===========================================================================
std::string FileUtils::readFromFile(
	std::string	filename
) {
	// initialize path to file
	std::string filepath = getGameDirectoryPath() + "\\" + filename;

	// initialize file content container
	std::string content = "";

	// attempt to create input file stream for file
	std::ifstream file (filepath);

	// ensure file is open before reading from file
	if (file.is_open()) {

		std::string line;

		// while there are still lines that can be read from file,
		while (std::getline(file, line)) {

			// append line to file contents
			content += (line + "\n");
		}

		// close file when finished
		file.close();
	}
	else throw (
		InkdotError(
			InkdotErrorType::WARNING,
			"Warning: Could not open file " + filepath
		)
	);

	// return file contents
	return content;
}

// ===========================================================================
// write text to file
// ===========================================================================
void FileUtils::writeToFile(
	std::string	filename,
	std::string	text
) {
	// initialize path to file
	std::string filepath = getGameDirectoryPath() + "\\" + filename;

	// attempt to create output file stream for file
	std::ofstream file(filepath);

	// ensure file is open before writing to file
	if (file.is_open()) {

		// write text to file
		file << text;

		// close file when finished
		file.close();
	}
	else throw (
		InkdotError(
			InkdotErrorType::WARNING,
			"Warning: Could not open file " + filepath
		)
	);
}
