cmake_minimum_required(VERSION 3.16)
project(QtBricks VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Test)

include_directories(include)

file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.h")

# Exclude main.cpp from the core library
list(REMOVE_ITEM SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")

# Create a static library for the core game logic
add_library(QtBricksCore STATIC
    ${SOURCES}
    ${HEADERS}
)
target_link_libraries(QtBricksCore PUBLIC Qt6::Widgets)

# Create the main executable
add_executable(QtBricks src/main.cpp)
target_link_libraries(QtBricks PRIVATE QtBricksCore)

# For macOS, configure app bundle if needed
if(APPLE)
    set_target_properties(QtBricks PROPERTIES MACOSX_BUNDLE TRUE)
endif()

# Enable testing
enable_testing()
add_subdirectory(UT)
