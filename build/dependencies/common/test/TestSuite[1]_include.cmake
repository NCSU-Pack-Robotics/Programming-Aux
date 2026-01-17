if(EXISTS "/home/wlfpk1/Pack-Robotics-Programming-Push-Back-Aux/build/dependencies/common/test/TestSuite")
  if(NOT EXISTS "/home/wlfpk1/Pack-Robotics-Programming-Push-Back-Aux/build/dependencies/common/test/TestSuite[1]_tests.cmake" OR
     NOT "/home/wlfpk1/Pack-Robotics-Programming-Push-Back-Aux/build/dependencies/common/test/TestSuite[1]_tests.cmake" IS_NEWER_THAN "/home/wlfpk1/Pack-Robotics-Programming-Push-Back-Aux/build/dependencies/common/test/TestSuite" OR
     NOT "/home/wlfpk1/Pack-Robotics-Programming-Push-Back-Aux/build/dependencies/common/test/TestSuite[1]_tests.cmake" IS_NEWER_THAN "${CMAKE_CURRENT_LIST_FILE}")
    include("/usr/share/cmake-3.31/Modules/GoogleTestAddTests.cmake")
    gtest_discover_tests_impl(
      TEST_EXECUTABLE [==[/home/wlfpk1/Pack-Robotics-Programming-Push-Back-Aux/build/dependencies/common/test/TestSuite]==]
      TEST_EXECUTOR [==[]==]
      TEST_WORKING_DIR [==[/home/wlfpk1/Pack-Robotics-Programming-Push-Back-Aux/build/dependencies/common/test]==]
      TEST_EXTRA_ARGS [==[]==]
      TEST_PROPERTIES [==[]==]
      TEST_PREFIX [==[]==]
      TEST_SUFFIX [==[]==]
      TEST_FILTER [==[]==]
      NO_PRETTY_TYPES [==[FALSE]==]
      NO_PRETTY_VALUES [==[FALSE]==]
      TEST_LIST [==[TestSuite_TESTS]==]
      CTEST_FILE [==[/home/wlfpk1/Pack-Robotics-Programming-Push-Back-Aux/build/dependencies/common/test/TestSuite[1]_tests.cmake]==]
      TEST_DISCOVERY_TIMEOUT [==[5]==]
      TEST_DISCOVERY_EXTRA_ARGS [==[]==]
      TEST_XML_OUTPUT_DIR [==[]==]
    )
  endif()
  include("/home/wlfpk1/Pack-Robotics-Programming-Push-Back-Aux/build/dependencies/common/test/TestSuite[1]_tests.cmake")
else()
  add_test(TestSuite_NOT_BUILT TestSuite_NOT_BUILT)
endif()
