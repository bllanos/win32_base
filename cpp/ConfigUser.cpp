/*
ConfigUser.cpp
--------------

Created for: Spring 2014 Direct3D 11 Learning
By: Bernard Llanos
August 31, 2014

Primary basis: None
Other references: None

Development environment: Visual Studio 2013 running on Windows 7, 64-bit
  -Note that the "Character Set" project property (Configuration Properties > General)
   should be set to Unicode for all configurations, when using Visual Studio.

Description
  -Implementation of the ConfigUser class.
*/

#include "ConfigUser.h"
#include "globals.h"
#include "defs.h"
#include "fileUtil.h"

ConfigUser::ConfigUser(const bool enableLogging, const std::wstring& msgPrefix,
	Usage usage) :
	LogUser(enableLogging, msgPrefix),
	m_config(0), m_configUseLoggingEnabled(true), m_usage(usage)
{}

ConfigUser::ConfigUser(const bool enableLogging, const std::wstring& msgPrefix,
	Config* sharedConfig) :
	LogUser(enableLogging, msgPrefix),
	m_config(sharedConfig), m_configUseLoggingEnabled(true), m_usage(Usage::SHARED)
{}

ConfigUser::~ConfigUser(void) {}

HRESULT ConfigUser::setSharedConfig(Config* sharedConfig) {}

HRESULT ConfigUser::getSharedConfig(Config* sharedConfig) const {}

void ConfigUser::enableConfigUseLogging() {}

void ConfigUser::disableConfigUseLogging() {}

bool ConfigUser::hasConfig() const {}

ConfigUser::Usage ConfigUser::getUsage() const {}

HRESULT ConfigUser::deletePrivateConfig() {}

HRESULT ConfigUser::helper_IOPrivateConfig(const bool useOwnConfig,
	const Config* locationSource,
	const std::wstring& filenameScope,
	const std::wstring& filenameField,
	const std::wstring& directoryScope,
	const std::wstring& directoryField,
	bool& quit,
	std::wstring& filenameAndPath,
	const std::wstring& logMsgPrefix
	) {

	quit = true;
	filenameAndPath.clear();

	// Error checking
	if( m_usage != Usage::PRIVATE ) {
		return MAKE_HRESULT(SEVERITY_ERROR, FACILITY_BL_ENGINE, ERROR_WRONG_STATE);
	} else if( useOwnConfig && (locationSource != 0) ) {
		return MAKE_HRESULT(SEVERITY_ERROR, FACILITY_BL_ENGINE, ERROR_INVALID_INPUT);
	} else if( (m_config == 0) && useOwnConfig ) {
		return MAKE_HRESULT(SEVERITY_ERROR, FACILITY_BL_ENGINE, ERROR_WRONG_STATE);
	} else if( filenameField.empty() ) {
		return MAKE_HRESULT(SEVERITY_ERROR, FACILITY_BL_ENGINE, ERROR_INVALID_INPUT);
	} else if( !directoryScope.empty() && directoryField.empty() ) {
		return MAKE_HRESULT(SEVERITY_ERROR, FACILITY_BL_ENGINE, ERROR_INVALID_INPUT);
	}

	// Set the Config instance to query for the filepath
	const Config* config = (useOwnConfig) ? m_config : ((locationSource == 0) ? g_defaultConfig : locationSource);
	if( config == 0 ) {
		CONFIGUSER_LOGMESSAGE(logMsgPrefix + L"Config pointer that would be used to obtain a filepath is null.")
		return MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_BL_ENGINE, ERROR_DATA_NOT_FOUND);
	}

	const std::wstring* value;
	std::wstring locators;

	// Retrieve the filename, or filename and path
	HRESULT error = config->retrieve<Config::DataType::FILENAME, std::wstring>(filenameScope, filenameField, value, &locators);
	if( FAILED(error) ) {
		std::wstring errorStr;
		if( FAILED(prettyPrintHRESULT(errorStr, error)) ) {
			errorStr = std::to_wstring(error);
		}
		CONFIGUSER_LOGMESSAGE(logMsgPrefix + L"Retrieval of a filename using the key " + locators + L" failed with error: " + errorStr)
	} else if( HRESULT_CODE(error) == ERROR_DATA_NOT_FOUND ) {
		CONFIGUSER_LOGMESSAGE(logMsgPrefix + L"Retrieval of a filename using the key " + locators + L" returned no data.")
	} else {
		filenameAndPath = *value;
		value = 0;
	}

	if( FAILED(error) ) {
		return ERROR_SUCCESS;
	} else if( HRESULT_CODE(error) == ERROR_DATA_NOT_FOUND ) {
		return MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_BL_ENGINE, ERROR_DATA_NOT_FOUND);
	} else {
		locators.clear();
	}

	// Retrieve the directory
	if( !directoryField.empty() ) {
		error = config->retrieve<Config::DataType::DIRECTORY, std::wstring>(directoryScope, directoryField, value, &locators);
		if( FAILED(error) ) {
			std::wstring errorStr;
			if( FAILED(prettyPrintHRESULT(errorStr, error)) ) {
				errorStr = std::to_wstring(error);
			}
			CONFIGUSER_LOGMESSAGE(logMsgPrefix + L"Retrieval of a path using the key " + locators + L" failed with error: " + errorStr)
		} else if( HRESULT_CODE(error) == ERROR_DATA_NOT_FOUND ) {
			CONFIGUSER_LOGMESSAGE(logMsgPrefix + L"Retrieval of a path using the key " + locators + L" returned no data.")
		} else {
			error = fileUtil::combineAsPath(filenameAndPath, *value, filenameAndPath);
			value = 0;
			if( FAILED(error) ) {
				CONFIGUSER_LOGMESSAGE(logMsgPrefix + L"Combination of the filename and path using fileUtil::combineAsPath() failed.")
				return MAKE_HRESULT(SEVERITY_ERROR, FACILITY_BL_ENGINE, ERROR_FUNCTION_CALL);
			}
		}

		if( FAILED(error) ) {
			filenameAndPath.clear();
			return ERROR_SUCCESS;
		} else if( HRESULT_CODE(error) == ERROR_DATA_NOT_FOUND ) {
			filenameAndPath.clear();
			return MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_BL_ENGINE, ERROR_DATA_NOT_FOUND);
		}
	}
	
	quit = false;
	return ERROR_SUCCESS;
}

HRESULT ConfigUser::helper_IOPrivateConfig(
	std::wstring*& filename,
	const std::wstring& path,
	bool& quit,
	std::wstring*& filenameAndPath,
	const std::wstring& logMsgPrefix
	) {

	quit = true;

	// Error checking
	if( m_usage != Usage::PRIVATE ) {
		return MAKE_HRESULT(SEVERITY_ERROR, FACILITY_BL_ENGINE, ERROR_WRONG_STATE);
	} else if( (filename == 0) || (filenameAndPath != 0) ) {
		return MAKE_HRESULT(SEVERITY_ERROR, FACILITY_BL_ENGINE, ERROR_INVALID_INPUT);
	} else if( filename->empty() ) {
		return MAKE_HRESULT(SEVERITY_ERROR, FACILITY_BL_ENGINE, ERROR_INVALID_INPUT);
	}

	// Prepare combined filename and path
	if( !path.empty() ) {
		filenameAndPath = new std::wstring;
		if( FAILED(fileUtil::combineAsPath(*filenameAndPath, path, *filename)) ) {
			CONFIGUSER_LOGMESSAGE(logMsgPrefix + L"Combination of the filename and path using fileUtil::combineAsPath() failed.")
			delete filenameAndPath;
			filenameAndPath = 0;
			return MAKE_HRESULT(SEVERITY_ERROR, FACILITY_BL_ENGINE, ERROR_FUNCTION_CALL);
		}
	} else {
		filenameAndPath = filename;
	}

	quit = false;
	return ERROR_SUCCESS;
}