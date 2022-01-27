cmake_minimum_required(VERSION 3.19)
project(LSH)

set(CMAKE_CXX_STANDARD 17)

add_executable(main main.cpp)
#add_executable(LSH LSH.cpp pri_queue.cpp)

add_executable(test test.cpp)

add_executable(LinearScan LinearScan.cpp)
add_executable(list list.cpp)

add_executable(util util.cpp)
add_executable(test_data test_data.cpp)
add_executable(p2h p2h.cpp)