/*
testTextProcessing.cpp
-----------------------------

Created for: Spring 2014 Direct3D 11 Learning
By: Bernard Llanos
June 22, 2014

Primary basis: None

Other references: None

Development environment: Visual Studio 2013 running on Windows 7, 64-bit
  -Note that the "Character Set" project property (Configuration Properties > General)
   should be set to Unicode for all configurations, when using Visual Studio.

Description
  -Implementations of test functions for the textProcessing namespace
*/

#include <string.h>
#include "testTextProcessing.h"
#include "textProcessing.h"
#include "defs.h"
#include "Logger.h"
#include "globals.h"

using std::wstring;

HRESULT testTextProcessing::testControlStrip(void) {

	// Create a file for logging the test results
	Logger* logger = 0;
	try {
		std::wstring logFilename = DEFAULT_LOG_PATH_TEST;
		logFilename += L"testControlStrip.txt";
		logger = new Logger(true, logFilename, true, false);
	} catch( ... ) {
		return MAKE_HRESULT(SEVERITY_ERROR, FACILITY_BL_ENGINE, ERROR_NO_LOGGER);
	}

	HRESULT result = ERROR_SUCCESS;
	HRESULT finalResult = ERROR_SUCCESS;

	// Create an array of test strings
	const char* const pConstStrings[] = {
		"Hello, \n World!",
		" Hello, \n World! ",
		" Hello, \n World!  D",
		" Hello, \n World!  ",
		"\nHello, \t World.\n"
		"",
		"Unmatched \" Delimiter.",
		"Matched \"D e l i m\t i\n t e r\".",
		"Matched |\"\"| No Character Between.",
		"Matched \"a\" One Character Between.",
		"Matched \"ab\" Two Characters In Between.",
		"Matched \"D e l i \tm\n i t e r\"",
		"Matched D e l i m i t e r At End \"\"",
		"Unmatched Escaped \\\" Delimiter.",
		"Matched Escaped \\\"D e l i \n\tm i t e r s\\\".",
		"Matched Delimiter At End: \\\\\"D e l i m i t e r\"",
		"Matched Escaped D e l i m i t e rs At End \\\"\\\"",
		"\"D e l i \tm\n i t e r\"",
		"\\\"D e l i \tm\n i t e r\"",
		"\"D e l i \tm\n i t e r\\\"",
		"\"D e l i \tm\n i t e r\\\\\""
	};
	size_t nStr = sizeof(pConstStrings) / sizeof(char*);

	// Copy the test strings to a modifiable array
	char** pStrings = new char*[nStr];
	for( size_t i = 0; i < nStr; ++i ) {
		size_t bufferSize = strlen(pConstStrings[i]) + 1;
		pStrings[i] = new char[bufferSize];
		strcpy_s(pStrings[i], bufferSize, pConstStrings[i]);
	}

	// Standard ignore characters
	const char ignore[] = { '\n' };
	size_t nIgnore = sizeof(ignore) / sizeof(char);
	const char specialIgnore[] = { '\t', ' ' };
	size_t nSpecialIgnore = sizeof(specialIgnore) / sizeof(char);
	std::wstring msg;

	for( size_t i = 0; i < nStr; ++i ) {
		textProcessing::remove_ASCII_controlAndWhitespace(pStrings[i], ignore,
			nIgnore, '"', specialIgnore, nSpecialIgnore
			);
		if( FAILED(result) ) {
			logger->logMessage(L"Test failed.");
			finalResult = result;
		} else {
			toWString(msg, pStrings[i]);
			logger->logMessage(msg);
		}
	}

	if( SUCCEEDED(finalResult) ) {
		logger->logMessage(L"All tests ran without errors.");
	} else {
		logger->logMessage(L"Some or all tests encountered errors.");
	}

	for( size_t i = 0; i < nStr; ++i ) {
		delete [] pStrings[i];
		pStrings[i] = 0;
	}

	delete[] pStrings;
	delete logger;

	return finalResult;
}