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
#include "LogUser.h"
#include "Config.h"

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
	   for the reading parameters in the global Config instance.

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
	// The basis for all other configuration data reading functions
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
	template<DataType D, typename T> bool insert(
		const std::wstring& scope, const std::wstring& field, const T* const value,
		const bool deleteValue);

	template<DataType D, typename T> bool retrieve(
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


template<typename ConfigIOClass> ConfigUser::ConfigUser(
	const bool enableLogging, const std::wstring& msgPrefix,
	const Config* locationSource,
	const std::wstring filenameScope,
	const std::wstring filenameField,
	const std::wstring directoryScope,
	const std::wstring directoryField
	) {}

template<typename ConfigIOClass> ConfigUser::ConfigUser(
	const bool enableLogging, const std::wstring& msgPrefix,
	const std::wstring filename,
	const std::wstring path
	) {}

template<typename ConfigIOClass> HRESULT ConfigUser::setPrivateConfig(const bool useOwnConfig,
	const Config* locationSource,
	const std::wstring filenameScope,
	const std::wstring filenameField,
	const std::wstring directoryScope,
	const std::wstring directoryField,
	const bool overwrite
	) {}

template<typename ConfigIOClass> HRESULT ConfigUser::setPrivateConfig(
	const std::wstring filename,
	const std::wstring path,
	const bool overwrite
	) {}

template<typename ConfigIOClass> HRESULT ConfigUser::setPrivateConfig(
	const std::wstring& filenameAndPath,
	const bool overwrite
	) {}

template<DataType D, typename T> bool ConfigUser::insert(
	const std::wstring& scope, const std::wstring& field, const T* const value,
	const bool deleteValue) {}

template<DataType D, typename T> bool ConfigUser::retrieve(
	const std::wstring& scope, const std::wstring& field, const T*& value) {}

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