#pragma once

#ifdef DOTNETCORESUBSYSTEM_EXPORTS
    #define DOTNETCORESUBSYSTEM __declspec(dllexport)
#else
    #define DOTNETCORESUBSYSTEM __declspec(dllimport)
#endif