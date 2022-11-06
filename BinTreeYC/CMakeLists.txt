cmake_minimum_required(VERSION 3.23)
project(BinTreeYC)

set(CMAKE_CXX_STANDARD 23)

add_executable(BinTreeYC main.cpp BinNode.hpp BinTree.hpp Solution.cpp Solution.hpp)
