#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Do_Model0::Do_Model0" for configuration ""
set_property(TARGET Do_Model0::Do_Model0 APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(Do_Model0::Do_Model0 PROPERTIES
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/./Do_Model0.so"
  IMPORTED_SONAME_NOCONFIG "Do_Model0.so"
  )

list(APPEND _IMPORT_CHECK_TARGETS Do_Model0::Do_Model0 )
list(APPEND _IMPORT_CHECK_FILES_FOR_Do_Model0::Do_Model0 "${_IMPORT_PREFIX}/./Do_Model0.so" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
