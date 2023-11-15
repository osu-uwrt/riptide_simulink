#ifndef SIMPLE_ACTIVE__VISIBILITY_CONTROL_H_
#define SIMPLE_ACTIVE__VISIBILITY_CONTROL_H_
#if defined _WIN32 || defined __CYGWIN__
  #ifdef __GNUC__
    #define SIMPLE_ACTIVE_EXPORT __attribute__ ((dllexport))
    #define SIMPLE_ACTIVE_IMPORT __attribute__ ((dllimport))
  #else
    #define SIMPLE_ACTIVE_EXPORT __declspec(dllexport)
    #define SIMPLE_ACTIVE_IMPORT __declspec(dllimport)
  #endif
  #ifdef SIMPLE_ACTIVE_BUILDING_LIBRARY
    #define SIMPLE_ACTIVE_PUBLIC SIMPLE_ACTIVE_EXPORT
  #else
    #define SIMPLE_ACTIVE_PUBLIC SIMPLE_ACTIVE_IMPORT
  #endif
  #define SIMPLE_ACTIVE_PUBLIC_TYPE SIMPLE_ACTIVE_PUBLIC
  #define SIMPLE_ACTIVE_LOCAL
#else
  #define SIMPLE_ACTIVE_EXPORT __attribute__ ((visibility("default")))
  #define SIMPLE_ACTIVE_IMPORT
  #if __GNUC__ >= 4
    #define SIMPLE_ACTIVE_PUBLIC __attribute__ ((visibility("default")))
    #define SIMPLE_ACTIVE_LOCAL  __attribute__ ((visibility("hidden")))
  #else
    #define SIMPLE_ACTIVE_PUBLIC
    #define SIMPLE_ACTIVE_LOCAL
  #endif
  #define SIMPLE_ACTIVE_PUBLIC_TYPE
#endif
#endif  // SIMPLE_ACTIVE__VISIBILITY_CONTROL_H_
// Generated 27-Oct-2023 03:27:15
 