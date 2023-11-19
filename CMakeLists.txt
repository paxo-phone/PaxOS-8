cmake_minimum_required(VERSION 3.18)
project(PaxOS8)

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

file(GLOB src RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} CONFIGURE_DEPENDS
        src/*.cpp
        src/lib/lua/*.c
        src/interface/LovyanGFX/LGFX_SDL.cpp
        src/interface/LovyanGFX/lgfx/Fonts/efont/*.c
        src/interface/LovyanGFX/lgfx/Fonts/IPA/*.c
        src/interface/LovyanGFX/lgfx/utility/*.c
        src/interface/LovyanGFX/lgfx/v1/*.cpp
        src/interface/LovyanGFX/lgfx/v1/misc/*.cpp
        src/interface/LovyanGFX/lgfx/v1/panel/Panel_Device.cpp
        src/interface/LovyanGFX/lgfx/v1/panel/Panel_FrameBufferBase.cpp
        src/interface/LovyanGFX/lgfx/v1/platforms/sdl/*.cpp
        src/interface/LovyanGFX/LGFX_SDL.cpp

        ./src/tasks/thread.cpp
        ./src/tasks/tasks.cpp
        ./src/widgets/touchManager.cpp
        ./src/widgets/gui/keyboard.cpp
        ./src/widgets/gui/canvas.cpp
        ./src/widgets/gui/image.cpp
        ./src/widgets/gui/box.cpp
        ./src/widgets/gui/window.cpp
        ./src/widgets/gui/label.cpp
        ./src/widgets/gui/label2.cpp
        ./src/widgets/gui/button.cpp
        ./src/widgets/gui/back.cpp
        ./src/widgets/gui/iframe/Neper/src/neper.cpp
        ./src/widgets/gui/iframe/Neper/src/dom/src/attributeList.cpp
        ./src/widgets/gui/iframe/Neper/src/dom/src/node.cpp
        ./src/widgets/gui/iframe/Neper/src/dom/src/styleList.cpp
        ./src/widgets/gui/iframe/Neper/src/dom/src/style.cpp
        ./src/widgets/gui/iframe/Neper/src/dom/src/attribute.cpp
        ./src/widgets/gui/iframe/Neper/src/html/src/parser.cpp
        ./src/widgets/gui/iframe/Neper/src/utils/src/unordered_map.cpp
        ./src/widgets/gui/iframe/Neper/src/utils/src/math.cpp
        ./src/widgets/gui/iframe/Neper/src/utils/src/string.cpp
        ./src/widgets/gui.cpp
        ./src/widgets/color.cpp
        ./src/app/settings/settings.cpp
        ./src/app/2048/2048.cpp
        ./src/app/message/message.cpp
        ./src/app/snake/snake.cpp
        ./src/app/launcher.cpp
        ./src/app/app.cpp
        ./src/app/phone/phone.cpp
        ./src/app/calcul/calcul.cpp
        ./src/app/contact/contact.cpp
        #./src/app/minecraft/SimplexNoise.cpp
        ./src/interface/screen.cpp
        ./src/interface/GSM/Sim800L/encoder.cpp
        ./src/interface/GSM/Sim800L/sim.cpp
        ./src/interface/filetree.cpp
        ./src/interface/lights.cpp
        ./src/interface/random.cpp
        ./src/interface/time.cpp
        ./src/interface/FileStream.cpp
        ./src/interface/button/button.cpp
        ./src/interface/interface.cpp
        ./src/interface/shell.cpp
        ./src/interface/random.cpp
        ./src/interface/time.cpp
        ./src/lua/lua.cpp
        ./src/main.cpp
        ./src/web/web.cpp
)

file(GLOB_RECURSE nepersrc ./src/widgets/gui/iframe/Neper/*.cpp)

add_definitions(-DLGFX_SDL)
#add_definitions(-DBUILD_EMU) // pour bien afficher les erreurs sur arduino au niveau de build_emu/paxo

add_executable(PaxOS8 ${src} ${nepersrc})

target_include_directories(PaxOS8 PUBLIC "src/interface/LovyanGFX")

IF (CMAKE_SYSTEM_NAME MATCHES "Linux")
    find_package(SDL2 REQUIRED SDL2)
    find_package(CURL REQUIRED) # Add this line to find cURL

    include_directories(${SDL2_INCLUDE_DIRS})
    target_link_libraries(PaxOS8 PUBLIC -lpthread ${SDL2_LIBRARIES} CURL::libcurl) # Link to cURL

    target_link_libraries(PaxOS8 PRIVATE m)
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "Windows")
    target_include_directories(PaxOS8 PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src/lib/SDL2-2.28.2/include")

    target_link_libraries(PaxOS8 PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src/lib/SDL2-2.28.2/lib/x64/SDL2.lib")
    target_link_libraries(PaxOS8 PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src/lib/SDL2-2.28.2/lib/x64/SDL2main.lib")

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
    include_directories(src/lib/dirent-1.23.2/include)
endif()

file(COPY resources/storage DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if (WIN32)
    file(COPY resources/SDL2.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endif()
