# CMakeLists.txt for Launcher executable

file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/Code/Source/*.cpp")
file(GLOB_RECURSE HEADER_FILES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/Code/Source/*.h")

add_executable(Launcher ${SOURCE_FILES} ${HEADER_FILES})

target_link_libraries(Launcher PRIVATE project_options CoreRuntime)

target_include_directories(Launcher PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Resources" "${CMAKE_CURRENT_SOURCE_DIR}/Code/Source")
set_target_properties(Launcher PROPERTIES WIN32_EXECUTABLE TRUE)

add_custom_command(TARGET Launcher POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:CoreRuntime>
        $<TARGET_FILE_DIR:Launcher>
)