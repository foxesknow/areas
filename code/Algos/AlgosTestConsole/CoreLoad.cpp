#include "stdafx.h"

#include <string>

#include <DotNetCoreSubsystem\DotNetHost.h>

DotNetHost *s_Host;

int StartAdapterEnvironment()
{
    const std::string corePath = "C:\\Program Files\\dotnet\\shared\\Microsoft.NETCore.App\\2.1.2";
    const char *exePath = "F:\\Git\\areas\\code\\Algos\\x64\\Debug";

    s_Host = new DotNetHost(exePath, corePath);
    s_Host->Initialize();

    typedef void(*Main)();

    auto main = s_Host->TryCreateDelegate<Main>("ManagedLayer", "ManagedLayer.Bootstrap", "Main");
    if(main)
    {
        main();
    }

    return 0;
}
