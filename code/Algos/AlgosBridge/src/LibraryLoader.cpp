#include "..\WindowsIncludes.h"

#include <AlgosBridge\LibraryLoader.h>

namespace AlgosBridge
{

struct LibraryLoader::Implementation
{
    HMODULE Handle;
};

LibraryLoader::LibraryLoader(const std::string libraryFilename) : m_Self(new Implementation)
{  
    m_Self->Handle = ::LoadLibraryA(libraryFilename.data());
}

LibraryLoader::~LibraryLoader()
{
    if(m_Self->Handle != nullptr)
    {
        ::FreeLibrary(m_Self->Handle);
    }

    delete m_Self;
}

bool LibraryLoader::IsLoaded() const noexcept
{
    return m_Self->Handle != nullptr;
}

void *LibraryLoader::DoTryGetFunction(const std::string &functionName) const noexcept
{
    if(m_Self->Handle == nullptr)
    {
        return nullptr;
    }

    void *function = ::GetProcAddress(m_Self->Handle, functionName.data());
    return function;
}

} // end of namespace