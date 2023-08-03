cmake_minimum_required(VERSION 3.18)
project(PaxOS8)

if(WIN32)
	message("> Building for Windows")
else()
	message("> Building for Linux")
endif()

set(CMAKE_CXX_STANDARD 17)

if(WIN32)
	set(SDL2_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/win32/SDL2-2.28.2/cmake)
	set(SDL2_image_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/win32/SDL2_image-2.6.3/cmake)
else()
	set(SDL2_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/linux/SDL2-2.28.2/cmake)
	set(SDL2_image_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/linux/SDL2_image-2.6.3/cmake)
endif()

find_package(SDL2 REQUIRED CONFIG REQUIRED COMPONENTS SDL2)
find_package(SDL2 REQUIRED CONFIG REQUIRED COMPONENTS SDL2main)
find_package(SDL2_image REQUIRED CONFIG REQUIRED COMPONENTS SDL2_image)

set(LovyanGFX_include
	"interface/LovyanGFX"
	"interface/LovyanGFX/lgfx/Fonts/efont"
	"interface/LovyanGFX/lgfx/v1"
)

file(GLOB LovyanGFX_src
#	"interface/LovyanGFX/lgfx/Fonts/efont/*.c"
#	"interface/LovyanGFX/lgfx/Fonts/IPA/*.c"
	"interface/LovyanGFX/lgfx/utility/*.c"
	"interface/LovyanGFX/lgfx/v1/*.cpp"
	"interface/LovyanGFX/lgfx/v1/misc/*.cpp"
	"interface/LovyanGFX/lgfx/v1/panel/Panel_Device.cpp"
	"interface/LovyanGFX/lgfx/v1/platforms/sdl/*.cpp"
)

add_definitions(-DLGFX_SDL)

if(WIN32)
	add_executable(PaxOS8 WIN32 main.cpp ${LovyanGFX_src})
else()
	add_executable(PaxOS8 main.cpp ${LovyanGFX_src})
endif()

target_include_directories(PaxOS8 PRIVATE ${LovyanGFX_include})

target_link_libraries(PaxOS8 PRIVATE m)
if (TARGET SDL2::SDL2main)
	target_link_libraries(PaxOS8 PRIVATE SDL2::SDL2main)
endif()

if (WIN32)
	target_link_libraries(PaxOS8 PRIVATE SDL2::SDL2)
	target_link_libraries(PaxOS8 PRIVATE SDL2_image::SDL2_image)
else()
	target_link_libraries(PaxOS8 PRIVATE SDL2)
	target_link_libraries(PaxOS8 PRIVATE SDL2_image)
endif()

target_link_directories(PaxOS8 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/interface/LovyanGFX/lgfx)

if(WIN32)
	include_directories(lib/dirent-1.23.2/include)
endif()





