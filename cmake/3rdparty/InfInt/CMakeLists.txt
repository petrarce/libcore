add_library(inf-int INTERFACE)

target_include_directories(inf-int INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/InfInt)

add_library(inf::int ALIAS inf-int)

add_subdirectory(tests)
