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