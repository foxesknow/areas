#pragma once

#include <AlgosBridge\AlgosBridge.h>

#include <string>
#include <exception>

namespace AlgosBridge
{

class BRIDGE_CLASS LibraryLoader final
{
private:
    struct Implementation;
    Implementation *m_Self;

    void *DoTryGetFunction(const std::string &functionName) const noexcept;

public:
    LibraryLoader(const std::string libraryFilename);
    ~LibraryLoader();

    bool IsLoaded() const noexcept;

    template<typename T>
    T TryGetFunction(const std::string &functionName)
    {
         auto function = DoTryGetFunction(functionName);
         return reinterpret_cast<T>(function);
    }

    template<typename T>
    T GetFunction(const std::string &functionName)
    {
         auto function = DoTryGetFunction(functionName);
         if(function == nullptr) throw std::exception("could not get function");

         return reinterpret_cast<T>(function);
    }
};

} // end of namespace