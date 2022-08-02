
//Using EXPORT_SYMBOL and LOCAL_SYMBOL instead of EXPORT and NO_EXPORT to 
//Make it harder to mistakenly type export when you don't mean to.


#if defined(_WIN32) || defined(__CYGWIN__)
  #ifdef FALPY_BUILD_DLL
	#define FALPY_EXPORT_SYMBOL __declspec(dllexport)
  #else
	#define FALPY_EXPORT_SYMBOL __declspec(dllimport) 
  #endif
  #define FALPY_LOCAL_SYMBOL
#else
  #ifndef __has_attribute
	#define __has_attribute(x) 0  // Compatibility with non-clang compilers.
  #endif

  #if (defined(__GNUC__) && (__GNUC__ >= 4)) ||\
      (defined(__clang__) && __has_attribute(visibility))

	#define FALPY_EXPORT_SYMBOL __attribute__ ((visibility ("default")))
	#define FALPY_LOCAL_SYMBOL  __attribute__ ((visibility ("hidden")))
  #else
	#define FALPY_EXPORT_SYMBOL
    #define FALPY_LOCAL_SYMBOL
  #endif
#endif