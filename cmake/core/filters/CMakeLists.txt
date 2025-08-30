add_library( filters
    soundex.cpp
    soundex.h
)
add_library( libcore::filters ALIAS filters )
