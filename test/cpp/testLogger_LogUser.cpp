/*
testLogger_LogUser.cpp
-----------------------------

Created for: Spring 2014 Direct3D 11 Learning
By: Bernard Llanos
June 8, 2014

Primary basis: None

Other references: None

Development environment: Visual Studio 2013 running on Windows 7, 64-bit
  -Note that the "Character Set" project property (Configuration Properties > General)
   should be set to Unicode for all configurations, when using Visual Studio.

Description
  -Implementations of test functions for the Logger and LogUser classes
*/

#include <string>
#include <iterator>
#include <list>
#include "testLogger_LogUser.h"
#include "defs.h"
#include "Logger.h"
#include "LogUser.h"

using std::wstring;

HRESULT testLogger_LogUser::testBulkLogging(void) {

	// Create a file for logging the test results
	Logger* logger = 0;
	try {
		std::wstring logFilename = DEFAULT_LOG_PATH_TEST;
		logFilename += L"testBulkLogging.txt";
		logger = new Logger(true, logFilename, true, true);
	} catch( ... ) {
		return MAKE_HRESULT(SEVERITY_ERROR, FACILITY_BL_ENGINE, ERROR_NO_LOGGER);
	}

	HRESULT result = ERROR_SUCCESS;
	HRESULT finalResult = ERROR_SUCCESS;

	// Begin tests
	std::list<wstring> list;
	result = logger->logMessage(list.cbegin(), list.cend(), L"Empty list:");
	if( FAILED(result) ) {
		finalResult = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_BL_ENGINE, ERROR_FUNCTION_CALL);
		logger->logMessage(L"Failed to log empty list.");
	}

	list.push_back(L"First message.");
	result = logger->logMessage(list.cbegin(), list.cend(), L"One-element list:");
	if( FAILED(result) ) {
		finalResult = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_BL_ENGINE, ERROR_FUNCTION_CALL);
		logger->logMessage(L"Failed to log one-element list.");
	}

	list.push_back(L"Second message.");
	list.push_back(L"Third message.");
	result = logger->logMessage(list.cbegin(), list.cend(), L"Three-element list:");
	if( FAILED(result) ) {
		finalResult = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_BL_ENGINE, ERROR_FUNCTION_CALL);
		logger->logMessage(L"Failed to log three-element list.");
	}

	// Try logging to another file
	result = logger->logMessage(list.cbegin(), list.cend(), L"Three-element list, to another file:",
		true, true, DEFAULT_LOG_PATH_TEST + wstring(L"testBulkLogging2.txt"));
	if( FAILED(result) ) {
		finalResult = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_BL_ENGINE, ERROR_FUNCTION_CALL);
		logger->logMessage(L"Failed to log three-element list to an alternate file.");
	}

	// Try logging to an invalid file
	result = logger->logMessage(list.cbegin(), list.cend(), L"Three-element list, to another file:",
		true, true, DEFAULT_LOG_PATH_TEST + wstring(L":.txt"));
	if( SUCCEEDED(result) ) {
		finalResult = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_BL_ENGINE, ERROR_FUNCTION_CALL);
		logger->logMessage(L"Failed to fail to log three-element list to an invalid alternate file.");
	}

	if( SUCCEEDED(finalResult) ) {
		logger->logMessage(L"All tests passed.");
	} else {
		logger->logMessage(L"Some or all tests failed.");
	}

	delete logger;

	return finalResult;
}

HRESULT testLogger_LogUser::testLocking(void) {

	// Create a file for logging the test results
	std::wstring logFilename = DEFAULT_LOG_PATH_TEST;
	logFilename += L"testLocking.txt";
	Logger* logger1 = 0;
	try {
		logger1 = new Logger(true, logFilename, true, true);
	} catch( ... ) {
		return MAKE_HRESULT(SEVERITY_ERROR, FACILITY_BL_ENGINE, ERROR_NO_LOGGER);
	}

	// See if a second logger can open the same file
	Logger* logger2 = 0;
	try {
		logger2 = new Logger(true, logFilename, true, true);
		logger2->logMessage(L"Second logger created successfully.");
	} catch( std::exception e ) {
		std::wstring exceptionMsg;
		if( FAILED(toWString(exceptionMsg, e.what())) ) {
			exceptionMsg = L"Error retrieving exception message.";
		}
		logger1->logMessage(L"Second logger could not be created: " + exceptionMsg);
	}

	delete logger1;
	delete logger2;

	return ERROR_SUCCESS;
}