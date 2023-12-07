cmake_minimum_required(VERSION 3.18)
project(PaxOS)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

if (WIN32)
    message("> Building for Windows")
elseif(APPLE)
    message("> Building for macOS")
else()
    message("> Building for Linux")
endif()

set(CMAKE_CXX_STANDARD 17)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc") # Enable exceptions for CLang
endif()


file(GLOB_RECURSE src RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} CONFIGURE_DEPENDS
    "./src/*.cpp"
    "./src/*.c"
)

file(GLOB_RECURSE ignored_src RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} CONFIGURE_DEPENDS
    "./src/interface/LovyanGFX/*.cpp"
    "./src/interface/LovyanGFX/*.c"
    "./src/lib/curl-8.5.0_1-win64-mingw/*.cpp"
    "./src/lib/curl-8.5.0_1-win64-mingw/*.c"
)

list(REMOVE_ITEM src ${ignored_src})

file(GLOB gfx_src RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} CONFIGURE_DEPENDS
    "./src/interface/LovyanGFX/LGFX_SDL.cpp"
    "./src/interface/LovyanGFX/lgfx/Fonts/efont/*.c"
    "./src/interface/LovyanGFX/lgfx/Fonts/IPA/*.c"
    "./src/interface/LovyanGFX/lgfx/utility/*.c"
    "./src/interface/LovyanGFX/lgfx/v1/*.cpp"
    "./src/interface/LovyanGFX/lgfx/v1/misc/*.cpp"
    "./src/interface/LovyanGFX/lgfx/v1/panel/Panel_Device.cpp"
    "./src/interface/LovyanGFX/lgfx/v1/panel/Panel_FrameBufferBase.cpp"
    "./src/interface/LovyanGFX/lgfx/v1/platforms/sdl/*.cpp"
    "./src/interface/LovyanGFX/LGFX_SDL.cpp"
)

add_definitions(-DLGFX_SDL)

add_executable(PaxOS ${src} ${gfx_src})

target_include_directories(PaxOS PUBLIC "src/interface/LovyanGFX")

IF (CMAKE_SYSTEM_NAME MATCHES "Linux")
    find_package(SDL2 REQUIRED SDL2)
    find_package(CURL REQUIRED) # Add this line to find cURL

    include_directories(${SDL2_INCLUDE_DIRS})
    target_link_libraries(PaxOS PUBLIC -lpthread ${SDL2_LIBRARIES} CURL::libcurl -lstdc++fs) # Link to cURL

    target_link_libraries(PaxOS PRIVATE m)
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "Windows")
    target_include_directories(PaxOS PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src/lib/SDL2-2.28.2/include")
    target_include_directories(PaxOS PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src/lib/curl-8.5.0_1-win64-mingw/include")

    target_link_libraries(PaxOS PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src/lib/SDL2-2.28.2/lib/x64/SDL2main.lib")
    target_link_libraries(PaxOS PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src/lib/SDL2-2.28.2/lib/x64/SDL2.lib")
    target_link_libraries(PaxOS PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src/lib/curl-8.5.0_1-win64-mingw/lib/libcurl.dll.a")
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "Darwin")
    find_package(SDL2 REQUIRED COMPONENTS SDL2)

    find_package(CURL REQUIRED) # Add this line to find cURL

    target_link_libraries(PaxOS PUBLIC -lpthread ${SDL2_LIBRARIES} CURL::libcurl) # Link to cURL

    target_link_libraries(PaxOS PRIVATE SDL2::SDL2)

    target_link_libraries(PaxOS PRIVATE m)
ENDIF ()

target_compile_features(PaxOS PUBLIC cxx_std_17)

if (WIN32)
    include_directories(src/lib/dirent-1.23.2/include)
endif()

file(COPY resources/storage DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if (WIN32)
    file(COPY resources/SDL2.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endif()
