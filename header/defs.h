/*
defs.h
-------------

Created for: Spring 2014 Direct3D 11 Learning
By: Bernard Llanos
May 18, 2014

Primary basis: No reference
Other references: None

Development environment: Visual Studio 2013 running on Windows 7, 64-bit
  -Note that the "Character Set" project property (Configuration Properties > General)
   should be set to Unicode for all configurations, when using Visual Studio.

Description
  A file containing global definitions and preprocessor constants
*/

#pragma once

// For finding memory leaks in the debug build configuration
// ---------------------------------------------------------
#ifndef _CRTDBG_MAP_ALLOC
#define _CRTDBG_MAP_ALLOC
// See http://msdn.microsoft.com/en-us/library/x98tx3cf.aspx
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
/* This allows the memory leak check to report more information
(e.g. file and line number) when it detects memory leaks resulting
from the use of the 'new' operator.
*/
#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif  // _DEBUG
#endif

// Macro for stringifying arguments
// (See http://msdn.microsoft.com/en-us/library/7e3a913x.aspx)
#define LCHAR_STRINGIFY( x ) L#x

// Default configuration values
// ----------------------------
// (To be overridden with values loaded from a configuration file)

#define DEFAULT_LOG_PATH			LCHAR_STRINGIFY(.\\logger_output\\)

// For the results of unit tests
#define DEFAULT_LOG_PATH_TEST		LCHAR_STRINGIFY(.\\logger_output\\test\\)

#define DEFAULT_LOG_FILENAME		LCHAR_STRINGIFY(log.txt)

#define DEFAULT_CONFIG_PATH			LCHAR_STRINGIFY(.\\configuration\\)

// For use with unit tests
#define DEFAULT_CONFIG_PATH_TEST	LCHAR_STRINGIFY(.\\configuration\\test\\)
#define DEFAULT_CONFIG_PATH_TEST_WRITE	LCHAR_STRINGIFY(.\\configuration\\test\\output\\)

// Custom error code definitions
// -----------------------------

// Used for making custom HRESULT values
#define FACILITY_BL_ENGINE 0

/* These error codes will take precedence over system-defined error codes
in case of conflict. (In other words, a system-defined error code
interpretation will be ignored if the error code number also matches
one of the interpretations below.)

On the other hand, some system-defined error codes are used in this program
because their names or descriptions are reasonably appropriate for the
contexts of certain errors. Therefore, the following is not a complete
list of all error codes used in the program.
*/
#define ERROR_FUNCTION_CALL			1800 // Error code returned by a function call
#define ERROR_WRONG_STATE			1801 // A member function was called for an object in an inappropriate state
#define ERROR_DATA_NOT_FOUND		1802 // Data structure failed to find an element
#define ERROR_NO_LOGGER				1803 // Failed to create a custom Logger object
#define ERROR_WINDOWS_CALL			1804 // Error in a call to a Windows function
#define ERROR_LIBRARY_CALL			1805 // Error in a call to a library function
#define ERROR_DATA_INCOMPLETE		1806 // Data entered or returned is not complete or is partially corrupted
#define ERROR_NULL_INPUT			1807 // A null pointer was passed to a function
#define ERROR_INVALID_INPUT			1808 // Bad input was passed to a function (excluding null pointers)
#define ERROR_BROKEN_CODE			1809 // The program logic is faulty and should be fixed by a developer

/* Error codes defined in Windows headers used in this program
   (Their meanings as normally used in this program are provided below.)
 */
// ---------------------------------------------------------------------
// ERROR_INVALID_DATA			// The data within an object is invalid
// ERROR_ALREADY_ASSIGNED		// A data structure already contains this data item


// Constants
// ---------

#define MIN_RGBA_UINT 0		// Minimum value of an integer RGBA color component
#define MAX_RGBA_UINT 255	// Maximum value of an integer RGBA color component