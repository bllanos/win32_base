/*
Config.h
---------

Created for: Spring 2014 Direct3D 11 Learning
By: Bernard Llanos
May 25, 2014

Primary basis: None
Other references: None

Development environment: Visual Studio 2013 running on Windows 7, 64-bit
  -Note that the "Character Set" project property (Configuration Properties > General)
   should be set to Unicode for all configurations, when using Visual Studio.

Description
  -A wrapper for the std::unordered_map container supplying
   functions for retrieving specific kinds of data from the map.
  -Intended to be used as a key-value mapping containing configuration data.

Usage Notes
  -The Config object assumes that it owns all of the data that it stores,
   meaning that its destructor will delete all of the mapped values.
  -This class is currently not intended to be inherited from. It may need
   to be modified to be suitable for inheritance (e.g. so that it has
   virtual destructors).
*/

#pragma once

#include <windows.h>
#include <unordered_map>
#include <string>

class Config {

private:

	// Supported data types of property values
	enum class DataType : unsigned int {
		WSTRING
	};

	// Nested classes
private:
	/* A map value is a data type-value pair
	In order to safely delete data, it is necessary to store
	data types with the data.
	 */
	class Value {

	private:
		const DataType m_type;
		const void* const m_value;

	public:
		Value(const DataType type, const void* const value);
		~Value(void);

	public:
		/* Returns the value stored in this object,
		or null, if the value is not of the input data type.
		*/
		const void* getValue(const DataType type);

		// Currently not implemented - will cause linker errors if called
	private:
		Value(const Value& other);
		Value& operator=(const Value& other);
	};

	/* A key is a scope name combined with a property name
	*/
	class Key {

	private:
		const std::wstring m_scope;
		const std::wstring m_field;

	public:
		Key(const std::wstring& scope, const std::wstring& field);

	public:
		bool operator==(Key other) const;

		// Currently not implemented - will cause linker errors if called
	private:
		Key(const Key& other);
		Key& operator=(const Key& other);
	};


private:
	// Stores the key-value data pairs
	std::unordered_map<Key, Value> map;

public:
	// A default constructor and destructor is sufficient for now

private:
	// All insertion functions call this function
	HRESULT insert(const std::wstring& scope, const std::wstring& field,
		const DataType type, const void* const value);

	/* All retrieval functions call this function
	The return value is the output data, and is null if there
	is no value for the given key information (scope and field) in the map,
	or if a value exists for the key, but with a data type other than 'type'.
	*/
	const void* retrieve(const std::wstring& scope, const std::wstring& field,
		const DataType type) const;

	// Currently not implemented - will cause linker errors if called
private:
	Config(const Config& other);
	Config& operator=(const Config& other);

	// The public interface
	// --------------------
	/* All retrieval functions return null as their final (output) parameter
	if there is no value corresponding to the key parameters,
	or if there is a value corresponding to the key parameters,
	but it has a type other than the given parameter type.

	The output parameter of a retrieval function is a reference to a pointer.
	The pointer is a pointer to a constant object.
	*/
public:
	
	// wstring
	HRESULT insert(const std::wstring& scope, const std::wstring& field, const std::wstring* const value);
	HRESULT retrieve(const std::wstring& scope, const std::wstring& field, const std::wstring*& value) const;
};