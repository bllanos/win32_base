/*
testFileUtil.cpp
-----------------

Created for: Spring 2014 Direct3D 11 Learning
By: Bernard Llanos
August 10, 2014

Primary basis: None

Other references: None

Development environment: Visual Studio 2013 running on Windows 7, 64-bit
  -Note that the "Character Set" project property (Configuration Properties > General)
   should be set to Unicode for all configurations, when using Visual Studio.

Description
  -Implementations of test functions for the fileUtil namespace
*/

#include <string>
#include <sstream>
#include "testFileUtil.h"
#include "fileUtil.h"
#include "defs.h"
#include "Logger.h"

using std::wstring;

HRESULT testFileUtil::testExtractPath(void) {

	// Create a file for logging the test results
	Logger* logger = 0;
	try {
		std::wstring logFilename = DEFAULT_LOG_PATH_TEST;
		logFilename += L"testExtractPath.txt";
		logger = new Logger(true, logFilename, true, false);
	} catch( ... ) {
		return MAKE_HRESULT(SEVERITY_ERROR, FACILITY_BL_ENGINE, ERROR_NO_LOGGER);
	}

	HRESULT result = ERROR_SUCCESS;
	HRESULT finalResult = ERROR_SUCCESS;

	// Create an array of test strings
	const wstring pConstStrings[] = {
		L"",
		L".", // Current Directory
		L"..", // Parent directory
		L".\\logger_output", // Valid directory
		L".\\invalid", // Non-existing directory or file
		L".\\text.txt", // Non-existng file
		L".\\.gitignore", // Existing file
		L".\\logger_output\\log.txt",
		L".\\logger_output\\test",
		L"C:\\Users",
		L"C:\\Users\\",
		L"C:\\Users\\Public",
		L"C:\\Users\\Public\\Public Documents",
		L"C:\\Users\\Public\\Public Documents\\",
		L"C:\\Users\\Public\\..\\..\\Users\\Public"
	};
	size_t nStr = sizeof(pConstStrings) / sizeof(wstring);

	// Run the tests
	std::wstring path;
	const std::wstring* pStr;
	std::wostringstream WOSStream;

	for( size_t i = 0; i < nStr; ++i ) {
		pStr = pConstStrings + i;
		result = fileUtil::extractPath(path, *pStr);
		if( FAILED(result) ) {
			logger->logMessage(L"Test failed.");
			finalResult = result;
		} else {
			WOSStream.str(L""); // Clear the string stream
			WOSStream << L"On \"" << *pStr << L"\", fileUtil::extractPath() produces: \"" << path << L"\"";
			logger->logMessage(WOSStream.str());
		}
	}

	if( SUCCEEDED(finalResult) ) {
		logger->logMessage(L"All tests ran without errors.");
	} else {
		logger->logMessage(L"Some or all tests encountered errors.");
	}

	delete logger;

	return finalResult;
}