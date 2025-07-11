find_package( absl REQUIRED)
set(CMAKE_CXX_STANDARD 20 CACHE STRING "c++ standard")

# Ensure asserts are enabled in all build types
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -UNDEBUG")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -UNDEBUG")
set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL} -UNDEBUG")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/find")


include(CTest)
include(AddBoostTests)
include(ReturnIf)
find_package(PkgConfig REQUIRED)
