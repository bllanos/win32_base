/*
FlatAtomicConfigIO.cpp
----------------------

Created for: Spring 2014 Direct3D 11 Learning
By: Bernard Llanos
July 1, 2014

Primary basis: None
Other references: None

Development environment: Visual Studio 2013 running on Windows 7, 64-bit
  -Note that the "Character Set" project property (Configuration Properties > General)
   should be set to Unicode for all configurations, when using Visual Studio.

Description
  -Implementation of the FlatAtomicConfigIO class
*/

#include "FlatAtomicConfigIO.h"
#include "defs.h"
#include <fstream>

const Config::DataType FlatAtomicConfigIO::s_supportedDataTypes[] = {
	Config::DataType::WSTRING,
	Config::DataType::BOOL
};

const size_t FlatAtomicConfigIO::s_nSupportedDataTypes = sizeof(s_supportedDataTypes) / sizeof(Config::DataType);

bool FlatAtomicConfigIO::isSupportedDataType(Config::DataType type) {
	for( size_t i = 0; i < s_nSupportedDataTypes; ++i ) {
		if( type == s_supportedDataTypes[i] ) {
			return true;
		}
	}
	return false;
}

FlatAtomicConfigIO::FlatAtomicConfigIO(void) :
LogUser(true, L"FlatAtomicConfigIO >")
{}

FlatAtomicConfigIO::~FlatAtomicConfigIO(void) {}

HRESULT FlatAtomicConfigIO::read(const std::wstring& filename, Config& config) {

	setMsgPrefix(L"FlatAtomicConfigIO reading " + filename + L" >");

	// Open the configuration file
	std::ifstream file(filename, std::ifstream::in);
	if( !file.is_open() ) {
		logMessage(L"Unable to open the file.");
		return MAKE_HRESULT(SEVERITY_ERROR, FACILITY_BL_ENGINE, ERROR_FILE_NOT_FOUND);
	}

	HRESULT result = ERROR_SUCCESS;
	HRESULT lineResult = ERROR_SUCCESS;

	// Process each line
	char line[FLATATOMICCONFIGIO_MAX_LINE_LENGTH];
	size_t lineNumber = 0;
	bool fail = false;
	do {
		file.getline(line, FLATATOMICCONFIGIO_MAX_LINE_LENGTH, FLATATOMICCONFIGIO_LINE_SEP);
		if( file.fail() ) {
			fail = true;
			logMessage(L"File stream bad bit or fail bit was set - Aborting read operation.");
			result = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_BL_ENGINE, ERROR_FUNCTION_CALL);
			break;
		}
		++lineNumber;
		lineResult = readDataLine(config, line, lineNumber);
		if( FAILED(lineResult) ) {
			// The readDataLine() function should already have logged an error to the message queue
			logMessage(L"readDataLine() returned a failure code - Aborting read operation.");
			result = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_BL_ENGINE, ERROR_FUNCTION_CALL);
			break;
		} else if( HRESULT_CODE(lineResult) == ERROR_DATA_INCOMPLETE) {
			result = lineResult;
		}
	} while( file.good() );

	file.close();

	// Write any parsing problems back to the file
	if( m_msgStore.empty() ) {
		logMessage(L"File parsing complete - No invalid data.");
	} else {
		logMessage(L"File parsing complete - Problems encountered.");
		
		if( !fail ) {
			std::wstring time;
			if( FAILED(Logger::getDateAndTime(time)) ) {
				time.clear();
			}
			m_msgStore.emplace_front(L"<<-- FlatAtomicConfigIO class object parsing report ("+time+L") begins --");
			m_msgStore.emplace_back(L"-- FlatAtomicConfigIO class object parsing report ends -->>");

			/* Get an "empty" Logger for easy output to the file,
			   such that this object has complete control over the
			   Logger's timestamping behaviour.
			*/
			if( FAILED(setLogger(false, L"", false, false)) ) {
				logMessage(L"Error getting a Logger to output parsing report to the file.");
				result = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_BL_ENGINE, ERROR_FUNCTION_CALL);
			} else {
				toggleTimestamp(false); // Lines need to start with the comment symbol, not a timestamp
				setMsgPrefix(FLATATOMICCONFIGIO_COMMENT_PREFIX);
				HRESULT tempResult = logMsgStore(true, false, true, filename);
				setMsgPrefix(L"FlatAtomicConfigIO reading " + filename + L" >");
				revertLogger();

				if( FAILED(tempResult) ) {
					logMessage(L"Problem appending parsing error messages to the file.");
					result = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_BL_ENGINE, ERROR_FUNCTION_CALL);
				} else {
					logMessage(L"Parsing error messages appended to the file without problems.");
				}
			}
		}
	}
	return result;
}

HRESULT FlatAtomicConfigIO::write(const std::wstring& filename, const Config& config, const bool overwrite) {

	setMsgPrefix(L"FlatAtomicConfigIO writing to " + filename + L" >");

	// Initialization
	std::map<Config::Key, Config::Value*>::const_iterator currentPair = config.cbegin();
	std::map<Config::Key, Config::Value*>::const_iterator end = config.cend();
	if( currentPair == end ) {
		logMessage(L"Config object is empty - Nothing to write.");
		return ERROR_SUCCESS;
	}

	// Open the output file
	std::basic_ofstream<wchar_t> file;
	if( overwrite ) {
		file.open(filename, std::ofstream::out);
	} else {
		file.open(filename, std::ofstream::app);
	}
	if( !file.is_open() ) {
		logMessage(L"Unable to open the file.");
		return MAKE_HRESULT(SEVERITY_ERROR, FACILITY_BL_ENGINE, ERROR_FILE_NOT_FOUND);
	}

	// Initialization
	HRESULT result = ERROR_SUCCESS;
	HRESULT lineResult = ERROR_SUCCESS;
	std::wstring line;
	bool notGood = false;

	// Process each key-value pair in the Config object
	while( currentPair != end ) {
		lineResult = writeDataLine(line, currentPair);
		if( FAILED(lineResult) ) {
			/* A severe error.
			The writeDataLine() function should already have logged an error to the message queue
			*/
			logMessage(L"writeDataLine() returned a failure code - Aborting write operation.");
			result = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_BL_ENGINE, ERROR_FUNCTION_CALL);
			break;
		} else if( HRESULT_CODE(lineResult) == ERROR_DATA_INCOMPLETE ) {
			/* A non-severe error.
			In this case, the line writing function has encountered
			unsupported or invalid data, and will have put an error
			message in the queue.
			*/
			result = lineResult;
		} else {
			// Write valid data to the file
			file << line << L'\n';
		}
		++currentPair;
		if( !file.good() ) {
			notGood = true;
			logMessage(L"File stream good() function returned false - Aborting write operation.");
			result = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_BL_ENGINE, ERROR_FUNCTION_CALL);
			break;
		}
	}

	// Write the configuration format to the file for reference
	if( !notGood ) {
		file << FLATATOMICCONFIGIO_DATA_FORMATSPEC;
		if( !file.good() ) {
			notGood = true;
			logMessage(L"File stream good() function returned false when writing formatting user guide.");
			result = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_BL_ENGINE, ERROR_FUNCTION_CALL);
		}
	}

	file.close();

	// Write any parsing problems back to the file
	if( m_msgStore.empty() ) {
		logMessage(L"Config object writing complete - No invalid or unsupported data.");
	} else {
		logMessage(L"Config object writing complete - Problems encountered.");

		if( !notGood ) {
			std::wstring time;
			if( FAILED(Logger::getDateAndTime(time)) ) {
				time.clear();
			}
			m_msgStore.emplace_front(L"<<-- FlatAtomicConfigIO class object Config writing report (" + time + L") begins --");
			m_msgStore.emplace_back(L"-- FlatAtomicConfigIO class object Config writing report ends -->>");

			/* Get an "empty" Logger for easy output to the file,
			such that this object has complete control over the
			Logger's timestamping behaviour.
			*/
			if( FAILED(setLogger(false, L"", false, false)) ) {
				logMessage(L"Error getting a Logger to output Config object writing report to the file.");
				result = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_BL_ENGINE, ERROR_FUNCTION_CALL);
			} else {
				toggleTimestamp(false); // Lines need to start with the comment symbol, not a timestamp
				setMsgPrefix(FLATATOMICCONFIGIO_COMMENT_PREFIX);
				HRESULT tempResult = logMsgStore(true, false, true, filename);
				setMsgPrefix(L"FlatAtomicConfigIO writing " + filename + L" >");
				revertLogger();

				if( FAILED(tempResult) ) {
					logMessage(L"Problem appending Config writing error messages to the file.");
					result = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_BL_ENGINE, ERROR_FUNCTION_CALL);
				} else {
					logMessage(L"Config writing error messages appended to the file without problems.");
				}
			}
		}
	}
	return result;
}

HRESULT FlatAtomicConfigIO::readDataLine(Config& config, char* const str, const size_t& lineNumber) {
	return ERROR_SUCCESS;
}

HRESULT FlatAtomicConfigIO::writeDataLine(std::wstring& str, const std::map<Config::Key, Config::Value*>::const_iterator& data) {
	return ERROR_SUCCESS;
}