find_package( absl REQUIRED)
set(CMAKE_CXX_STANDARD 20 CACHE STRING "c++ standard")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/find")


include(CTest)
include(AddBoostTests)
include(ReturnIf)
find_package(PkgConfig REQUIRED)
