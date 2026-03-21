# CMakeLists.txt for CoreRuntime library

set(WINDOWS_LIBS dxgi.lib d3dcompiler.lib d3d11.lib $<$<CONFIG:DEBUG>:dxguid.lib>)
set(LIBS $<$<PLATFORM_ID:Windows>:${WINDOWS_LIBS}>)

file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/Code/Source/*.cpp")
file(GLOB_RECURSE HEADER_FILES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/Code/Include/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/Code/Source/*.h")

if(WIN32)
	find_package(wil CONFIG REQUIRED)
	find_package(directxtk CONFIG REQUIRED)
endif()

add_library(CoreRuntime SHARED ${SOURCE_FILES} ${HEADER_FILES})

target_link_libraries(CoreRuntime PRIVATE project_options ${LIBS})
target_include_directories(CoreRuntime PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Code/Include")
target_include_directories(CoreRuntime PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Code/Source")
target_compile_definitions(CoreRuntime PRIVATE $<$<PLATFORM_ID:Windows>:UNICODE;_UNICODE> BYTEENGINE_EXPORTS)

if(WIN32)
	target_link_libraries(CoreRuntime PRIVATE WIL::WIL Microsoft::DirectXTK)
endif()