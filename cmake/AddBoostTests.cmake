find_package(Boost COMPONENTS unit_test_framework REQUIRE)

function(add_boost_test)

  set(options OPTIONAL)
  set(oneValueArgs NAME)
  set(multiValueArgs SOURCES LINK DEPENDENCES)
  cmake_parse_arguments(BOOSTTEST "${options}" "${oneValueArgs}"
                        "${multiValueArgs}" "${ARGN}")

  set(TESTNAME "${BOOSTTEST_NAME}-test")

  add_executable("${TESTNAME}" "${BOOSTTEST_SOURCES}")

  target_link_libraries("${TESTNAME}" PRIVATE Boost::unit_test_framework
                                              "${BOOSTTEST_LINK}")

  if(NOT "${TESTNAME_DEPENDENCES}" STREQUAL "")
    add_dependencies("${TESTNAME}" "${TESTNAME_DEPENDENCES}")
  endif()

  add_test(NAME "${TESTNAME}" COMMAND "${TESTNAME}")

endfunction()
