cmake_minimum_required(VERSION 3.4.1)
project(testsocket)
set(INC_DIR ../)  
set(LINK_DIR ../lib)
include_directories(${INC_DIR})     # 用${}引用变量
link_directories(${LINK_DIR})

add_executable(testsocket 
 ./test.cpp 
)
target_link_libraries(testsocket socket)

