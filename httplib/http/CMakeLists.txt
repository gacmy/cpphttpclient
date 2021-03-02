cmake_minimum_required(VERSION 3.4.1)
project(httpclient)
add_executable(httpclient 
  ./test.cpp
  ./dns.h
  ./dns.cpp
  ./request.h
  ./request.cpp
  ./http_parser.h
  ./http_parser.cpp
  ./httpurl.h
  ./httpurl.cpp
  ./httphead.h
  ./httphead.cpp
  ./buffersocket.h
  ./buffersocket.cpp
  ../utils/util.h
  ../utils/util.cpp
  ../cppokio/buffer.h
  ../cppokio/buffer.cpp
  ../cppokio/segment.h
  ../cppokio/segment.cpp
  ../cppokio/segmentpool.h
  ../cppokio/segmentpool.cpp

  )

