// DotNetCoreSubsystem.cpp : Defines the exported functions for the DLL application.
//

#include "..\stdafx.h"


#pragma comment(lib, "AlgosBridge.lib")

#include <DotNetCoreSubsystem\DotNetHost.h>
#include <AlgosBridge\AlgosBridge.Subsystem.h>

// C:\Program Files\dotnet\shared\Microsoft.NETCore.App\2.1.2

DotNetHost *s_Host;


SUBSYSTEM_API int32_t StartAdapterEnvironment(SUBSYSTEM_HANDLE subsystemHandle)
{
    int32_t retCode = 0;

    try
    {
        std::string exePath = "F:\\Git\\areas\\code\\Algos\\x64\\Debug";
        std::string corePath = "C:\\Program Files\\dotnet\\shared\\Microsoft.NETCore.App\\2.1.2";

        s_Host = new DotNetHost(exePath, corePath);
        s_Host->Initialize();

        // TODO: Read this from some sort of config
        std::string libraryName = "ManagedLayer";
        std::string fullyQualifiedTypeName = "ManagedLayer.Bootstrap";
        std::string method = "SubsystemMain";

        typedef void(*Main)(void*);
        auto main = s_Host->TryCreateDelegate<Main>(libraryName, fullyQualifiedTypeName, method);
        main(subsystemHandle);

        retCode = 1;
    }
    catch(...)
    {
        delete s_Host;
        retCode = 0;
    }

    return retCode;
}

SUBSYSTEM_API void StopAdapterEnvironment()
{
    
}