cmake_minimum_required(VERSION 3.6)
project(heap_lib)


set(CMAKE_CXX_STANDARD 11)
include_directories( /Users/snytkind/projects/node/deps/v8/include /usr/local/Cellar/libuv/HEAD/include /Users/snytkind/projects/node/node_modules/nan /Users/snytkind/projects/node/src  )

set(SOURCE_FILES src/priorityqueue_native.cpp src/bootstrap.h src/ObjectHolder.cpp src/ObjectHolder.h, src/qtest.cpp)

#add_executable(heap_lib  src/qtest.h)

add_executable("example" src/qtest.cpp)

#add_executable(heap_lib  src/main.cpp)