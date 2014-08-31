/*
ConfigUser.h
------------

Created for: Spring 2014 Direct3D 11 Learning
By: Bernard Llanos
August 30, 2014

Primary basis: None
Other references: None

Development environment: Visual Studio 2013 running on Windows 7, 64-bit
  -Note that the "Character Set" project property (Configuration Properties > General)
   should be set to Unicode for all configurations, when using Visual Studio.

Description
  -A base class that provides simplified use
     of the Config class for its descendants

  -Allows for reading, writing, and switching configuration data

  -Allows for setting the visibility of the configuration data
	 used by the object (i.e. its visibility from
	 the perspective of other objects).
	 
	 As currently implemented, the constructor
	 determines whether this object uses the global Config instance,
	 a shared Config instance, or its own Config instance.
	 There is no mechanism for changing this visibility/usage setting later.

  -Uses its inherited LogUser functionality to record how configuration
     data is used and any errors that occur during interactions
	 with configuration data. (This behaviour can be turned on or off,
	 and is initially enabled by default.)

Notes
  -The globally-visible Config object is initialized and destroyed in main.cpp,
     rather than being managed by this class, despite the fact that objects
     of this class may make use of the global Config object.
  -This class is derived from LogUser. Any classes derived from this class
     should not declare LogUser as a base class, unless the intention is
	 to have multiple copies of the LogUser base.
*/

#pragma once

#include <Windows.h>
#include <string>
#include "globals.h"
#include "LogUser.h"
#include "Config.h"
#include "fileUtil.h"
#include "IConfigIO.h"

class ConfigUser : public LogUser {

public:
	/* An enumeration describing the visibility
	   of the Config instance used by this object
	   -----------------------------------------------------------------

	   GLOBAL = The object will use the global Config object.

	   SHARED = The object can be passed a Config object to use,
	              and will not rebuild or delete it.
				  Note: The object will not use the global Config object.
	   
	   PRIVATE = The object can create a Config object over which
	               it has full control, but which is not
				   visible to other objects.

	   Each usage type imposes restrictions on the
	   operations that this object can perform with respect
	   to configuration data, as will be described
	   in the documentation comments of this class's function
	   members.
	 */
	enum class Usage : unsigned int {
		GLOBAL, SHARED, PRIVATE
	};

	// Data members
	// -----------------------------------------------------------------
private:
	// Switch for turning logging specific to this class on or off
	bool m_configUseLoggingEnabled;

	/* The Config instance used by this class. This will be
	   null in cases where the global Config instance is being used.
	 */
	Config* m_config;

	// The way in which this object is using Config instance(s)
	Usage m_usage;

	// Constructors
	// -----------------------------------------------------------------
	/* Note that the constructors for objects with PRIVATE configuration
	   data usage may produce a small amount of logging output.

	   The first two parameters of all constructors, 'enableLogging'
	   and 'msgPrefix', are forwarded to the LogUser constructor.
	   'enableLogging' is not used by this class itself. It acts like
	   LogUser::enableLogging() or LogUser::disableLogging(),
	   as opposed to ConfigUser::enableConfigUseLogging()
	   or ConfigUser::disableConfigUseLogging().
	 */
protected:
	/* Creates an object with GLOBAL configuration data usage,
	   if 'usage' is GLOBAL.

	   If 'usage' is PRIVATE or SHARED, creates an object with
	   PRIVATE or SHARED configuration data usage,
	   respectively, but no initial Config instance.
	 */
	ConfigUser(const bool enableLogging, const std::wstring& msgPrefix,
		Usage usage);

	/* Creates an object with SHARED configuration data usage.
	   The object will use the 'sharedConfig' Config instance.
	   If 'sharedConfig' is null, the object will not have
	   access to a Config instance until a non-null
	   Config instance is passed to setSharedConfig().
	 */
	ConfigUser(const bool enableLogging, const std::wstring& msgPrefix,
		Config* sharedConfig);

	/* Creates an object with PRIVATE configuration data usage.
	   
	   This constructor's parameters are used to retrieve the
	   location of a configuration file
	   from an existing Config object, 'locationSource'.
	   The configuration file (if it exists) will be used to
	   construct this object's Config instance.

	   (filenameScope, filenameField) = Key used to retrieve the
	     name of the file, if 'directoryScope' and 'directoryField'
		 are not both empty. If 'directoryScope' and 'directoryField'
		 are both empty, this key is expected to provide the
		 combined name and path of the file.
		 This key must correspond to a value of type Config::DataType::FILENAME.

	   (directoryScope, directoryField) = Key used to retrieve
	     the directory in which the configuration file
		 is to be found (i.e. the file's path).
		 This key must correspond to a value of type Config::DataType::DIRECTORY.

	   If 'locationSource' is null, the object will look
	   for the filepath parameters in the global Config instance.

	   If all parameters are empty strings or null, this is equivalent
	   to calling ConfigUser(Usage usage) where 'usage' is PRIVATE.

	   The type parameter, 'ConfigIOClass', is the type of loader to use for
	   reading the configuration file. Therefore, 'ConfigIOClass' must be
	   derived from the IConfigIO interface class.
	 */
	template<typename ConfigIOClass> ConfigUser(
		const bool enableLogging, const std::wstring& msgPrefix,
		const Config* locationSource,
		const std::wstring filenameScope,
		const std::wstring filenameField,
		const std::wstring directoryScope = L"",
		const std::wstring directoryField = L""
		);

	/* Creates an object with PRIVATE configuration data usage.

	   This constructor's parameters, 'filename' and 'path',
	   are the name and path, respectively, of the configuration
	   file to load when creating this object's Config instance.

	   'path' can be empty, in which case 'filename' must
	   contain a combined filename and path.

	   If all parameters are empty strings, this is equivalent
	   to calling ConfigUser(Usage usage) where 'usage' is PRIVATE.

	   The type parameter, 'ConfigIOClass', is the type of loader to use for
	   reading the configuration file. Therefore, 'ConfigIOClass' must be
	   derived from the IConfigIO interface class.
	 */
	template<typename ConfigIOClass> ConfigUser(
		const bool enableLogging, const std::wstring& msgPrefix,
		const std::wstring filename,
		const std::wstring path = L""
		);

public:
	virtual ~ConfigUser(void);

	/* Public getters and setters
	   -----------------------------------------------------------------
	   The following functions should be used only with some knowledge
	   of what the specific class of this object uses configuration data for.
	 */
public:
	/* This function will do nothing and return
	   a failure code if this object has a usage
	   type other than SHARED, or if the
	   'sharedConfig' parameter is the global Config instance.

	   If 'sharedConfig' is null, the object will
	   not have access to a Config instance
	   (until this function is passed a non-null parameter).

	   Unlike the change of a PRIVATE Config instance (refer to the setter
	   functions below), this function will not generate logging output
	   if the operation succeeds.
	 */
	HRESULT setSharedConfig(Config* sharedConfig);

	/* Returns the Config instance used by this object.
	   Does nothing and returns a failure code if this object
	   does not have the SHARED usage type, or if the
	   'sharedConfig' parameter is not passed in as a null pointer.
	 */
	HRESULT getSharedConfig(Config* sharedConfig) const;

	/* These functions toggle whether or not this object
	   will output log messages relating to its use of
	   configuration data.

	   Note that the LogUser member function disableLogging()
	   will also suppress configuration usage logging output.
	   However, the LogUser member functions enableLogging()
	   and disableLogging() do not change the ConfigUser class's
	   logging output switch.

	   ConfigUser instances are constructed with configuration data
	   usage logging enabled by default.
	 */
	void enableConfigUseLogging();
	void disableConfigUseLogging();

	/* Outputs whether or not this object has access to a Config instance.
	   If this object has the GLOBAL usage type, this function will
	   return false only if the global Config instance is null.

	   Note that this function will return true if the Config instance
	   used by this object is empty.
	 */
	bool hasConfig() const;

	/* Returns the usage type of this object. */
	Usage getUsage() const;


	/* Protected setter functions, for use only with the PRIVATE
	   usage type.
	   -----------------------------------------------------------------

	   These functions behave the same way as the constructors
	   with the corresponding signatures, except where noted otherwise.

	   All of the following functions will return failure codes
	   and do nothing if the object has a usage type other than PRIVATE.

	   These functions will log messages to describe errors output by the IConfigIO objects
	   used to read the data. The return values of these functions will, where appropriate,
	   be copies of the results returned by the IConfigIO objects.

	   When any of these functions succeed, they will log messages to make note of the change
	   to this object's Config instance.
	 */
protected:
	// Deletes this object's Config instance
	HRESULT deletePrivateConfig();

	/* If 'useOwnConfig' is true, and 'locationSource' is null,
	   the name and path of the configuration file will be
	   looked for in this object's current Config instance.
	   If 'useOwnConfig' is true and 'locationSource' is not
	   null, the function will do nothing and return a failure code.

	   If 'overwrite' is true, this object's Config instance
	   will be replaced with an instance containing the loaded data.
	   If 'overwrite is false, the loaded data will instead
	   be inserted into the existing Config instance.

	   If all key scope and field parameters are empty, this
	   function will return a failure result and do nothing.

	   If configuration data cannot be found, or is not complete,
	   this function will return a success result with the codes
	   ERROR_DATA_NOT_FOUND or ERROR_DATA_INCOMPLETE, respectively, and will
	   log a message describing the issue.
	 */
	template<typename ConfigIOClass> HRESULT setPrivateConfig(const bool useOwnConfig,
		const Config* locationSource,
		const std::wstring filenameScope,
		const std::wstring filenameField,
		const std::wstring directoryScope = L"",
		const std::wstring directoryField = L"",
		const bool overwrite = true
		);

	/* If 'overwrite' is true, this object's Config instance
	   will be replaced with an instance containing the loaded data.
	   If 'overwrite is false, the loaded data will instead
	   be inserted into the existing Config instance.

	   If both 'filename' and 'path' are empty, this function
	   will return a failure result and do nothing.

	   If configuration data cannot be found, or is not complete,
	   this function will return a success result with the codes
	   ERROR_DATA_NOT_FOUND or ERROR_DATA_INCOMPLETE, respectively, and will
	   log a message describing the issue.
	 */
	template<typename ConfigIOClass> HRESULT setPrivateConfig(
		const std::wstring filename,
		const std::wstring path = L"",
		const bool overwrite = true
		);

private:
	/* The basis for all other configuration data reading functions
	   This function relies on its callers to perform some error checking
	   (which is acceptable since it is 'private').

	   Returns a failure code and does nothing if 'filenameAndPath' is empty.

	   If an IConfigIO data reading operation fails, this object's Config instance
	   will not be overwritten (when the 'overwrite' parameter is true), and
	   this function will log the error, but return a success result.
	   If 'overwrite' is false and the data reading operation fails,
	   this object's Config instance may have been modified, if it existed,
	   but will not have been assigned to a Config instance if it did not exist.

	   Note that, if the reading operation succeeds, but returns the code
	   ERROR_DATA_INCOMPLETE, this object's Config instance will likely
	   have been augmented (if 'overwrite' is false), or will have been replaced
	   with a Config instance that is missing some data (if 'overwrite' is true).
	   In this case, this function will also return ERROR_DATA_INCOMPLETE.
	 */
	template<typename ConfigIOClass> HRESULT setPrivateConfig(
		const std::wstring& filenameAndPath,
		const bool overwrite
		);


	/* Proxies for the Config class insertion and retrieval functions.
	   -----------------------------------------------------------------

	   These functions return 'true' if data was retrieved/inserted successfully,
	   and 'false' if there was an error, or if no data was retrieved/inserted.

	   Internally, the functions will log the exact situation in more detail
	   (e.g. to distinguish errors from 'not found' or 'already present' results).
	   No messages will be logged when the functions return 'true'.
	 */
protected:
	/* If 'deleteValue' is true, the function will delete the 'value' pointer
	   if the insertion does not occur. (This unburdens the client of some
	   memory management.)
	 */
	template<Config::DataType D, typename T> bool insert(
		const std::wstring& scope, const std::wstring& field, const T* const value,
		const bool deleteValue);

	template<Config::DataType D, typename T> bool retrieve(
		const std::wstring& scope, const std::wstring& field, const T*& value);


	/* Configuration data output
	   -----------------------------------------------------------------

	   The 'overwrite' parameter indicates whether to append to (false)
	   or replace (true) the file to which data will be output.

	   The 'outputContext' parameter indicates whether to have the IConfigIO object
	   used to write the configuration data also output contextual information
	   (e.g. configuration data formatting guidelines).

	   Otherwise, the function parameters have the same meanings and effect
	   as with the PRIVATE usage configuration setter functions.

	   These functions will do nothing and return a failure result
	   if this object has a usage type other than PRIVATE.

	   These functions will log messages to describe errors output by the IConfigIO objects
	   used to write the data. The return values of these functions will, where appropriate,
	   be copies of the results returned by the IConfigIO objects.
	 */
public:
	template<typename ConfigIOClass> HRESULT writePrivateConfig(const bool useOwnConfig,
		const Config* locationSource,
		const std::wstring filenameScope,
		const std::wstring filenameField,
		const std::wstring directoryScope = L"",
		const std::wstring directoryField = L"",
		const bool overwrite = true,
		const bool outputContext = true
		);

	template<typename ConfigIOClass> HRESULT writePrivateConfig(
		const std::wstring filename,
		const std::wstring path = L"",
		const bool overwrite = true,
		const bool outputContext = true
		);

private:
	// The basis for all other configuration data writing functions
	template<typename ConfigIOClass> HRESULT writePrivateConfig(
		const std::wstring& filenameAndPath,
		const bool overwrite
		);

	// Currently not implemented - will cause linker errors if called
private:
	ConfigUser(const ConfigUser& other);
	ConfigUser& operator=(const ConfigUser& other);
};


#define CONFIGUSER_LOGMESSAGE(msg) \
	if( m_configUseLoggingEnabled ) logMessage( msg, false, true );

template<typename ConfigIOClass> ConfigUser::ConfigUser(
	const bool enableLogging, const std::wstring& msgPrefix,
	const Config* locationSource,
	const std::wstring filenameScope,
	const std::wstring filenameField,
	const std::wstring directoryScope,
	const std::wstring directoryField
	) :
	LogUser(enableLogging, msgPrefix),
	m_config(0), m_configUseLoggingEnabled(true), m_usage(Usage::PRIVATE)
{
	if( !filenameField.empty() ) {
		HRESULT error = setPrivateConfig<ConfigIOClass>(
			false,
			locationSource,
			filenameScope,
			filenameField,
			directoryScope,
			directoryField,
			true);

		if( FAILED(error) ) {
			std::wstring errorStr;
			if( FAILED(prettyPrintHRESULT(errorStr, error)) ) {
					errorStr = std::to_wstring(error);
			}
			CONFIGUSER_LOGMESSAGE(L"Usage::PRIVATE constructor call to setPrivateConfig([key arguments]) failed with error: " + errorStr)
		}
	}
}

template<typename ConfigIOClass> ConfigUser::ConfigUser(
	const bool enableLogging, const std::wstring& msgPrefix,
	const std::wstring filename,
	const std::wstring path
	) :
	LogUser(enableLogging, msgPrefix),
	m_config(0), m_configUseLoggingEnabled(true), m_usage(Usage::PRIVATE)
{
	if( !filename.empty() ) {
		HRESULT error = setPrivateConfig<ConfigIOClass>(
			filename,
			path,
			true);

		if( FAILED(error) ) {
			std::wstring errorStr;
			if( FAILED(prettyPrintHRESULT(errorStr, error)) ) {
				errorStr = std::to_wstring(error);
			}
			CONFIGUSER_LOGMESSAGE(L"Usage::PRIVATE constructor call to setPrivateConfig([filepath arguments]) failed with error: " + errorStr)
		}
	}
}

template<typename ConfigIOClass> HRESULT ConfigUser::setPrivateConfig(const bool useOwnConfig,
	const Config* locationSource,
	const std::wstring filenameScope,
	const std::wstring filenameField,
	const std::wstring directoryScope,
	const std::wstring directoryField,
	const bool overwrite
	)
{
	// Error checking
	if( m_usage != Usage::PRIVATE ) {
		return MAKE_HRESULT(SEVERITY_ERROR, FACILITY_BL_ENGINE, ERROR_WRONG_STATE);
	} else if( useOwnConfig && (locationSource != 0) ) {
		return MAKE_HRESULT(SEVERITY_ERROR, FACILITY_BL_ENGINE, ERROR_INVALID_INPUT);
	} else if( (m_config == 0) && (useOwnConfig || !overwrite) ) {
		return MAKE_HRESULT(SEVERITY_ERROR, FACILITY_BL_ENGINE, ERROR_WRONG_STATE);
	} else if( filenameField.empty() ) {
		return MAKE_HRESULT(SEVERITY_ERROR, FACILITY_BL_ENGINE, ERROR_INVALID_INPUT);
	} else if( !directoryScope.empty() && directoryField.empty() ) {
		return MAKE_HRESULT(SEVERITY_ERROR, FACILITY_BL_ENGINE, ERROR_INVALID_INPUT);
	}

	// Set the Config instance to query for the filepath
	Config* config = (useOwnConfig) ? m_config : ((locationSource == 0) ? g_defaultConfig : locationSource);
	if( config == 0 ) {
		CONFIGUSER_LOGMESSAGE(L"setPrivateConfig([key arguments]): Config pointer that would be used to obtain a filepath is null.")
		return MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_BL_ENGINE, ERROR_DATA_NOT_FOUND);
	}

	std::wstring filenameAndPath;
	const std::wstring* value;
	std::wstring locators;

	// Retrieve the filename, or filename and path
	HRESULT error = config->retrieve<Config::DataType::FILENAME, std::wstring>(filenameScope, filenameField, value, &locators);
	if( FAILED(error) ) {
		std::wstring errorStr;
		if( FAILED(prettyPrintHRESULT(errorStr, error)) ) {
			errorStr = std::to_wstring(error);
		}
		CONFIGUSER_LOGMESSAGE(L"setPrivateConfig([key arguments]): retrieval of a filename using the key " + locators + L" failed with error: " + errorStr)
	} else if( HRESULT_CODE(error) == ERROR_DATA_NOT_FOUND ) {
		CONFIGUSER_LOGMESSAGE(L"setPrivateConfig([key arguments]): retrieval of a filename using the key " + locators + L" returned no data.")
	} else {
		filenameAndPath = *value;
		value = 0;
	}

	if( FAILED(error) ) {
		return ERROR_SUCCESS;
	} else if(HRESULT_CODE(error) == ERROR_DATA_NOT_FOUND ) {
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
			CONFIGUSER_LOGMESSAGE(L"setPrivateConfig([key arguments]): retrieval of a path using the key " + locators + L" failed with error: " + errorStr)
		} else if( HRESULT_CODE(error) == ERROR_DATA_NOT_FOUND ) {
			CONFIGUSER_LOGMESSAGE(L"setPrivateConfig([key arguments]): retrieval of a path using the key " + locators + L" returned no data.")
		} else {
			error = fileUtil::combineAsPath(filenameAndPath, *value, filenameAndPath);
			value = 0;
			if( FAILED(error) ) {
				CONFIGUSER_LOGMESSAGE(L"setPrivateConfig([key arguments]): combination of the filename and path using fileUtil::combineAsPath() failed.")
				return MAKE_HRESULT(SEVERITY_ERROR, FACILITY_BL_ENGINE, ERROR_FUNCTION_CALL);
			}
		}

		if( FAILED(error) ) {
			return ERROR_SUCCESS;
		} else if( HRESULT_CODE(error) == ERROR_DATA_NOT_FOUND ) {
			return MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_BL_ENGINE, ERROR_DATA_NOT_FOUND);
		}
	}

	// Perform the change of Config instance
	return setPrivateConfig<ConfigIOClass>(filenameAndPath, overwrite);
}

template<typename ConfigIOClass> HRESULT ConfigUser::setPrivateConfig(
	const std::wstring filename,
	const std::wstring path,
	const bool overwrite
	)
{
	// Error checking
	if( m_usage != Usage::PRIVATE ) {
		return MAKE_HRESULT(SEVERITY_ERROR, FACILITY_BL_ENGINE, ERROR_WRONG_STATE);
	} else if( (m_config == 0) && !overwrite ) {
		return MAKE_HRESULT(SEVERITY_ERROR, FACILITY_BL_ENGINE, ERROR_WRONG_STATE);
	} else if( filename.empty() ) {
		return MAKE_HRESULT(SEVERITY_ERROR, FACILITY_BL_ENGINE, ERROR_INVALID_INPUT);
	}

	// Prepare combined filename and path
	std::wstring* filenameAndPath = 0;
	if( !path.empty() ) {
		filenameAndPath = new std::wstring;
		if( FAILED(fileUtil::combineAsPath(*filenameAndPath, path, filename)) ) {
			CONFIGUSER_LOGMESSAGE(L"setPrivateConfig([filepath arguments]): combination of the filename and path using fileUtil::combineAsPath() failed.")
			delete filenameAndPath;
			return MAKE_HRESULT(SEVERITY_ERROR, FACILITY_BL_ENGINE, ERROR_FUNCTION_CALL);
		}
	} else {
		filenameAndPath = &filename;
	}

	// Perform the change of Config instance
	HRESULT result = setPrivateConfig<ConfigIOClass>(*filenameAndPath, overwrite);

	if( filenameAndPath != &filename ) {
		delete filenameAndPath;
	}
	return result;
}

template<typename ConfigIOClass> HRESULT ConfigUser::setPrivateConfig(
	const std::wstring& filenameAndPath,
	const bool overwrite
	)
{
	// Error checking
	if( filenameAndPath.empty() ) {
		return MAKE_HRESULT(SEVERITY_ERROR, FACILITY_BL_ENGINE, ERROR_INVALID_INPUT);
	} else {
		if( overwrite ) {
			CONFIGUSER_LOGMESSAGE(L"setPrivateConfig() base function: called with file = " + filenameAndPath + L" and overwrite = true.")
		} else {
			CONFIGUSER_LOGMESSAGE(L"setPrivateConfig() base function: called with file = " + filenameAndPath + L" and overwrite = false.")
		}
	}

	IConfigIO* configIO = new ConfigIOClass;
	Config* config = (overwrite || (m_config == 0)) ? new Config : m_config;

	HRESULT error = configIO->read(filenameAndPath, config);
	if( FAILED(error) ) {
		std::wstring errorStr;
		if( FAILED(prettyPrintHRESULT(errorStr, error)) ) {
			errorStr = std::to_wstring(error);
		}
		CONFIGUSER_LOGMESSAGE(L"setPrivateConfig() base function: Configuration data failed to load with error: " + errorStr)

		if( config != m_config ) {
			CONFIGUSER_LOGMESSAGE(L"setPrivateConfig() base function: Config instance has not been changed.")
			delete config;
		} else {
			CONFIGUSER_LOGMESSAGE(L"setPrivateConfig() base function: Config instance may have been augmented.")
		}
		error = ERROR_SUCCESS; // The error has been logged, so it can be forgotten.

	} else {
		if( HRESULT_CODE(error) == ERROR_DATA_INCOMPLETE ) {
			CONFIGUSER_LOGMESSAGE(L"setPrivateConfig() base function: Configuration data reading operation indicated that the data is incomplete.")
		}
		
		if( config != m_config ) {
			if( m_config != 0 ) {
				CONFIGUSER_LOGMESSAGE(L"setPrivateConfig() base function: This object's Config instance was overwritten.")
				delete m_config;
			} else {
				CONFIGUSER_LOGMESSAGE(L"setPrivateConfig() base function: Config instance was (re-)initialized.")
			}
			m_config = config;
		} else {
			CONFIGUSER_LOGMESSAGE(L"setPrivateConfig() base function: Config instance was augmented (assuming the loaded data was non-empty and at least some keys were new).")
		}
	}

	// Cleanup
	if( configIO != 0 ) {
		delete configIO;
	}

	return error;
}

template<Config::DataType D, typename T> bool ConfigUser::insert(
	const std::wstring& scope, const std::wstring& field, const T* const value,
	const bool deleteValue)
{
	bool result = false;
	std::wstring locators;

	HRESULT error = config->insert<D, T>(scope, field, value, &locators);
	if( FAILED(error) ) {
		std::wstring errorStr;
		if( FAILED(prettyPrintHRESULT(errorStr, error)) ) {
			errorStr = std::to_wstring(error);
		}
		CONFIGUSER_LOGMESSAGE(L"insert() using the key " + locators + L" failed with error: " + errorStr)
	} else if( HRESULT_CODE(error) == ERROR_ALREADY_ASSIGNED ) {
		CONFIGUSER_LOGMESSAGE(L"insert() using the key " + locators + L" did not proceed as the key is already associated with data.")
	} else {
		result = true;
	}

	if( deleteValue && !result && value != 0 ) {
		delete value;
		value = 0;
	}
	return result;
}

template<Config::DataType D, typename T> bool ConfigUser::retrieve(
	const std::wstring& scope, const std::wstring& field, const T*& value)
{
	bool result = false;
	std::wstring locators;

	HRESULT error = config->retrieve<D, T>(scope, field, value, &locators);
	if( FAILED(error) ) {
		std::wstring errorStr;
		if( FAILED(prettyPrintHRESULT(errorStr, error)) ) {
			errorStr = std::to_wstring(error);
		}
		CONFIGUSER_LOGMESSAGE(L"retrieve() using the key " + locators + L" failed with error: " + errorStr)
	} else if( HRESULT_CODE(error) == ERROR_DATA_NOT_FOUND ) {
		CONFIGUSER_LOGMESSAGE(L"retrieve() using the key " + locators + L" returned no data.")
	} else {
		result = true;
	}
	return result;
}

template<typename ConfigIOClass> HRESULT ConfigUser::writePrivateConfig(const bool useOwnConfig,
	const Config* locationSource,
	const std::wstring filenameScope,
	const std::wstring filenameField,
	const std::wstring directoryScope,
	const std::wstring directoryField,
	const bool overwrite,
	const bool outputContext
	) {}

template<typename ConfigIOClass> HRESULT ConfigUser::writePrivateConfig(
	const std::wstring filename,
	const std::wstring path,
	const bool overwrite,
	const bool outputContext
	) {}

template<typename ConfigIOClass> HRESULT ConfigUser::writePrivateConfig(
	const std::wstring& filenameAndPath,
	const bool overwrite
	) {}