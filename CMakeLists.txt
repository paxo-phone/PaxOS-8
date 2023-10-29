cmake_minimum_required(VERSION 3.18)
project(PaxOS8)

if (WIN32)
    message("> Building for Windows")
else()
    if (__APPLE__)
        message("> Building for macOS")
    else()
        message("> Building for Linux")
    endif()
endif()

set(CMAKE_CXX_STANDARD 17)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc") # Enable exceptions for CLang
endif()

#----------------------

add_definitions(-DLGFX_SDL)

file(GLOB srcLov RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} CONFIGURE_DEPENDS
    src/*.cpp
    src/drivers/screen/LovyanGFX/LGFX_SDL.cpp
    src/drivers/screen/LovyanGFX/lgfx/Fonts/efont/*.c
    src/drivers/screen/LovyanGFX/lgfx/Fonts/IPA/*.c
    src/drivers/screen/LovyanGFX/lgfx/utility/*.c
    src/drivers/screen/LovyanGFX/lgfx/v1/*.cpp
    src/drivers/screen/LovyanGFX/lgfx/v1/misc/*.cpp
    src/drivers/screen/LovyanGFX/lgfx/v1/panel/Panel_Device.cpp
    src/drivers/screen/LovyanGFX/lgfx/v1/panel/Panel_FrameBufferBase.cpp
    src/drivers/screen/LovyanGFX/lgfx/v1/platforms/sdl/*.cpp


    src/apps/2048/*.cpp
    src/apps/calcul/*.cpp
    src/apps/contact/*.cpp
    src/apps/hour/*.cpp
    src/apps/map/*.cpp
    src/apps/map/*.cpp
    src/apps/message/*.cpp
    src/apps/phone/*.cpp
    src/apps/settings/*.cpp
    src/apps/snake/*.cpp
    src/apps/*.cpp
    src/drivers/button/*.cpp
    src/drivers/GSM/Sim800L/*.cpp
    src/drivers/lights/*.cpp
    src/drivers/storage/storage/*.cpp
    src/lib/lua/*.cpp
    src/modules/lua/*.cpp
    src/modules/shell/*.cpp
    src/modules/tasks/*.cpp
    src/modules/web/*.cpp
    src/modules/widgets/*.cpp
    src/modules/widgets/gui/*.cpp)

add_executable(PaxOS8 main.cpp ${srcLov})
target_include_directories(PaxOS8 PUBLIC "src/drivers/screen/LovyanGFX")

#----------------------


IF (CMAKE_SYSTEM_NAME MATCHES "Linux")
    find_package(SDL2 REQUIRED SDL2)
    find_package(CURL REQUIRED) # Add this line to find cURL

    include_directories(${SDL2_INCLUDE_DIRS})
    target_link_libraries(PaxOS8 PUBLIC -lpthread ${SDL2_LIBRARIES} CURL::libcurl) # Link to cURL

    target_link_libraries(PaxOS8 PRIVATE m)
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "Windows")
    target_include_directories(PaxOS8 PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/lib/SDL2-2.28.2/include")

    target_link_libraries(PaxOS8 PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/lib/SDL2-2.28.2/lib/x64/SDL2main.lib")
    target_link_libraries(PaxOS8 PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/lib/SDL2-2.28.2/lib/x64/SDL2.lib")

    add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/Zc:__cplusplus>")
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "Darwin")
    find_package(SDL2 REQUIRED COMPONENTS SDL2)

    find_package(CURL REQUIRED) # Add this line to find cURL

    target_link_libraries(PaxOS8 PUBLIC -lpthread ${SDL2_LIBRARIES} CURL::libcurl) # Link to cURL

    target_link_libraries(PaxOS8 PRIVATE SDL2::SDL2)

    target_link_libraries(PaxOS8 PRIVATE m)
ENDIF ()

target_compile_features(PaxOS8 PUBLIC cxx_std_17)

if (WIN32)
    include_directories(lib/dirent-1.23.2/include)
endif()

file(COPY resources/storage DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if (WIN32)
    file(COPY resources/SDL2.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endif()
