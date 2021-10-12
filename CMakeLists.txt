cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 17)

project(3dengine)

set(BIN_DIR "${3dengine_SOURCE_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${BIN_DIR}")
set(SFML_DIR "/home/manu/SFML-2.5.1/lib/cmake/SFML")
set(IMGUI_DIR "${3dengine_SOURCE_DIR}/external/imgui")
set(IMGUI_SFML_DIR "${3dengine_SOURCE_DIR}/external/imgui-sfml")
find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)

file(GLOB_RECURSE MAIN_SOURCES RELATIVE ${3dengine_SOURCE_DIR} "src/*.cpp")
file(GLOB_RECURSE EXT_SOURCES RELATIVE ${3dengine_SOURCE_DIR} "external/*.cpp")
add_executable(3dengine ${MAIN_SOURCES} ${EXT_SOURCES})
target_include_directories(3dengine PRIVATE "include" ${IMGUI_DIR} ${IMGUI_SFML_DIR})
target_link_libraries(3dengine sfml-graphics sfml-audio GL)

install(TARGETS 3dengine RUNTIME DESTINATION ${BIN_DIR})

add_custom_target(run COMMAND 3dengine WORKING_DIRECTORY ${BIN_DIR})

find_package(Catch2 3 REQUIRED)
add_executable(tests "test/test.cpp")
target_include_directories(tests PRIVATE "include")
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)

add_custom_target(test COMMAND tests WORKING_DIRECTORY ${BIN_DIR})
