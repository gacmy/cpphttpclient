cmake_minimum_required(VERSION 3.4.1)
project(cppokio)
add_library(cppokio SHARED
  ./buffer.h
  ./buffer.cpp
  ./segment.h
  ./segment.cpp
  ./segmentpool.h
  ./segmentpool.cpp)

