add_library(glad SHARED
    glad.c
    glad.h
    khrplatform.h
)

target_include_directories(glad PUBLIC ../)

add_library(glad::glad ALIAS glad)
