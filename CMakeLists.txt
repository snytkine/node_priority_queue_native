cmake_minimum_required(VERSION 3.6)
project(heap_lib)


set(CMAKE_CXX_STANDARD 11)
include_directories( /Users/snytkind/projects/node/deps/v8/include /usr/local/Cellar/libuv/HEAD/include /Users/snytkind/projects/node/node_modules/nan /Users/snytkind/projects/node/src  )

set(SOURCE_FILES src/mypq.cpp src/bootstrap.h src/ObjectHolder.cpp src/ObjectHolder.h)
#add_executable(heap_lib  src/main2.cpp)

add_executable("example" src/mypq.cpp src/bootstrap.h src/ObjectHolder.cpp src/ObjectHolder.h src/mypq.h)

#add_executable(heap_lib  src/main.cpp)