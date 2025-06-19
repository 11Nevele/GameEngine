#pragma once
#include "Event\AllowToken.h"
// Remove this include to avoid circular dependency
// #include "World.hpp"
namespace ac
{
    // Forward declaration instead of including
    class World;

    template<class T>
    struct OnAdded
    {
        Entity ID;
        const T& component;
    };

    template<class T>
    class AllowToken<OnAdded<T>>
    {
    private:
        AllowToken() = default;
        AllowToken(const AllowToken&) = delete;
        AllowToken(AllowToken&&) = delete;
        AllowToken& operator=(const AllowToken&) = delete;
        AllowToken& operator=(AllowToken&&) = delete;
        friend class World;
    };

    template<class T>
    struct OnDeleted
    {
        Entity ID;
        const T& component;
    };

    template<class T>
    class AllowToken<OnDeleted<T>>
    {
    private:
        AllowToken() = default;
        AllowToken(const AllowToken&) = delete;
        AllowToken(AllowToken&&) = delete;
        AllowToken& operator=(const AllowToken&) = delete;
        AllowToken& operator=(AllowToken&&) = delete;
        friend class World;
    };
}