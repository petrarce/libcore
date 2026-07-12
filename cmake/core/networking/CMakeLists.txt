add_library(networking SHARED
    SocSession.cpp
    SocSession.h
    SocDatagram.cpp
    SocDatagram.h
)

target_include_directories(networking PUBLIC .)

add_subdirectory(python)
