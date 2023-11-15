#ifndef FF_CONTROLLER__VISIBILITY_CONTROL_H_
#define FF_CONTROLLER__VISIBILITY_CONTROL_H_
#if defined _WIN32 || defined __CYGWIN__
  #ifdef __GNUC__
    #define FF_CONTROLLER_EXPORT __attribute__ ((dllexport))
    #define FF_CONTROLLER_IMPORT __attribute__ ((dllimport))
  #else
    #define FF_CONTROLLER_EXPORT __declspec(dllexport)
    #define FF_CONTROLLER_IMPORT __declspec(dllimport)
  #endif
  #ifdef FF_CONTROLLER_BUILDING_LIBRARY
    #define FF_CONTROLLER_PUBLIC FF_CONTROLLER_EXPORT
  #else
    #define FF_CONTROLLER_PUBLIC FF_CONTROLLER_IMPORT
  #endif
  #define FF_CONTROLLER_PUBLIC_TYPE FF_CONTROLLER_PUBLIC
  #define FF_CONTROLLER_LOCAL
#else
  #define FF_CONTROLLER_EXPORT __attribute__ ((visibility("default")))
  #define FF_CONTROLLER_IMPORT
  #if __GNUC__ >= 4
    #define FF_CONTROLLER_PUBLIC __attribute__ ((visibility("default")))
    #define FF_CONTROLLER_LOCAL  __attribute__ ((visibility("hidden")))
  #else
    #define FF_CONTROLLER_PUBLIC
    #define FF_CONTROLLER_LOCAL
  #endif
  #define FF_CONTROLLER_PUBLIC_TYPE
#endif
#endif  // FF_CONTROLLER__VISIBILITY_CONTROL_H_
// Generated 21-Oct-2023 17:57:01
 