#ifndef ___FALCIE_CONFIG_H__
#define ___FALCIE_CONFIG_H__

//I think this version supports all the c++20 features I like
#if _MSC_VER >= 1928
#define CONSTEXPR_FUNC constexpr
#endif // _MSC_VER 

#if __clang__
#define CONSTEXPR_FUNC
#endif


#endif