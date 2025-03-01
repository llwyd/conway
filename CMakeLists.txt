cmake_minimum_required(VERSION 3.2)

project( conway )

set (CMAKE_C_STANDARD 11 )
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY bin/ )
set (CMAKE_BUILD_TYPE Debug )


add_executable( conway.out
                desktop/emulator.c
                life/life.c
                life/life.h
                )

add_executable( bird.out
                desktop/emulator.c
                bird/bird.c
                bird/bird.h
                common/trig.h
                common/trig.c
                common/qmath.h
                common/qmath.c
                common/lut.h
                )

add_executable( tests.out
                tests/tests.c
                tests/trig_tests.c
                tests/trig_tests.h
                tests/qmath_tests.c
                tests/qmath_tests.h
                tests/movement_tests.h
                tests/movement_tests.c
                tests/fp_tests.h
                tests/fp_tests.c
                common/trig.h
                common/trig.c
                common/qmath.h
                common/qmath.c
                common/lut.h
                Unity/src/unity.c
                Unity/src/unity.h
                Unity/src/unity_internals.h ) 

include_directories( tests.out PRIVATE Unity )
include_directories( tests.out PRIVATE Unity/src )
include_directories( tests.out PRIVATE bird )
include_directories( tests.out PRIVATE life )
include_directories( tests.out PRIVATE common )

target_compile_definitions(conway.out PRIVATE TARGET_DESKTOP)
target_compile_definitions(conway.out PRIVATE SIM_GOL)

target_compile_definitions(bird.out PRIVATE TARGET_DESKTOP)
target_compile_definitions(bird.out PRIVATE SIM_BIRD)

target_compile_definitions(tests.out PRIVATE TARGET_DESKTOP)

include_directories( conway.out PRIVATE desktop )
include_directories( conway.out PRIVATE life )

include_directories( bird.out PRIVATE desktop )
include_directories( bird.out PRIVATE life )
include_directories( bird.out PRIVATE bird )
include_directories( bird.out PRIVATE common )

target_compile_options( conway.out
                        PUBLIC
                        -Wfatal-errors
                        #                        -Wall
                        #-Wextra
                        #-Werror
                        #-Wpedantic
                        #-Wpointer-arith
                        -g)

target_compile_options( bird.out
                        PUBLIC
                        -Wfatal-errors
                        -Wall
                        -Wextra
                        -Werror
                        -Wpedantic
                        -Wpointer-arith
                        -g)

target_compile_options( tests.out
                        PUBLIC
                        -Wfatal-errors
                        #-Wall
                        #-Wextra
                        #-Werror
                        #-Wpedantic
                        #-Wpointer-arith
                        -g
                        -DUNIT_TESTS
                        -DUNITY_OUTPUT_COLOR )

target_link_libraries( conway.out X11)

target_link_libraries( bird.out X11)

