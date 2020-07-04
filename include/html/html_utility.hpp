#ifndef HTML_UTILITY_HPP
#define HTML_UTILITY_HPP

#ifdef CPPHTML_STATIC_DEFINE
    #define CPPHTML_API
#else // CPPHTML_STATIC_DEFINE
    #ifndef CPPHTML_API
        #if defined(_MSC_VER) || (defined(__MINGW32__) && defined(__GNUC__))
            #ifdef cpphtml_EXPORTS
                #define CPPHTML_API __declspec(dllexport)
            #else // cpphtml_EXPORTS
                #define CPPHTML_API __declspec(dllimport)
            #endif // cpphtml_EXPORTS
        #elif defined(__GNUC__)
            #ifdef cpphtml_EXPORTS
                #define CPPHTML_API __attribute__((visibility("default")))
            #else // cpphtml_EXPORTS
                #define CPPHTML_API
            #endif // cpphtml_EXPORTS
        #else
            #define CPPHTML_API
        #endif
    #endif // !CPPHTML_API
#endif // CPPHTML_STATIC_DEFINE

#ifndef CPPHTML_CXX20
    #if __cplusplus > 201703L || _MSVC_LANG > 201703L
        #define CPPHTML_CXX20
    #endif
#endif // !CPPHTML_CXX20

#ifndef CPPHTML_CXX17
    #if __cplusplus >= 201703L || _MSVC_LANG >= 201703L
        #define CPPHTML_CXX17
    #endif
#endif // !CPPHTML_CXX17

#define CPPHTML_PROP_GETSET(name, type, getv, setv)                \
    constexpr type& name()& noexcept { return getv; }              \
    constexpr const type& name() const& noexcept { return getv; }  \
    constexpr type&& name()&& noexcept { return std::move(getv); } \
    void name(const type& value) noexcept { setv = value; }        \
    void name(type&& value) noexcept { setv = std::move(value); }

#define CPPHTML_PROP(name, type) CPPHTML_PROP_GETSET(name, type, m_##name, m_##name)

#endif // !HTML_UTILITY_HPP
