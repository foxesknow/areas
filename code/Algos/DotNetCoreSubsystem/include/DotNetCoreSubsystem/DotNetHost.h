#pragma once

#include <string>
#include <memory>

#include <DotNetCoreSubsystem\coreclrhost.h>
#include <DotNetCoreSubsystem\DotNetCoreSubsystem.h>

#include <AlgosBridge\LibraryLoader.h>

class DOTNETCORESUBSYSTEM DotNetHost final
{
private:
    struct Implementation;
    Implementation *m_Self;

    

    void *DoTryCreateDelegate(const std::string &assembly, const std::string &fullyQualifiedTypeName, const std::string &method);

public:
    DotNetHost(const std::string &exePath, const std::string &corePath);
    ~DotNetHost();

    DotNetHost(DotNetHost&&) = delete;
    DotNetHost &operator=(DotNetHost&&) = delete;

    void Initialize();

    template<typename T>
    T TryCreateDelegate(const std::string &assembly, const std::string &fullyQualifiedTypeName, const std::string &method)
    {
        auto delegate = DoTryCreateDelegate(assembly, fullyQualifiedTypeName, method);
        return reinterpret_cast<T>(delegate);
    }
};