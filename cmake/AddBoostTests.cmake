find_package(Boost COMPONENTS unit_test_framework REQUIRE)

function(add_boost_test)

    set(options OPTIONAL)
    set(oneValueArgs NAME)
    set(multiValueArgs SOURCES LINK DEPENDENCES RESOURCES)
    cmake_parse_arguments(BOOSTTEST "${options}" "${oneValueArgs}"
                        "${multiValueArgs}" "${ARGN}")

    set(TESTNAME "${BOOSTTEST_NAME}-test")

    add_executable("${TESTNAME}" "${BOOSTTEST_SOURCES}")

    target_link_libraries("${TESTNAME}" PRIVATE Boost::unit_test_framework
                                              "${BOOSTTEST_LINK}")

    if(NOT "${BOOSTTEST_DEPENDENCES}" STREQUAL "")
        add_dependencies("${TESTNAME}" "${BOOSTTEST_DEPENDENCES}")
    endif()

    add_test(NAME "${TESTNAME}" COMMAND "${TESTNAME}")

    message("BOOSTTEST_RESOURCES=${BOOSTTEST_RESOURCES}")
    if(NOT "${BOOSTTEST_RESOURCES}" STREQUAL "")
        message("adding for BOOSTTEST_RESOURCES=${BOOSTTEST_RESOURCES}")
        set(BUPRODUCT_FILES)
        foreach(resource "${BOOSTTEST_RESOURCES}")
            add_custom_command(
                OUTPUT "${CMAKE_CURRENT_SOURCE_DIR}/${resource}"
                COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/${resource}" "${CMAKE_CURRENT_BINARY_DIR}/${resource}"
                BYPRODUCTS "${CMAKE_CURRENT_BINARY_DIR}/${resource}"
            )
            list(APPEND BUPRODUCT_FILES "${CMAKE_CURRENT_BINARY_DIR}/${resource}")

        endforeach()
        add_custom_target( ${TESTNAME}-resources DEPENDS ${BOOSTTEST_RESOURCES} )
        add_dependencies( ${TESTNAME} ${TESTNAME}-resources )
    endif()
endfunction()
