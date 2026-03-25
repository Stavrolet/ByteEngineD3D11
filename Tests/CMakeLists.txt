# CMakeLists.txt for tests

file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

find_package(GTest REQUIRED)

add_executable(Tests ${SOURCE_FILES})
target_link_libraries(Tests PRIVATE GTest::gtest GTest::gtest_main CoreRuntime)

add_custom_command(TARGET Tests POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:CoreRuntime>
        $<TARGET_FILE_DIR:Tests>
)

include(GoogleTest)
gtest_discover_tests(Tests)