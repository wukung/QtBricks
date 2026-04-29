cmake_minimum_required(VERSION 3.16)
project(QtBricks VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

include_directories(include)

file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.h")

add_executable(QtBricks
    ${SOURCES}
    ${HEADERS}
)

target_link_libraries(QtBricks PRIVATE Qt6::Widgets)

# For macOS, configure app bundle if needed, but for a simple game executable is fine
if(APPLE)
    set_target_properties(QtBricks PROPERTIES MACOSX_BUNDLE TRUE)
endif()
