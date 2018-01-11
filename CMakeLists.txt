cmake_minimum_required (VERSION 2.8)
project (kdtree)

add_executable(kdtree index.cpp)

target_link_libraries(kdtree tree point)