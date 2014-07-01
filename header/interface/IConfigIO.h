/*
IConfigIO.h
---------------

Created for: Spring 2014 Direct3D 11 Learning
By: Bernard Llanos
May 25, 2014

Primary basis: None
Other references: None

Development environment: Visual Studio 2013 running on Windows 7, 64-bit
  -Note that the "Character Set" project property (Configuration Properties > General)
   should be set to Unicode for all configurations, when using Visual Studio.

Description
  -An interface class defining an object which can load a configuration file into
   a key-value data mapping and write a key-value data mapping to a
   configuration file.
  -Derived classes are to be created to handle various data formats
*/

#pragma once

#include <windows.h>
#include <string>
#include "Config.h"

class IConfigIO {

protected:
	IConfigIO(void) {}

public:
	virtual ~IConfigIO(void) {}

public:
	/* Read configuration data from a file

	Note that the Config object will be augmented, rather
	than having its existing data replaced. Keys already mapped in the Config
	object will not have their values overwritten, and the first mapping
	encountered in the file for a key not already mapped in the Config object
	will become the final mapping for that key.)

	Parsing warnings, errors, etc., may be appended to the configuration
	file in the form of comments. (They will not be read as data
	by instances of the same IConfigIO derived class as the object
	that logged them.)
	*/
	virtual HRESULT read(const std::wstring& filename,
		Config& config) = 0;

	/* Write configuration data to a file.
	The 'overwrite' parameter specifies whether to overwrite the file
	(true), or append to the file (false).
	*/
	virtual HRESULT write(const std::wstring& filename,
		const Config& config, const bool overwrite) = 0;

	// Currently not implemented - will cause linker errors if called
private:
	IConfigIO(const IConfigIO& other);
	IConfigIO& operator=(const IConfigIO& other);
};