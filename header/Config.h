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

Issues
  -Objects of this class are not safe for access by multiple threads
   (unless all threads are performing const operations).
*/

#pragma once

#include <windows.h>
#include <map>
#include <iterator>
#include <string>

class Config {

public:

	// Supported data types of property values
	// ---------------------------------------
	/* The use of this enumeration is as an alternative
	to using run-time type information (e.g. the typeid operator),
	for allowing a type-safe class interface and
	the storage of values of multiple data types in the same
	class instance (which is not possible with a class template).

	See http://msdn.microsoft.com/en-us/library/b2ay8610.aspx for
	more information on run-time type information.
	I am actually not sure if RTTI could be used to achieve all of
	the functionality currently implemented here.
	*/
	enum class DataType : unsigned int {
		WSTRING, BOOL
	};
	/* When adding new data types to this enumeration, also do the following:
	- Update the 's_dataTypesNames' and 's_dataTypesInOrder' static members
	- Update the Value class destructor
	- Add public retrieval and insertion member functions for values of the
	    new data type
	*/

	/* Outputs the DataType constant name that has the same form
	as the input string (case-sensitive).
	Returns a failure code if there is no corresponding DataType constant.
	*/
	static HRESULT wstringToDataType(DataType& out, const std::wstring& in);

	/* The inverse of wstringToDataType()
	Outpus the name corresponding to the data type enum constant
	*/
	static HRESULT dataTypeToWString(std::wstring& out, const DataType& in);

private:
	/* Number of constants in the DataType enumeration,
	which also corresponds to the length of the following
	two arrays
	*/
	static const size_t s_nDataTypes;

	// Names of data type constants as string literals, in order of declaration
	static const std::wstring s_dataTypesNames[];

	// DataType constants in order of declaration
	static const DataType s_dataTypesInOrder[];

	// Nested classes
public:
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
		const void* const getValue(const DataType type) const;

		// Currently not implemented - will cause linker errors if called
	private:
		Value(const Value& other);
		Value& operator=(const Value& other);
	};

	/* A key is a scope name combined with a property name
	The scope name can be an empty string, but not the property name.
	*/
	class Key {

	private:
		const std::wstring m_scope;
		const std::wstring m_field;

	public:
		Key(const std::wstring& scope, const std::wstring& field);

		// The default destructor is sufficient

	public:
		bool operator==(const Key& other) const;
		bool operator<(const Key& other) const;

	public:
		const std::wstring& getScope(void) const;
		const std::wstring& getField(void) const;

	public:
		Key(const Key& other);

		// Currently not implemented - will cause linker errors if called
	private:
		Key& operator=(const Key& other);
	};


private:

	/* Stores the key-value data pairs
	I am using an ordered map because it will make it easier to
	write the configuration data to a file ordered by scope name,
	then by field name (as defined by the '<' operator of the Key class).
	*/
	std::map<Key, Value*> m_map;

public:
	Config(void);

	~Config(void);

private:
	/* All insertion functions call this function
	Returns a failure code and does nothing if another map element
	is stored under the same key parameters.
	*/
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


	// The public interface: bulk data access for writing to a file
	// ------------------------------------------------------------
	/* These functions are proxies for the underlying STL container
	functions
	*/
public:
	std::map<Key, Value*>::const_iterator cbegin(void) const;
	std::map<Key, Value*>::const_iterator cend(void) const;

	// The public interface: insertion and retrieval of field data
	// -----------------------------------------------------------
	/* All retrieval functions return null as their final (output) parameter
	if there is no value corresponding to the key parameters,
	or if there is a value corresponding to the key parameters,
	but it has a type other than the given parameter type.

	The output parameter of a retrieval function is a reference to a pointer.
	The pointer is a pointer to a constant object.

	Insertion functions will return a failure code if the Config
	object already has a value stored with the given key parameters,
	if the value to be stored is a null pointer, or if the field string
	is empty.
	*/
public:
	
	// wstring
	HRESULT insert(const std::wstring& scope, const std::wstring& field, const std::wstring* const value);
	HRESULT retrieve(const std::wstring& scope, const std::wstring& field, const std::wstring*& value) const;

	// bool
	HRESULT insert(const std::wstring& scope, const std::wstring& field, const bool* const value);
	HRESULT retrieve(const std::wstring& scope, const std::wstring& field, const bool*& value) const;

};