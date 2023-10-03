#ifndef ___FALCIE_CONFIG_H__
#define ___FALCIE_CONFIG_H__

// May be able to remove this constexpr shizz
// I think this version supports all the c++20 features I like

/*
#if _MSC_VER >= 1928
#define CONSTEXPR_FUNC constexpr
#endif // _MSC_VER

#if __clang__ //Due to Android iirc
#define CONSTEXPR_FUNC
#endif
*/

#ifdef _MSC_VER
#define REGION(x) _Pragma("region " #x)
#define END_REGION _Pragma("endregion")
// #define END_REGION(x) _Pragma("endregion " #x)
#else
#define REGION(x)
#define END_REGION
#endif

#if __cpp_concepts >= 201907
#define REQUIRES(x) requires x
#define CONSTRAINT(x) x
#else
#define REQUIRES(x)
#define CONSTRAINT(x) typename
#endif  // __cpp_concepts (lower part)

// Using EXPORT and LOCAL instead of EXPORT and NO_EXPORT to
// Make it harder to mistakenly type export when you don't mean to.

#if defined(_WIN32) || defined(__CYGWIN__)
#ifdef FALCIE_BUILD_DLL
#define FALCIE_EXPORT __declspec(dllexport)
// #define FALCIE_EXPORT_TEMPLATE // Will this cause issues in other dll's?
#else
#define FALCIE_EXPORT __declspec(dllimport)
// #define FALCIE_EXPORT_TEMPLATE extern //Will this cause issues in other dll's?
#endif
#define FALCIE_LOCAL
#else
#ifndef __has_attribute
#define __has_attribute(x) 0  // Compatibility with non-clang compilers.
#endif

#if (defined(__GNUC__) && (__GNUC__ >= 4)) || \
    (defined(__clang__) && __has_attribute(visibility))

#define FALCIE_EXPORT __attribute__((visibility("default")))
#define FALCIE_LOCAL __attribute__((visibility("hidden")))
// #define FALCIE_EXPORT_TEMPLATE  // Will this cause issues in other dll's?
#else
#define FALCIE_EXPORT
#define FALCIE_LOCAL
// #define FALCIE_EXPORT_TEMPLATE  // Will this cause issues in other dll's?
#endif
#endif



// TODO: Just fornow, holdover from legacy code
#if _MSC_VER >= 1928
#define CONSTEXPR_FUNC constexpr
#endif  // _MSC_VER

#if __clang__
#define CONSTEXPR_FUNC
#endif



#endif