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
public:
	virtual ~IConfigIO(void) {}

public:
	// Read configuration data from a file
	virtual HRESULT read(const std::wstring& filename,
		Config& config) = 0;

	// Write configuration data to a file
	virtual HRESULT write(const std::wstring& filename,
		const Config& config) = 0;

	// Currently not implemented - will cause linker errors if called
private:
	IConfigIO(const IConfigIO& other);
	IConfigIO& operator=(const IConfigIO& other);
};