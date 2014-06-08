/*
FlatAtomicConfigIO.h
--------------------

Created for: Spring 2014 Direct3D 11 Learning
By: Bernard Llanos
May 31, 2014

Primary basis: None
Other references: None

Development environment: Visual Studio 2013 running on Windows 7, 64-bit
  -Note that the "Character Set" project property (Configuration Properties > General)
   should be set to Unicode for all configurations, when using Visual Studio.

Description
  -A class which inherits from the IConfigIO interface
  -See below for the data format accepted by this configuration read/write class
    -"Flat" refers to the fact that Config objects handled cannot have nested data structures
	 (e.g. such as a Config object being stored with a given key inside another Config object)
	-"Atomic" refers to the fact that only single values are stored under each key in the Config
	 objects handled by this class, rather than lists/arrays/sets of values.
*/

#pragma once

#include <windows.h>
#include <string>
#include "defs.h"
#include "LogUser.h"
#include "Config.h"
#include "IConfigIO.h"

/* This guide will be written to configuration files
created from Config objects by instances of this class.
*/
#define FLATATOMICCONFIGIO_DATA_FORMATSPEC \
	L"# ----------------------------------------------------------------------------\n"\
	L"# The FlatAtomicConfigIO class, used to process this file \n"\
	L"# defines the following file format:\n"\
	L"#\n"\
	L"# ASCII encoding is expected."\
	L"#\n"\
	L"# Lines are either\n"\
	L"# \t-Blank\n"\
	L"# \t-Comments, starting with optional whitespace,\n"\
	L"# \t\t followed by '#', optional whitespace, and then the comment text\n"\
	L"# \t-Configuration key-value pairs\n"\
	L"#\n"\
	L"# Whitespace is any character, other than '\\n',"\
	L"# with an ASCII decimal value from 1 (inclusive) to 32 (inclusive), or 127."\
	L"#\n"\
	L"# A configuration key value pair is, in the same order,\n"\
	L"# \t Optional whitespace\n"\
	L"# \t A datatype\n"\
	L"# \t Optionally a key scope string (any characters), followed by '::'\n"\
	L"# \t\t ('::' is optional if a scope string is not present)\n"\
	L"# \t A key field parameter (any characters), followed by '='\n"\
	L"# \t A value\n"\
	L"# \t Optional whitespace\n"\
	L"#\n"\
	L"# Each of the above elements can optionally be separated by whitespace\n"\
	L"# from the others, but any whitespace within an element\n"\
	L"# will be stripped during processing.\n"\
	L"#\n"\
	L"# Datatypes must match one of the 'DataType' enumeration constant names\n"\
	L"# found in Config.h\n"\
	L"# that corresponds to either a wide character string or a fundamental type.\n"\
	L"# The matching is case-sensitive,\n"\
	L"# and is implemented in the Config::wstringToDataType() function.\n"\
	L"#\n"\
	L"# Values are C++ literals corresponding to the datatypes\n"\
	L"# at the start of the line.\n"\
	L"# For exammple, in the case of WSTRING literals,\n"\
	L"# they must start with 'L\"' and end with '\"'.\n"\
	L"# ----------------------------------------------------------------------------\n"


class FlatAtomicConfigIO : public IConfigIO, public LogUser {

private:
	// Stores the types of data that this class can process
	static const size_t s_nSupportedDataTypes;
	static const Config::DataType s_supportedDataTypes[];

public:
	// Returns true if this class can process this data type
	static bool isSupportedDataType(Config::DataType type);

public:
	FlatAtomicConfigIO(void);
	virtual ~FlatAtomicConfigIO(void);

public:
	/* Read configuration data from a file
	This function is responsible for ignoring comment lines
	in the file.
	*/
	virtual HRESULT read(const std::wstring& filename,
		Config& config) override;

	// Write configuration data to a file
	virtual HRESULT write(const std::wstring& filename,
		const Config& config) override;

protected:
	/* Processes a data type, key, value line from the input file
	(the first parameter). If successful, adds the data to the
	Config object passed as the second parameter.
	All newline characters, '\n' and equivalent, are expected to have
	been stripped, and the line is expected to be null-terminated.
	(Therefore, the 'line' could actually have been multiple lines
	in the input file.)
	*/
	HRESULT readDataLine(Config& config, const char* const str);

	/* Format a key-value pair as a string
	Only values of data types which are supported by this class
	can be processed. (Refer to the isSupportedDataType() function)
	*/
	HRESULT writeDataLine(std::wstring& str, const Config& config);

	// Currently not implemented - will cause linker errors if called
private:
	FlatAtomicConfigIO(const FlatAtomicConfigIO& other);
	FlatAtomicConfigIO& operator=(const FlatAtomicConfigIO& other);
};