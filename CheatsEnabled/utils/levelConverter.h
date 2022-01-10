// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Module:			Gameplay Programming
// Assignment1:		Cheats:Enabled
// Student Name:	Eugene Long - [Engine Designer]
// Student No.:		S10193060J
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// ===========================================================================
// Level Converter Utility Class Specification
// ===========================================================================

#ifndef _LEVELCONVERTER_H
#define _LEVELCONVERTER_H

// import necessary headers
#include <sstream>
#include <string>
#include <vector>
#include "../inkdot/src/entity/entityManager.h"
#include "../inkdot/src/entity/legacyEntity.h"


// class specification

class LevelConverter
{
public:
	// default constructor
	LevelConverter();


	// convert level to string
	static std::string convertLevelToString();

	// convert level to string
	static void convertStringToLevel(EntityManager& entityManager, std::string s);

	static std::vector<std::string> split_string_by(const std::string& str,char delim);

private:
	TextureManager wallTexture;
};

#endif // !_LEVELCONVERTER_H