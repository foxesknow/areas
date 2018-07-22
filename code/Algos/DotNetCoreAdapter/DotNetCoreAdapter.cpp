// DotNetCoreAdapter.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"

#include "Adapter.h"

#pragma comment(lib, "AlgosBridge.lib")

#include <DotNetCoreAdapter\DotNetHost.h>

// C:\Program Files\dotnet\shared\Microsoft.NETCore.App\2.1.2

DotNetHost *s_Host;


ADAPTER_API int32_t StartAdapterEnvironment()
{
    int32_t retCode = 0;

    try
    {
        std::string exePath = "F:\\Git\\areas\\code\\Algos\\x64\\Debug";
        std::string corePath = "C:\\Program Files\\dotnet\\shared\\Microsoft.NETCore.App\\2.1.2";

        s_Host = new DotNetHost(exePath, corePath);
        s_Host->Initialize();

        retCode = 1;
    }
    catch(...)
    {
        delete s_Host;
        retCode = 0;
    }

    return retCode;
}

ADAPTER_API void StopAdapterEnvironment()
{
    
}