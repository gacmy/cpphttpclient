cmake_minimum_required(VERSION 3.4.1)
project(gthread)
#add_library(gthread SHARED
add_library(gthread  SHARED
  ./event_thread.h
  ./event_thread.cpp
  ./task.h
  ./thread.h
  ./thread.cpp
  ./thread_notify.h
  ./thread_notify.cpp
  ./thread_pool.h
  ./thread_pool.cpp
  ./work_thread.h
  ./work_thread.cpp
  )
target_link_libraries(gthread -lpthread)

