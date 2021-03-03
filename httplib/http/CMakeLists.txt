cmake_minimum_required(VERSION 3.4.1)
project(httpclient)
MESSAGE( STATUS "p-path=${THREAD_LIB_PATH}.")

ADD_SUBDIRECTORY(./src/cppokio)
ADD_SUBDIRECTORY(./src/thread)

add_executable(httpclient
  ./src/test.cpp
  ./src/dns.h
  ./src/dns.cpp
  ./src/request.h
  ./src/request.cpp
  ./src/http_parser.h
  ./src/http_parser.cpp
  ./src/httpurl.h
  ./src/httpurl.cpp
  ./src/httphead.h
  ./src/httphead.cpp
  ./src/buffersocket.h
  ./src/buffersocket.cpp
  ./src//utils/util.h
  ./src/utils/util.cpp

  )
target_link_libraries(httpclient cppokio gthread)

