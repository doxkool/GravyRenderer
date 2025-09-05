#cmake_minimum_required (VERSION 2.8)

project (SoLoud)

set (CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake;${CMAKE_MODULE_PATH}")
set (CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
#INCLUDE (StandartIncludes)

include_directories (include)

#include (Configure.cmake)
#INCLUDE (InstallIncludes.cmake)
#INCLUDE (InstallStaticData.cmake)

option (SOLOUD_STATIC "Set to ON to build static SoLoud" ON)
option (SOLOUD_BACKEND_NULL "Set to ON for building NULL backend" ON)
option (SOLOUD_BUILD_DEMOS "Set to ON for building demos" OFF)

if(UNIX)
	option (SOLOUD_BACKEND_SDL2 "Set to ON for building SDL2 backend" OFF)
	option (SOLOUD_BACKEND_COREAUDIO "Set to ON for building CoreAudio backend" OFF)
	option (SOLOUD_BACKEND_OPENSLES "Set to ON for building OpenSLES backend" OFF)
	option (SOLOUD_BACKEND_XAUDIO2 "Set to ON for building XAudio2 backend" ON)
	option (SOLOUD_BACKEND_MINIAUDIO "Set to ON for building XAudio2 backend" ON)
	option (SOLOUD_BACKEND_WINMM "Set to ON for building WINMM backend" OFF)
	option (SOLOUD_BACKEND_WASAPI "Set to ON for building WASAPI backend" OFF)	
endif(UNIX)

if(WIN32)
	option (SOLOUD_BACKEND_SDL2 "Set to ON for building SDL2 backend" OFF)
	option (SOLOUD_BACKEND_COREAUDIO "Set to ON for building CoreAudio backend" OFF)
	option (SOLOUD_BACKEND_OPENSLES "Set to ON for building OpenSLES backend" OFF)
	option (SOLOUD_BACKEND_XAUDIO2 "Set to ON for building XAudio2 backend" OFF)
	option (SOLOUD_BACKEND_WINMM "Set to ON for building WINMM backend" ON)
	option (SOLOUD_BACKEND_WASAPI "Set to ON for building WASAPI backend" ON)	
endif(WIN32)

if (UNIX AND NOT WIN32 AND NOT APPLE)
	if (CMAKE_SIZEOF_VOID_P MATCHES "8")
		set (LIB_POSTFIX "64" CACHE STRING "suffix for 32/64 dir placement")
		mark_as_advanced (LIB_POSTFIX)
	endif ()
endif ()
if (NOT DEFINED LIB_POSTFIX)
	set (LIB_POSTFIX "")
endif ()

include (src.cmake)

#include (InstallExport)
