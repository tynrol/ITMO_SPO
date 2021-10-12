cmake_minimum_required(VERSION 3.20)
project(spo_lab3 C)

set(CMAKE_C_STANDARD 11)
set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(Threads REQUIRED)
find_package(Curses REQUIRED)

include_directories(${CURSES_INCLUDE_DIR})

add_executable(main main.c server.c server.h utils.c utils.h client.c client.h ui.c ui.h)
#target_compile_features(main -)

target_link_libraries(main PRIVATE ${CURSES_LIBRARIES} Threads::Threads)
