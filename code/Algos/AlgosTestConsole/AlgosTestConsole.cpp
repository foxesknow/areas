// AlgosTestConsole.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#pragma comment(lib, "AlgosBridge.lib")
#pragma comment(lib, "DotNetCoreSubsystem.lib")

int StartAdapterEnvironment();
void InitializeMarketData();
void PushMarketData();

int main()
{
    InitializeMarketData();
    StartAdapterEnvironment();
    PushMarketData();

    return 0;
}

