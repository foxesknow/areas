#pragma once

#ifdef DOTNETCOREADAPTER_EXPORTS
    #define DOTNETCOREADAPTER __declspec(dllexport)
#else
    #define DOTNETCOREADAPTER __declspec(dllimport)
#endif