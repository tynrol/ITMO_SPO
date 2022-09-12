cmake_minimum_required(VERSION 3.10)
project(spo_lab15 C)

set(THREADS_PREFER_PTHREAD_FLAG ON)
set(CMAKE_MODULE_PATH "${CMAKE_HOME_DIRECTORY}/module")
message("CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}")

find_package(Threads REQUIRED)
find_package(Protobuf REQUIRED)
find_package(PROTOBUF-C REQUIRED)
include_directories(${CMAKE_MODULE_PATH})
include_directories(${Protobuf_INCLUDE_DIRS})
include_directories(${PROTOBUF_INCLUDE_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

message("PROTOBUF_INCLUDE_DIR ${PROTOBUF_INCLUDE_DIR}")
message("Protobuf_INCLUDE_DIRS ${Protobuf_INCLUDE_DIRS}")
message("PROTOBUF_LIBRARIES ${PROTOBUF_LIBRARIES}")
message("PROTOBUF_C_LIBRARIES ${PROTOBUF_C_LIBRARIES}")



set(CMAKE_C_STANDARD 99)

message("CMAKE_SOURCE_DIR ${CMAKE_SOURCE_DIR}")
message("CMAKE_CURRENT_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}")

add_executable(
        main
        src/main.c
        src/cursor/cursor.c src/cursor/cursor.h
        src/client/client.c src/client/client.h
        src/server/server.c src/server/server.h
        src/common/net.h src/common/net.c
        src/common/net.pb-c.h src/common/net.pb-c.c
        src/client/parser.c src/client/parser.h
        src/client/lexer/lex.yy.c src/client/lexer/lex.yy.h
        src/client/lexer/y.tab.c src/client/lexer/y.tab.h
        src/common/utils.h src/common/utils.c
        src/client/lexer/lex.yy.h src/client/lexer/lex.yy.h)
target_link_libraries(spo_lab_1_5 PRIVATE Threads::Threads ${Protobuf_LIBRARIES} ${PROTOBUF_C_LIBRARIES})
