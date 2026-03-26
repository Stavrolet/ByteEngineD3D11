# CMakeLists.txt for Launcher executable

add_executable(Launcher
    "Code/Source/Main.cpp"
    "Code/Source/Win32Window.cpp"
    "Code/Source/Win32Window.h"
)

target_link_libraries(Launcher PRIVATE project_options CoreRuntime)

target_include_directories(Launcher PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Resources" "${CMAKE_CURRENT_SOURCE_DIR}/Code/Source")
set_target_properties(Launcher PROPERTIES WIN32_EXECUTABLE TRUE)

add_custom_command(TARGET Launcher POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:CoreRuntime>
        $<TARGET_FILE_DIR:Launcher>
)