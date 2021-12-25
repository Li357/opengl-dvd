cmake_minimum_required(VERSION 3.21)
project(learn-opengl)
set(CMAKE_CXX_STANDARD 20)

add_executable(main main.cpp)

include_directories(/opt/homebrew/include include)
target_link_libraries(main "-framework OpenGL" glfw -L/opt/homebrew/lib)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/dvd.jpg ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/vertex.glsl ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/fragment.glsl ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)