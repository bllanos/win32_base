/*
testConfig_IConfigManager.cpp
-----------------------------

Created for: Spring 2014 Direct3D 11 Learning
By: Bernard Llanos
May 30, 2014

Primary basis: None

Other references: None

Development environment: Visual Studio 2013 running on Windows 7, 64-bit
  -Note that the "Character Set" project property (Configuration Properties > General)
   should be set to Unicode for all configurations, when using Visual Studio.

Description
  -Implementations of test functions for the Config class
   and the IConfigManager class hierarchy.
*/

#include <string>
#include <iterator>
#include <map>
#include "testConfig_IConfigManager.h"
#include "defs.h"
#include "Config.h"
#include "Logger.h"

using std::wstring;

HRESULT testConfig_IConfigManager::testConfigWithStringValues(
	const unsigned int n, unsigned int nScopes) {

	if( nScopes == 0 ) {
		nScopes = 1;
	}
	
	// Create a file for logging the test results
	Logger* logger = 0;
	try {
		std::wstring logFilename = DEFAULT_LOG_PATH_TEST;
		logFilename += L"testConfigWithStringValues.txt";
		logger = new Logger(true, logFilename, true, false);
	} catch( ... ) {
		return MAKE_HRESULT(SEVERITY_ERROR, FACILITY_BL_ENGINE, ERROR_NO_LOGGER);
	}

	Config config;
	HRESULT result = ERROR_SUCCESS;
	HRESULT finalResult = ERROR_SUCCESS;

	// Load the Config object
	wstring scope = L"Scope";
	wstring field = L"Field";
	wstring value = L"Value";
	
	// Note that values are inserted in descending order of Key
	if( n > 0 ) {
		for( int i = n - 1; i >= 0; --i ) {
			result = config.insert(scope + std::to_wstring(i % nScopes), field + std::to_wstring(i),
				new wstring(value + std::to_wstring(i)));
			if( FAILED(result) ) {
				finalResult = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_BL_ENGINE, ERROR_FUNCTION_CALL);
				logger->logMessage(L"Failed to insert on iteration " + std::to_wstring(i));
			}
		}
	}

	/* Test that duplicate Keys cannot be used,
	that nulls cannot be inserted,
	and that field strings must not be empty.
	*/
	const wstring* temp;
	for( unsigned int i = 0; i < n; ++i ) {
		temp = new wstring(value + std::to_wstring(i));
		result = config.insert(scope + std::to_wstring(i % nScopes), field + std::to_wstring(i),
			temp);
		if( SUCCEEDED(result) ) {
			finalResult = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_BL_ENGINE, ERROR_FUNCTION_CALL);
			logger->logMessage(L"Success code returned for duplicate insertion on iteration "
				+ std::to_wstring(i));
		} else {
			delete temp;
		}

		temp = 0;
		result = config.insert(L"0" + scope + std::to_wstring(i % nScopes),
			field + std::to_wstring(i), temp);
		if( SUCCEEDED(result) ) {
			finalResult = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_BL_ENGINE, ERROR_FUNCTION_CALL);
			logger->logMessage(L"Success code returned for insertion of null on iteration "
				+ std::to_wstring(i));
		}
		temp = new wstring(value + std::to_wstring(i));
		result = config.insert(L"1" + scope + std::to_wstring(i % nScopes),
			L"", temp);
		if( SUCCEEDED(result) ) {
			finalResult = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_BL_ENGINE, ERROR_FUNCTION_CALL);
			logger->logMessage(L"Success code returned for insertion with an empty field on iteration "
				+ std::to_wstring(i));
		} else {
			delete temp;
		}
	}

	// Read out values from the Config object
	wstring prevKeyString;
	wstring currKeyString;
	const wstring* currValue;

	std::map<Config::Key, Config::Value*>::const_iterator start = config.cbegin();
	std::map<Config::Key, Config::Value*>::const_iterator end = config.cend();

	if( n == 0 && start != end ) {
		logger->logMessage(L"cbegin() and cend() returned different iterators for an empty Config object.");
	}

	logger->logMessage(L"Reading values from the Config object, by scope and field...");

	for( unsigned int i = 0; i < n; ++i ) {
		result = config.retrieve(scope + std::to_wstring(i % nScopes),
			field + std::to_wstring(i), currValue);

		if( FAILED(result) ) {
			finalResult = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_BL_ENGINE, ERROR_FUNCTION_CALL);
			logger->logMessage(L"Failed to retrieve value by key parameters on iteration "
				+ std::to_wstring(i));
		} else {
			currKeyString = scope + std::to_wstring(i % nScopes) + L"::" + field + std::to_wstring(i);
			logger->logMessage(currKeyString + L" = " + *currValue);
		}
	}

	logger->logMessage(L"");
	logger->logMessage(L"Reading values from the Config object, in order...");
	Config::DataType type = Config::DataType::WSTRING;

	if( n != 0 ) {
		currValue = static_cast<const wstring*>(start->second->getValue(type));
		if( currValue == 0 ) {
			finalResult = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_BL_ENGINE, ERROR_FUNCTION_CALL);
			logger->logMessage(L"Retrieved null value through iterator on iteration 0.");
		} else {
			prevKeyString = start->first.getScope() + L"::" + start->first.getField();
			logger->logMessage(prevKeyString + L" = " + *currValue);
		}
		++start;
	}

	for( unsigned int i = 1; i < n; ++i, ++start ) {
		if( start == end ) {
			finalResult = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_BL_ENGINE, ERROR_FUNCTION_CALL);
			logger->logMessage(L"Reached end of Config object's data too early.");
			break;
		}

		currValue = static_cast<const wstring*>(start->second->getValue(type));
		if( currValue == 0 ) {
			finalResult = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_BL_ENGINE, ERROR_FUNCTION_CALL);
			logger->logMessage(L"Retrieved null value through iterator on iteration "
				+ std::to_wstring(i));
		} else {
			currKeyString = start->first.getScope() + L"::" + start->first.getField();
			if( prevKeyString > currKeyString ) {
				finalResult = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_BL_ENGINE, ERROR_FUNCTION_CALL);
				logger->logMessage(L"Keys are out of order on iteration "
					+ std::to_wstring(i));
			}
			logger->logMessage(currKeyString + L" = " + *currValue);
			prevKeyString = currKeyString;
		}
	}

	logger->logMessage(L"");

	if( start != end ) {
		finalResult = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_BL_ENGINE, ERROR_FUNCTION_CALL);
		logger->logMessage(L"More data is being stored in the Config object than expected.");
	}

	// Try to retrieve values that are not present
	logger->logMessage(L"Trying to retrieve values from Config object that are not present...");

	const bool* boolPtr = 0;

	for( unsigned int i = 0; i < n; ++i ) {
		result = config.retrieve(L"2"+scope + std::to_wstring(i % nScopes),
			field + std::to_wstring(i), currValue);

		if( SUCCEEDED(result) || currValue != 0 ) {
			finalResult = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_BL_ENGINE, ERROR_FUNCTION_CALL);
			logger->logMessage(L"Retrieved a value for an invalid scope on iteration "
				+ std::to_wstring(i));
		}

		result = config.retrieve(scope + std::to_wstring(i % nScopes),
			L"2" + field + std::to_wstring(i), currValue);

		if( SUCCEEDED(result) || currValue != 0 ) {
			finalResult = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_BL_ENGINE, ERROR_FUNCTION_CALL);
			logger->logMessage(L"Retrieved a value for an invalid field on iteration "
				+ std::to_wstring(i));
		}

		result = config.retrieve(scope + std::to_wstring(i % nScopes),
			field + std::to_wstring(i), boolPtr);

		if( SUCCEEDED(result) || boolPtr != 0 ) {
			finalResult = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_BL_ENGINE, ERROR_FUNCTION_CALL);
			logger->logMessage(L"Retrieved a value for an invalid data type (bool) on iteration "
				+ std::to_wstring(i));
		}
	}

	if( SUCCEEDED(finalResult) ) {
		logger->logMessage(L"All tests passed.");
	} else {
		logger->logMessage(L"Some or all tests failed.");
	}

	delete logger;

	return finalResult;
}

HRESULT testConfig_IConfigManager::testConfigWithStringAndBoolValues(void) {

	// The test will break at the read-back section if these constants are changed
	const unsigned int n = 6;
	const unsigned int nScopes = 2;

	// Create a file for logging the test results
	Logger* logger = 0;
	try {
		std::wstring logFilename = DEFAULT_LOG_PATH_TEST;
		logFilename += L"testConfigWithStringAndBoolValues.txt";
		logger = new Logger(true, logFilename, true, false);
	} catch( ... ) {
		return MAKE_HRESULT(SEVERITY_ERROR, FACILITY_BL_ENGINE, ERROR_NO_LOGGER);
	}

	Config config;
	HRESULT result = ERROR_SUCCESS;
	HRESULT finalResult = ERROR_SUCCESS;

	// Load the Config object
	wstring scope = L"Scope";
	wstring field = L"Field";
	wstring value = L"Value";

	// Note that values are inserted in descending order of Key
	for( int i = n - 1; i >= 0; --i ) {
		if( i % 2 == 0 ) {
			result = config.insert(scope + std::to_wstring(i % nScopes), field + std::to_wstring(i),
				new wstring(value + std::to_wstring(i)));
		} else {
			result = config.insert(scope + std::to_wstring(i % nScopes), field + std::to_wstring(i),
				new bool(i % 3 == 0));
		}
		if( FAILED(result) ) {
			finalResult = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_BL_ENGINE, ERROR_FUNCTION_CALL);
			logger->logMessage(L"Failed to insert on iteration " + std::to_wstring(i));
		}
	}

	// Read out values from the Config object
	wstring currKeyString;
	const wstring* currStringValue;
	const bool* currBoolValue;

	std::map<Config::Key, Config::Value*>::const_iterator start = config.cbegin();
	std::map<Config::Key, Config::Value*>::const_iterator end = config.cend();

	if( n == 0 && start != end ) {
		logger->logMessage(L"cbegin() and cend() returned different iterators for an empty Config object.");
	}

	logger->logMessage(L"Reading values from the Config object, in order...");

	for( unsigned int i = 0; i < n; ++i, ++start ) {
		if( start == end ) {
			finalResult = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_BL_ENGINE, ERROR_FUNCTION_CALL);
			logger->logMessage(L"Reached end of Config object's data too early.");
			break;
		}

		if( i < n / 2 ) {
			currStringValue = static_cast<const wstring*>(start->second->getValue(Config::DataType::WSTRING));
			if( currStringValue == 0 ) {
				finalResult = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_BL_ENGINE, ERROR_FUNCTION_CALL);
				logger->logMessage(L"Retrieved null value through iterator on iteration "
					+ std::to_wstring(i));
			} else {
				currKeyString = start->first.getScope() + L"::" + start->first.getField();
				logger->logMessage(currKeyString + L" = " + *currStringValue);
			}
		} else {
			currBoolValue = static_cast<const bool*>(start->second->getValue(Config::DataType::BOOL));
			if( currBoolValue == 0 ) {
				finalResult = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_BL_ENGINE, ERROR_FUNCTION_CALL);
				logger->logMessage(L"Retrieved null value through iterator on iteration "
					+ std::to_wstring(i));
			} else {
				currKeyString = start->first.getScope() + L"::" + start->first.getField();
				logger->logMessage(currKeyString + L" = " + std::to_wstring(*currBoolValue));
			}
		}
	}

	logger->logMessage(L"");

	if( start != end ) {
		finalResult = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_BL_ENGINE, ERROR_FUNCTION_CALL);
		logger->logMessage(L"More data is being stored in the Config object than expected.");
	}

	if( SUCCEEDED(finalResult) ) {
		logger->logMessage(L"All tests passed.");
	} else {
		logger->logMessage(L"Some or all tests failed.");
	}

	delete logger;

	return finalResult;

}