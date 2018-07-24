#include "..\WindowsIncludes.h"

#include <AlgosBridge\AlgosBridge.Subsystem.h>
#include <AlgosBridge\HandleFactory.h>
#include <AlgosBridge\LibraryLoader.h>

#include <map>
#include <mutex>
#include <memory>

AlgosBridge::HandleFactory<SUBSYSTEM_HANDLE> s_SubsystemHandles;

std::mutex s_SubsystemLock;
std::map<size_t, std::unique_ptr<AlgosBridge::LibraryLoader>> s_Subsystems;

BRIDGE_API int32_t Subsystem_Install(const char *libraryName)
{
    REQUIRE_OR_SET_ERROR_AND_RETURN(libraryName != nullptr, BRIDGE_ERROR_INVALID_PARAMETER, false);

    auto library = std::make_unique<AlgosBridge::LibraryLoader>(libraryName);
    if(library->IsLoaded() == false)
    {
        BRIDGE_SET_ERROR(BRIDGE_ERROR_LIBRARY_NOT_LOADED);
        return false;
    }

    auto handle = s_SubsystemHandles.Allocate();

    auto startAdapterEnvironment = library->TryGetFunction<START_ADAPTER_ENVIRONMENT>("StartAdapterEnvironment");
    if(startAdapterEnvironment == nullptr)
    {
        BRIDGE_SET_ERROR(BRIDGE_ERROR_FUNCTION_NOT_FOUND);
        return false;
    }

    // Store the handle before calling
    {
        std::lock_guard<std::mutex> guard(s_SubsystemLock);
        s_Subsystems.emplace(s_SubsystemHandles.FromHandle(handle), std::move(library));
    }

    auto adapterResult = startAdapterEnvironment(handle);
    if(adapterResult == 0)
    {
        BRIDGE_SET_ERROR(BRIDGE_ERROR_SUBSYSTEM_DIDNT_START);

        // Unload the library
        std::lock_guard<std::mutex> guard(s_SubsystemLock);
        s_Subsystems.erase(s_SubsystemHandles.FromHandle(handle));

        return false;
    }

    Bridge_SetLastError(BRIDGE_SUCCESS);
    return true;
}