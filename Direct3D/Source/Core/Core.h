#pragma once

////////////////////////////////
// DEBUG BREAK /////////////////
////////////////////////////////

#ifndef DEBUG_BREAK
    #define DEBUG_BREAK() __debugbreak()
#endif

#include "Log.h"

////////////////////////////////
// ASSERTION(S) ////////////////
////////////////////////////////

#ifndef ASSERT
    #define ASSERT(x, ...) if (x) { LOG_ERROR(__VA_ARGS__); DEBUG_BREAK(); }
#endif

#include <memory>
#include <utility>

////////////////////////////////
// SCOPE ///////////////////////
////////////////////////////////

template<typename T>
using Scope = std::unique_ptr<T>;

template<typename T, typename ... Args>
static constexpr Scope<T> CreateScope(Args&& ... args)
{
    return std::make_unique<T>(std::forward<Args>(args)...);
}

////////////////////////////////
// REF. ////////////////////////
////////////////////////////////

template<typename T>
using Ref = std::shared_ptr<T>;

template<typename T, typename ... Args>
static constexpr Ref<T> CreateRef(Args&& ... args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

template<typename T>
static constexpr bool operator==(const Ref<T>& lhs, const Ref<T>& rhs)
{
    return (*lhs) == (*rhs);
}

template<typename T>
static constexpr bool operator!=(const Ref<T>& lhs, const Ref<T>& rhs)
{
    return !(lhs == rhs);
}