cmake_minimum_required(VERSION 3.5)

project(ez-imgui-button)

set(BIN_DIR "${ez-imgui-button_SOURCE_DIR}/bin")

set(SFML_DIR "/home/manu/SFML-2.5.1/lib/cmake/SFML")

set(IMGUI_DIR "${ez-imgui-button_SOURCE_DIR}/external/imgui")

set(IMGUI_SFML_DIR "${ez-imgui-button_SOURCE_DIR}/external/imgui-sfml")

find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)

file(GLOB_RECURSE SOURCES RELATIVE ${ez-imgui-button_SOURCE_DIR} "src/*.cpp")

file(GLOB_RECURSE EXT_SOURCES RELATIVE ${ez-imgui-button_SOURCE_DIR} "external/*.cpp")

add_executable(ez-imgui-button ${SOURCES} ${EXT_SOURCES})

target_include_directories(ez-imgui-button PRIVATE "include" ${IMGUI_DIR} ${IMGUI_SFML_DIR})

target_link_libraries(ez-imgui-button sfml-graphics sfml-audio GL)

install(TARGETS ez-imgui-button RUNTIME DESTINATION ${BIN_DIR})
