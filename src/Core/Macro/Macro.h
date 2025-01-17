#pragma once

#if defined(_WIN32)
    #define MXENGINE_WINDOWS
    #if defined(_WIN64)
        #define MXENGINE_WINDOWS64
    #else
        #define MXENGINE_WINDOWS32
    #endif
#elif defined(__linux__)
    #define MXENGINE_LINUX
#elif defined(__APPLE__)
    #define MXENGINE_MACOS
#endif

#if defined(NDEBUG) && !defined(_DEBUG)
    #define MXENGINE_RELEASE
#else 
    #define MXENGINE_DEBUG
#endif

#if defined(MXENGINE_WINDOWS)
    #define MXENGINE_DLL_IMPORT __declspec(dllimport)
    #define MXENGINE_DLL_EXPORT __declspec(dllexport)
#else
    #define MXENGINE_DLL_IMPORT __attribute__ ((visibility ("default")))
    #define MXENGINE_DLL_EXPORT __attribute__ ((visibility ("default")))
#endif

#if INTPTR_MAX != INT64_MAX
#error MxEngine supports only x64 builds
#endif

// assert handling 
#define MX_ASSERT_EXCEPTION

#define MXENGINE_CONCAT_IMPL(x, y) x##y
#define MXENGINE_CONCAT(x, y) MXENGINE_CONCAT_IMPL(x, y)

#define INVOKE_ONCE(...) static char MXENGINE_CONCAT(unused, __LINE__) = [&](){ __VA_ARGS__; return '\0'; }(); (void)MXENGINE_CONCAT(unused, __LINE__)

#define BOOL_STRING(b) (bool(b) ? "true" : "false")

#define MXENGINE_STRING_IMPL(x) #x
#define MXENGINE_STRING(x) MXENGINE_STRING_IMPL(x)

#define MXENGINE_MAKE_INTERNAL_TAG(tag) ("!" tag)
#define MXENGINE_INTERNAL_TAG_SYMBOL '!'

#define MXENGINE_MAKE_MOVEONLY(class_name) class_name() = default; class_name(const class_name&) = delete; class_name& operator=(const class_name&) = delete;\
        class_name(class_name&&) noexcept = default; class_name& operator=(class_name&&) noexcept = default

#if defined(MXENGINE_DEBUG)
    #if defined(MX_ASSERT_EXCEPTION)
            #include <exception>
            namespace MxEngine { class assert_exception : public std::exception { \
                public: const char* message; assert_exception(const char* msg) {}\
                        const char* what() const noexcept { return message; } }; }
        #define MX_ASSERT(expr) if(!(expr)) throw MxEngine::assert_exception(#expr) 
    #else
        #define MX_ASSERT(expr) assert(expr)
    #endif
#else
    #define MX_ASSERT(expr)
#endif

#define GENERATE_METHOD_CHECK(NAME, ...) namespace MxEngine { template<typename T> class has_method_##NAME {\
    template<typename U> constexpr static auto check(int) -> decltype(std::declval<U>().__VA_ARGS__, bool()) { return true; }\
    template<typename> constexpr static bool check(...) { return false; } public:\
    static constexpr bool value = check<T>(0); }; }

#define MXENGINE_FORCE_REFLECTION(TYPE) namespace MxEngine { \
    namespace detail {                                       \
        void MXENGINE_DLL_EXPORT DynamicInitDummy##TYPE();   \
    }                                                        \
}                                                            \
namespace {                                                  \
    struct DynamicInit##TYPE {                               \
        DynamicInit##TYPE() {                                \
            ::MxEngine::detail::DynamicInitDummy##TYPE();    \
        }                                                    \
    } dynamicInitInstance##TYPE;                             \
}

#define MXENGINE_FORCE_REFLECTION_IMPLEMENTATION(TYPE) namespace MxEngine { \
    namespace detail {                                                      \
        void MXENGINE_DLL_EXPORT DynamicInitDummy##TYPE() { }               \
    }                                                                       \
}

namespace MxEngine
{
    template<int X, int Y>
    struct AssertEquality
    {
        static_assert(X == Y, "equality check failed");
        static constexpr bool value = X == Y;
    };
}

#if !defined(MXENGINE_SHIPPING)
    #define MXENGINE_PROFILING_ENABLED
#endif