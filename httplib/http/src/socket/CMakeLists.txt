cmake_minimum_required(VERSION 3.4.1)
project(socket)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/)       #设置可执行文件的输出目录
#SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)   
#add_library(socket SHARED
add_executable(socket test.cpp
  ./client_socket.h
  ./client_socket.cpp
  ./inputstream.h
  ./inputstream.cpp
  ./outputstream.h
  ./outputstream.cpp
  ./real_inputstream.h
  ./real_inputstream.cpp
  ./real_outputstream.h
  ./real_outputstream.cpp
  ./socket.h
  ./socket.cpp
  )


