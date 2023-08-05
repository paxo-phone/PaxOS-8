cmake_minimum_required(VERSION 3.18)
project(PaxOS8)

if(WIN32)
	message("> Building for Windows")
else()
	message("> Building for Linux")
endif()

set(CMAKE_CXX_STANDARD 17)

file(GLOB src RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} CONFIGURE_DEPENDS
	*.cpp

	interface/LovyanGFX/LGFX_SDL.cpp

 	interface/LovyanGFX/lgfx/Fonts/efont/*.c
	interface/LovyanGFX/lgfx/Fonts/IPA/*.c
	interface/LovyanGFX/lgfx/utility/*.c
	interface/LovyanGFX/lgfx/v1/*.cpp
	interface/LovyanGFX/lgfx/v1/misc/*.cpp
	interface/LovyanGFX/lgfx/v1/panel/Panel_Device.cpp
	interface/LovyanGFX/lgfx/v1/panel/Panel_FrameBufferBase.cpp
	interface/LovyanGFX/lgfx/v1/platforms/sdl/*.cpp
)

add_definitions(-DLGFX_SDL)

add_executable(PaxOS8 ${src})

target_include_directories(PaxOS8 PUBLIC "interface/LovyanGFX")

IF (CMAKE_SYSTEM_NAME MATCHES "Linux")
	find_package(SDL2 REQUIRED SDL2)
	include_directories(${SDL2_INCLUDE_DIRS})
	target_link_libraries(LGFX_SDL -lpthread ${SDL2_LIBRARIES})

	target_link_libraries(PaxOS8 PRIVATE m)
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "Windows")
	target_include_directories(PaxOS8 PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/lib/SDL2-2.28.2/include")

	target_link_libraries(PaxOS8 PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/lib/SDL2-2.28.2/lib/x64/SDL2.lib")
	target_link_libraries(PaxOS8 PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/lib/SDL2-2.28.2/lib/x64/SDL2main.lib")

	add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
	add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
	add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/Zc:__cplusplus>")
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "Darwin")
	find_package(SDL2 REQUIRED COMPONENTS SDL2)
	target_link_libraries(PaxOS8 PRIVATE SDL2::SDL2)

	target_link_libraries(PaxOS8 PRIVATE m)
ENDIF ()

target_compile_features(PaxOS8 PUBLIC cxx_std_17)

if(WIN32)
	include_directories(lib/dirent-1.23.2/include)
endif()

file(COPY resources/storage DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/storage)

if (WIN32)
	file(COPY resources/SDL2.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endif()
