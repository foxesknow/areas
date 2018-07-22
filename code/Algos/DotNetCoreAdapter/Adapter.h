#pragma once

#ifdef DOTNETCOREADAPTER_EXPORTS
    #define ADAPTER_API extern "C" __declspec(dllexport)
#else
    #define ADAPTER_API extern "C" __declspec(dllimport)
#endif
