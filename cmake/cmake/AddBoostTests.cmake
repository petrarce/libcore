find_package( Boost COMPONENTS unit_test_framework REQUIRED )

# Creates an executable and links it to the boost::unit_test_framework
# parameters:
# - NAME - test name
# - SOURCES - list of source files
# - LINK list of targets to which test executable should be linked
# - DEPENDENCES - dependence targets.
# - RESOURCES - list of files, that should be copied along the test directory
function( add_executable_test )
    set(options OPTIONAL)
    set(oneValueArgs NAME)
    set(multiValueArgs SOURCES LINK DEPENDENCES RESOURCES)
    cmake_parse_arguments(BOOSTTEST "${options}" "${oneValueArgs}"
                        "${multiValueArgs}" "${ARGN}")

    set(TESTNAME "${BOOSTTEST_NAME}")

    add_executable("${TESTNAME}" "${BOOSTTEST_SOURCES}")
    target_compile_options("${TESTNAME}" PRIVATE -g -O0)

    target_link_libraries("${TESTNAME}" PRIVATE "${BOOSTTEST_LINK}")

    if(NOT "${BOOSTTEST_DEPENDENCES}" STREQUAL "")
        add_dependencies("${TESTNAME}" "${BOOSTTEST_DEPENDENCES}")
    endif()

    if(NOT "${BOOSTTEST_RESOURCES}" STREQUAL "")
        set(OUTPUT_RESOURCES)
        foreach(resource ${BOOSTTEST_RESOURCES})
            set(SOURCE_RES "${CMAKE_CURRENT_SOURCE_DIR}/${resource}")
            set(DEST_RES "${CMAKE_CURRENT_BINARY_DIR}/${resource}")
            add_custom_command(
                OUTPUT "${DEST_RES}"
                DEPENDS "${SOURCE_RES}"
                COMMAND ${CMAKE_COMMAND} -E copy "${SOURCE_RES}" "${DEST_RES}"
            )
            list(APPEND OUTPUT_RESOURCES "${DEST_RES}")

        endforeach()
        add_custom_target( ${TESTNAME}-resources DEPENDS ${OUTPUT_RESOURCES} )
        add_dependencies( ${TESTNAME} ${TESTNAME}-resources )
    endif()

endfunction()

# Wrapper over the add_executable_test
# which adds a link dependence to Boost::unit_test_framework
function(add_boost_test)
    set(options OPTIONAL)
    set(oneValueArgs NAME)
    set(multiValueArgs SOURCES LINK DEPENDENCES RESOURCES)
    cmake_parse_arguments(BOOSTTEST "${options}" "${oneValueArgs}"
                        "${multiValueArgs}" "${ARGN}")

    add_executable_test(NAME ${BOOSTTEST_NAME}
        SOURCES ${BOOSTTEST_SOURCES}
        LINK ${BOOSTTEST_LINK} Boost::unit_test_framework
        DEPENDENCES ${BOOSTTEST_DEPENDENCES}
        RESOURCES ${BOOSTTEST_RESOURCES}
    )

    add_test(NAME "${BOOSTTEST_NAME}" COMMAND "${BOOSTTEST_NAME}")
endfunction()
