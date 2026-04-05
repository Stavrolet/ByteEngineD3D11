# CMakeLists.txt for CoreRuntime library

set(WINDOWS_LIBS dxgi.lib d3dcompiler.lib d3d11.lib $<$<CONFIG:DEBUG>:dxguid.lib>)
set(LIBS $<$<PLATFORM_ID:Windows>:${WINDOWS_LIBS}>)

if(WIN32)
	find_package(wil CONFIG REQUIRED)
	find_package(directxtk CONFIG REQUIRED)
endif()

add_library(CoreRuntime SHARED 
	"Code/Include/ByteEngine/Core/Base/Application.h"
	"Code/Include/ByteEngine/Core/Base/MainWindow.h"
	"Code/Include/ByteEngine/Core/Base/Singleton.h"
	"Code/Include/ByteEngine/Core/EventSystem/Delegate.h"
	"Code/Include/ByteEngine/Core/EventSystem/MulticastDelegate.h"
	"Code/Include/ByteEngine/Core/Input/Input.h"
	"Code/Include/ByteEngine/Core/Input/KeyCode.h"
	"Code/Include/ByteEngine/Core/Renderer/RenderContext.h"
	"Code/Include/ByteEngine/Detail/Core/EventSystem/Subscriptions.h"
	
	"Code/Include/ByteEngine/Math/Math.h"
	"Code/Include/ByteEngine/Math/Quaternion.h"
	"Code/Include/ByteEngine/Math/Vector2.h"
	"Code/Include/ByteEngine/Math/Vector3.h"
	"Code/Include/ByteEngine/Math/Vector4.h"
	"Code/Include/ByteEngine/Utilities/BitFlagsHelper.h"
	"Code/Include/ByteEngine/Utilities/EnumFlagsOperators.h"
	"Code/Include/ByteEngine/WinApiExcludingDefs/LeanAndMean.h"
	"Code/Include/ByteEngine/WinApiExcludingDefs/NoAll.h"
	"Code/Include/ByteEngine/WinApiExcludingDefs/NoGdi.h"
	"Code/Include/ByteEngine/WinApiExcludingDefs/NoInput.h"
	"Code/Include/ByteEngine/WinApiExcludingDefs/NoIO.h"
	"Code/Include/ByteEngine/WinApiExcludingDefs/NoLocalization.h"
	"Code/Include/ByteEngine/WinApiExcludingDefs/NoSys.h"
	"Code/Include/ByteEngine/WinApiExcludingDefs/NoUiAndWindows.h"
	"Code/Include/ByteEngine/CoreDefs.h"
	"Code/Include/ByteEngine/DebugLogHelper.h"
	"Code/Include/ByteEngine/GameTime.h"
	"Code/Include/ByteEngine/Primitives.h"
	"Code/Source/Core/Base/Application.cpp"
	"Code/Source/Core/Input/Input.cpp"
	"Code/Source/Core/Renderer/RenderContext.cpp"
	"Code/Source/Math/Math.cpp"
	"Code/Source/Math/Quaternion.cpp"
	"Code/Source/DebugLogHelper.cpp"
 "Code/Include/ByteEngine/Math/Matrix4x4F.h" "Code/Source/Math/Matrix4x4F.cpp" "Code/Source/Core/Graphics/GraphicsApi.h" "Code/Include/ByteEngine/Math/Rotation.h")

target_link_libraries(CoreRuntime PRIVATE project_options ${LIBS})
target_include_directories(CoreRuntime PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Code/Include")
target_include_directories(CoreRuntime PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Code/Source")
target_compile_definitions(CoreRuntime PRIVATE $<$<PLATFORM_ID:Windows>:UNICODE;_UNICODE> BYTEENGINE_EXPORTS)

if(WIN32)
	target_link_libraries(CoreRuntime PRIVATE WIL::WIL Microsoft::DirectXTK)
endif()