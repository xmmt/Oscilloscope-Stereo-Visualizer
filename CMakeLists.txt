cmake_minimum_required(VERSION 3.9)
project(oscilloscope)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -no-pie")

include_directories("/home/binniewinnie/imgui-sfml/")
include_directories("/home/binniewinnie/imgui/")

set(SOURCE_FILES main.cpp imgui.cpp imgui_draw.cpp imgui-SFML.cpp imgui_demo.cpp)
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} openal -lGL -lGLU -lglut -lsfml-graphics -lsfml-window
        -lsfml-system -lfreetype -lsfml-audio)