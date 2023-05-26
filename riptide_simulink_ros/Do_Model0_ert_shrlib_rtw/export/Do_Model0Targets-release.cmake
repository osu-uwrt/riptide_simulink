#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Do_Model0::Do_Model0" for configuration "Release"
set_property(TARGET Do_Model0::Do_Model0 APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(Do_Model0::Do_Model0 PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/./Do_Model0.so"
  IMPORTED_SONAME_RELEASE "Do_Model0.so"
  )

list(APPEND _IMPORT_CHECK_TARGETS Do_Model0::Do_Model0 )
list(APPEND _IMPORT_CHECK_FILES_FOR_Do_Model0::Do_Model0 "${_IMPORT_PREFIX}/./Do_Model0.so" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
