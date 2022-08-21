#ifndef _ERROR_CODES_H_
#	define _ERROR_CODES_H_

#	define	ERROR_NO_ERROR				(0)				// No error occurred.
#	define	ERROR_GENERIC_ERROR			((1<<29) + 1)	// An error occurred, but your feeling lazy.
#	define	ERROR_INVALID_OBJECT		((1<<29) + 2)	// Invalid object - i.e. NULL, etc.
#	define	ERROR_INVALID_PARAM			((1<<29) + 3)	// Invalid parameter.
#	define	ERROR_OUT_OF_MEMORY			((1<<29) + 4)	// Insufficent memory.
#	define	ERROR_MISSING_RESOURCE		((1<<29) + 5)	// A file or resource is missing / corrupt.
#	define	ERROR_SECOND_INSTANCE		((1<<29) + 6)	// This is a second instance of the app. 


#	define	EXCEPTION_GENERAL					(0xEFFFFFFF - 0)
#	define	EXCEPTION_FAILED_TO_CREATE_OBJECT	(EXCEPTION_GENERAL - 1)
#	define	EXCEPTION_REFERENCED_NULL_OBJECT	(EXCEPTION_GENERAL - 2)
//#	define	EXCEPTION_	(EXCEPTION_GENERAL - 3)
//#	define	EXCEPTION_FAILED_TO_CREATE_OBJECT	(EXCEPTION_GENERAL - 4)

#endif