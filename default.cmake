list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
message("CMAKE_MODULE_PATH=${CMAKE_MODULE_PATH}")

include(CTest)
include(AddBoostTests)
