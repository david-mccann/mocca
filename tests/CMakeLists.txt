AddGoogleTest()

OPTION(MOCCA_TEST_TCP "Run network tests with TCP network service (slows down test execution)" OFF)

FILE(GLOB_RECURSE MOCCA_TEST_SOURCE ${CMAKE_CURRENT_LIST_DIR}/*.cpp)

ADD_EXECUTABLE(tests ${MOCCA_TEST_SOURCE}
                     $<TARGET_OBJECTS:mocca>)
TARGET_LINK_LIBRARIES(tests gtest_main)
IF (${MOCCA_TEST_TCP})
	SET_TARGET_PROPERTIES(tests PROPERTIES COMPILE_DEFINITIONS "MOCCA_TEST_TCP")
ENDIF ()

MarkAsExternal(gmock)
MarkAsExternal(gmock_main)
MarkAsExternal(gtest)
MarkAsExternal(gtest_main)