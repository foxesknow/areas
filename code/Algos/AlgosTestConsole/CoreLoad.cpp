#include "stdafx.h"

#include <string>

#include <AlgosBridge\AlgosBridge.Subsystem.h>

int StartAdapterEnvironment()
{
    Subsystem_Install("DotNetCoreSubsystem.dll");
    return 1;
}
