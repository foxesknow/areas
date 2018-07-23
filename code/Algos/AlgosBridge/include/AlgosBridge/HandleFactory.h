#pragma once

#include <atomic>

namespace AlgosBridge
{

template<typename HANDLE>
class HandleFactory final
{
private:
    std::atomic<size_t> m_Counter;

public:
    HandleFactory() : m_Counter(1)
    {
    }

    HANDLE Allocate() noexcept
    {
        auto id = m_Counter.fetch_add(1);
        return reinterpret_cast<HANDLE>(id);
    }

    HANDLE ToHandle(size_t id) const noexcept
    {
        return reinterpret_cast<HANDLE>(id);
    }

    size_t FromHandle(HANDLE handle) const noexcept
    {
        return reinterpret_cast<size_t>(handle);
    }
};

} // end of namespace