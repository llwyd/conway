cmake_minimum_required(VERSION 3.2)

project( conway )

set (CMAKE_C_STANDARD 11 )
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY bin/ )
set (CMAKE_BUILD_TYPE Debug )


add_executable( conway.out
                desktop/conway.c
                life/life.c
                life/life.h
                )
target_compile_definitions(conway.out PRIVATE TARGET_DESKTOP)

include_directories( conway.out PRIVATE desktop )
include_directories( conway.out PRIVATE life )

target_compile_options( conway.out
                        PUBLIC
                        -Wfatal-errors
                        #                        -Wall
                        #-Wextra
                        #-Werror
                        #-Wpedantic
                        #-Wpointer-arith
                        -g)

target_link_libraries( conway.out X11)
target_link_libraries( conway.out m)

