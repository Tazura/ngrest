#ifndef NGRESTXMLEXPORT_H
#define NGRESTXMLEXPORT_H

#ifdef WIN32
  #ifdef _MSC_VER
    #pragma warning(disable: 4786 4251 4521)
  #endif

  #ifdef NGREST_XML_DLL_EXPORTS
    #define NGREST_XML_EXPORT __declspec(dllexport)
  #else
    #define NGREST_XML_EXPORT __declspec(dllimport)
  #endif
#else
  #define NGREST_XML_EXPORT 
#endif

#ifndef NGREST_DEPRECATED
  #ifdef _MSC_VER
    #if _MSC_FULL_VER >= 140050320
      #define NGREST_DEPRECATED(Replacement) __declspec(deprecated("You are using deprecated API. Consider using " #Replacement " instead."))
    #else
      #define NGREST_DEPRECATED(Replacement) __declspec(deprecated)
    #endif
  #elif __GNUC__ >= 3
    #define NGREST_DEPRECATED(Replacement) __attribute__ ((deprecated))
  #else
    #define NGREST_DEPRECATED(Replacement)
  #endif
#endif

#endif // NGRESTXMLEXPORT_H

