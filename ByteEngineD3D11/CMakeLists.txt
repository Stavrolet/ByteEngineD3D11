# CMakeLists.txt for ByteEngineD3D11

if(WIN32)
	set(MANIFEST_PATH "${CMAKE_CURRENT_SOURCE_DIR}/Platform/Windows/${CMAKE_PROJECT_NAME}.manifest")
	set(RESOURCES_PATH "${CMAKE_CURRENT_SOURCE_DIR}/Platform/Windows/Resources/${CMAKE_PROJECT_NAME}.rc")

	cmake_path(CONVERT ${MANIFEST_PATH} TO_NATIVE_PATH_LIST NATIVE_MANIFEST_PATH)
	cmake_path(CONVERT ${RESOURCES_PATH} TO_NATIVE_PATH_LIST NATIVE_RESOURCES_PATH)
endif()

set(WINDOWS_LIBS dxgi.lib d3dcompiler.lib d3d11.lib $<$<CONFIG:DEBUG>:dxguid.lib>)
set(LIBS $<$<PLATFORM_ID:Windows>:${WINDOWS_LIBS}>)

file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/Code/Source/*.cpp")
file(GLOB_RECURSE HEADER_FILES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/Code/Include/*.h")

if(WIN32)
	find_package(wil CONFIG REQUIRED)
	find_package(directxtk CONFIG REQUIRED)
endif()

add_executable(ByteEngineD3D11 ${SOURCE_FILES} ${HEADER_FILES} $<$<PLATFORM_ID:Windows>:${NATIVE_MANIFEST_PATH};${NATIVE_RESOURCES_PATH}>)

target_link_libraries(ByteEngineD3D11 PRIVATE project_options ${LIBS})
target_include_directories(ByteEngineD3D11 PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Platform/Windows/Resources" "${CMAKE_CURRENT_SOURCE_DIR}/Code/Include")

if(WIN32)
	target_link_libraries(ByteEngineD3D11 PRIVATE WIL::WIL Microsoft::DirectXTK)
	target_include_directories(ByteEngineD3D11 PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Resources")
	set_target_properties(ByteEngineD3D11 PROPERTIES WIN32_EXECUTABLE TRUE)
endif()