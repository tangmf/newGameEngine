// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Module:			Gameplay Programming
// Assignment1:		Cheats:Enabled
// Student Name:	Eugene Long - [Engine Designer]
// Student No.:		S10193060J
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// ===========================================================================
// InkdotEngine - Error Handling Header
// ===========================================================================
// * This is a InkdotEngine component, derived from the standard GPP engine
//   templates provided by Ms. Lee Ching Yun, modified by Eugene Long.
// 
// ! This class uses STANDARD STRINGS.
// ===========================================================================

#ifndef _INKDOT_ERROR_H
#define _INKDOT_ERROR_H

// import minimal windows headers
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// import necessary headers
#include <string>
#include <exception>


// related constructs

namespace InkdotErrorType
{
	const int FATAL_ERROR	= -1;
	const int WARNING		=  1;
}


// class specification

class InkdotError : public std::exception
{
private:

	// members

	// code of error being thrown
	int errorCode;

	// message describing error being thrown
	std::string errorMsg;

public:

	// default constructor
	InkdotError():
		errorCode	(InkdotErrorType::FATAL_ERROR),
		errorMsg	("An undefined fatal engine error occurred.")
	{}

	// copy constructor
	InkdotError(
		const InkdotError& e
	):
		std::exception	(e),
		errorCode		(e.errorCode),
		errorMsg		(e.errorMsg)
	{}

	// constructor with args
	InkdotError(
		int				errCode,
		std::string	msg
	):
		errorCode	(errCode),
		errorMsg	(msg)
	{}

	// assignment (=) operator
	InkdotError& operator= (
		const InkdotError& rhs
	) {
		// set this object's exception to rhs exception
		std::exception::operator=(rhs);

		// set this object's members to rhs members
		this->errorCode = rhs.errorCode;
		this->errorMsg	= rhs.errorMsg;
	}

	// destructor
	~InkdotError() {};


	// getters
	
	// return pointer to error message
	const char* getMessage() const { return errorMsg.c_str(); }

	// return error code of error
	int getErrorCode() const { return errorCode; }


	// overrides

	// override original exception class what() method
	virtual const char* what() const { return getMessage(); }

};

#endif // !_INKDOT_ERROR_H