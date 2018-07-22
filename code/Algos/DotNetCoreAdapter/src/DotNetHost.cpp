#include "stdafx.h"

#include <exception>

#include <DotNetCoreAdapter\DotNetHost.h>
#include <DotNetCoreAdapter\coreclrhost.h>

#ifdef _WINDOWS
    const char CoreClrFilename[] = "coreclr.dll";
#else
    const char CoreClrFilename[] = "libcoreclr.so";
#endif

namespace
{

std::string AddFilesFromDirectory(const std::string &path)
{
    // TRUSTED_PLATFORM_ASSEMBLIES
    // "Trusted Platform Assemblies" are prioritized by the loader and always loaded with full trust.
    // A common pattern is to include any assemblies next to CoreCLR.dll as platform assemblies.
    // More sophisticated hosts may also include their own Framework extensions (such as AppDomain managers)
    // in this list.
    std::string trustedPlatformAssemblies;

    // Extensions to probe for when finding TPA list files
    const char *tpaExtensions[] = {
	    "*.dll",
	    "*.exe",
	    "*.winmd"
    };

    // Probe next to CoreCLR.dll for any files matching the extensions from tpaExtensions and
    // add them to the TPA list. In a real host, this would likely be extracted into a separate function
    // and perhaps also run on other directories of interest.
    for (int i = 0; i < _countof(tpaExtensions); i++)
    {
	    // Construct the file name search pattern
	    std::string searchPath = path;
	    searchPath += "\\";
	    searchPath +=  tpaExtensions[i];

	    // Find files matching the search pattern
	    WIN32_FIND_DATAA findData;
	    HANDLE fileHandle = FindFirstFileA(searchPath.data(), &findData);

	    if (fileHandle != INVALID_HANDLE_VALUE)
	    {
		    do
		    {
			    // Construct the full path of the trusted assembly
			    std::string pathToAdd = path;
			    pathToAdd += "\\";
			    pathToAdd += findData.cFileName;

			    // Add the assembly to the list and delimited with a semi-colon
			    trustedPlatformAssemblies += pathToAdd;
			    trustedPlatformAssemblies += ";";

			    // Note that the CLR does not guarantee which assembly will be loaded if an assembly
			    // is in the TPA list multiple times (perhaps from different paths or perhaps with different NI/NI.dll
			    // extensions. Therefore, a real host should probably add items to the list in priority order and only
			    // add a file if it's not already present on the list.
			    //
			    // For this simple sample, though, and because we're only loading TPA assemblies from a single path,
			    // we can ignore that complication.
		    }
		    while (FindNextFileA(fileHandle, &findData));
		    FindClose(fileHandle);
	    }
    }

    return trustedPlatformAssemblies;
}

} // end of namespace

struct DotNetHost::Implementation
{
    std::string ExePath;
    std::string CorePath;

    void *HostHandle = nullptr;
    unsigned int DomainID = 0;

    std::unique_ptr<AlgosBridge::LibraryLoader> LibraryLoader;

    coreclr_create_delegate_ptr CreateDelegate = nullptr;
};

DotNetHost::DotNetHost(const std::string &exePath, const std::string &corePath) : m_Self(new Implementation)
{
    m_Self->ExePath = exePath;
    m_Self->CorePath = corePath;
}

DotNetHost::~DotNetHost()
{
    delete m_Self;
}

void DotNetHost::Initialize()
{
    auto coreClrFilename = m_Self->CorePath + "\\coreclr.dll";

    m_Self->LibraryLoader = std::make_unique<AlgosBridge::LibraryLoader>(coreClrFilename);

    auto coreclr_initialize = m_Self->LibraryLoader->TryGetFunction<coreclr_initialize_ptr>("coreclr_initialize");
    if(coreclr_initialize == nullptr) throw std::exception("could not find coreclr_initialize"); 

    const char *propertyKeys[] = 
    {
        "APP_PATHS",
        "TRUSTED_PLATFORM_ASSEMBLIES"
    };

    auto tpa = AddFilesFromDirectory(m_Self->CorePath);

    const char *propertyValues[] = 
    {
        // APP_PATHS
        m_Self->ExePath.data(),
        // TRUSTED_PLATFORM_ASSEMBLIES
        tpa.data()
    };

    void *hostHandle = nullptr;
    unsigned int domainID = 0;

    int ret = coreclr_initialize
    (
        m_Self->ExePath.data(),
        "host",
        sizeof(propertyValues) / sizeof(propertyValues[0]),
        propertyKeys,
        propertyValues,
        &m_Self->HostHandle,
        &m_Self->DomainID
    );

    if(ret != 0) throw std::exception("could not initialize core clr");

    m_Self->CreateDelegate = m_Self->LibraryLoader->TryGetFunction<coreclr_create_delegate_ptr>("coreclr_create_delegate");
    if(m_Self->CreateDelegate == nullptr) throw std::exception("could not find create delegate"); 
}

void *DotNetHost::DoTryCreateDelegate(const std::string &assembly, const std::string &fullyQualifiedTypeName, const std::string &method)
{
    if(m_Self->CreateDelegate == nullptr) throw std::exception("not initialized");

    void *methodPtr = nullptr;
    auto ret = m_Self->CreateDelegate
    (
        m_Self->HostHandle,
        m_Self->DomainID,
        assembly.data(),
        fullyQualifiedTypeName.data(),
        method.data(),
        reinterpret_cast<void**>(&methodPtr)
    );

    if(ret != 0) methodPtr = nullptr;

    return methodPtr;
}