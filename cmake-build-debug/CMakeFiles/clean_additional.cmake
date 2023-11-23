# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/PassKeep_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/PassKeep_autogen.dir/ParseCache.txt"
  "PassKeep_autogen"
  )
endif()
