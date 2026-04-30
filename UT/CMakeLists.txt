# Find the testing framework
find_package(Qt6 REQUIRED COMPONENTS Test)

# Macro to add tests easily
macro(add_qt_test test_name source_file)
    add_executable(${test_name} ${source_file})
    target_link_libraries(${test_name} PRIVATE Qt6::Test QtBricksCore)
    target_include_directories(${test_name} PRIVATE ../include)
    add_test(NAME ${test_name} COMMAND ${test_name})
endmacro()

# Add test executables
add_qt_test(tst_ball tst_ball.cpp)
add_qt_test(tst_paddle tst_paddle.cpp)
add_qt_test(tst_brick tst_brick.cpp)
