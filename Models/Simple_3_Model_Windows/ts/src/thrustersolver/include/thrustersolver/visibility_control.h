#ifndef THRUSTERSOLVER__VISIBILITY_CONTROL_H_
#define THRUSTERSOLVER__VISIBILITY_CONTROL_H_
#if defined _WIN32 || defined __CYGWIN__
  #ifdef __GNUC__
    #define THRUSTERSOLVER_EXPORT __attribute__ ((dllexport))
    #define THRUSTERSOLVER_IMPORT __attribute__ ((dllimport))
  #else
    #define THRUSTERSOLVER_EXPORT __declspec(dllexport)
    #define THRUSTERSOLVER_IMPORT __declspec(dllimport)
  #endif
  #ifdef THRUSTERSOLVER_BUILDING_LIBRARY
    #define THRUSTERSOLVER_PUBLIC THRUSTERSOLVER_EXPORT
  #else
    #define THRUSTERSOLVER_PUBLIC THRUSTERSOLVER_IMPORT
  #endif
  #define THRUSTERSOLVER_PUBLIC_TYPE THRUSTERSOLVER_PUBLIC
  #define THRUSTERSOLVER_LOCAL
#else
  #define THRUSTERSOLVER_EXPORT __attribute__ ((visibility("default")))
  #define THRUSTERSOLVER_IMPORT
  #if __GNUC__ >= 4
    #define THRUSTERSOLVER_PUBLIC __attribute__ ((visibility("default")))
    #define THRUSTERSOLVER_LOCAL  __attribute__ ((visibility("hidden")))
  #else
    #define THRUSTERSOLVER_PUBLIC
    #define THRUSTERSOLVER_LOCAL
  #endif
  #define THRUSTERSOLVER_PUBLIC_TYPE
#endif
#endif  // THRUSTERSOLVER__VISIBILITY_CONTROL_H_
// Generated 18-Oct-2023 23:23:31
 