cmake_minimum_required(VERSION 3.5)

project( conway )

set (CMAKE_C_STANDARD 11 )
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY bin/ )
set (CMAKE_BUILD_TYPE Debug )

if(TARGET_MAC)
    link_directories(/opt/X11/lib)
endif()

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
                common/lut.c
                )

add_executable( gerono.out
                desktop/emulator.c
                gerono/gerono.c
                gerono/gerono.h
                common/trig.h
                common/trig.c
                common/qmath.h
                common/qmath.c
                common/lut.h
                common/lut.c
                )

add_executable( heart.out
                desktop/emulator.c
                heart/heart.c
                heart/heart.h
                common/trig.h
                common/trig.c
                common/qmath.h
                common/qmath.c
                common/lut.h
                common/lut.c
                )

if(UNIT_TESTS)
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
                common/lut.c
                Unity/src/unity.c
                Unity/src/unity.h
                Unity/src/unity_internals.h ) 

include_directories( tests.out PRIVATE Unity )
include_directories( tests.out PRIVATE Unity/src )
include_directories( tests.out PRIVATE bird )
include_directories( tests.out PRIVATE life )
include_directories( tests.out PRIVATE common )

target_compile_definitions(tests.out PRIVATE TARGET_DESKTOP)

target_compile_options( tests.out
                        PUBLIC
                        -O0
                        -Wfatal-errors
			#-Wall
			#-Wextra
			#-Werror
			#-Wpedantic
			#-Wpointer-arith
			#-Wconversion
                        -g
                        -DUNIT_TESTS
                        -DUNITY_OUTPUT_COLOR )

endif()

target_compile_definitions(conway.out PRIVATE TARGET_DESKTOP)
target_compile_definitions(conway.out PRIVATE SIM_GOL)

target_compile_definitions(bird.out PRIVATE TARGET_DESKTOP)
target_compile_definitions(bird.out PRIVATE SIM_BIRD)

target_compile_definitions(gerono.out PRIVATE TARGET_DESKTOP)
target_compile_definitions(gerono.out PRIVATE SIM_GERONO)

target_compile_definitions(heart.out PRIVATE TARGET_DESKTOP)
target_compile_definitions(heart.out PRIVATE SIM_HEART)


include_directories( conway.out PRIVATE desktop )
include_directories( conway.out PRIVATE life )

include_directories( bird.out PRIVATE desktop )
include_directories( bird.out PRIVATE life )
include_directories( bird.out PRIVATE bird )
include_directories( bird.out PRIVATE common )
include_directories( bird.out PRIVATE /opt/X11/include )

include_directories( gerono.out PRIVATE desktop )
include_directories( gerono.out PRIVATE gerono )
include_directories( gerono.out PRIVATE common )
include_directories( gerono.out PRIVATE /opt/X11/include )

include_directories( heart.out PRIVATE desktop )
include_directories( heart.out PRIVATE heart )
include_directories( heart.out PRIVATE common )
include_directories( heart.out PRIVATE /opt/X11/include )

target_compile_options( conway.out
                        PUBLIC
                        -O3
                        -Wfatal-errors
                        -Wall
                        -Wextra
                        -Werror
                        -Wpedantic
                        -Wpointer-arith
                        -Wconversion
                        -fsanitize=undefined
                        -g)

target_compile_options( gerono.out
                        PUBLIC
                        -O3
                        -Wfatal-errors
                        -Wall
                        -Wextra
                        -Werror
                        -Wpedantic
                        -Wpointer-arith
                        -Wconversion
                        -fsanitize=undefined
                        -g)

target_compile_options( heart.out
                        PUBLIC
                        -O3
                        -Wfatal-errors
                        -Wall
                        -Wextra
                        -Werror
                        -Wpedantic
                        -Wpointer-arith
                        -Wconversion
                        -fsanitize=undefined
                        -g)

target_compile_options( bird.out
                        PUBLIC
                        -O3
                        -Wfatal-errors
                        -Wall
                        -Wextra
                        -Werror
                        -Wpedantic
                        -Wpointer-arith
                        -Wconversion
                        -fsanitize=undefined
                        -g)

target_link_libraries( conway.out X11)
target_link_libraries( bird.out X11)
target_link_libraries( gerono.out X11)
target_link_libraries( heart.out X11)

target_link_options(bird.out PUBLIC -fsanitize=undefined)
target_link_options(conway.out PUBLIC -fsanitize=undefined)
target_link_options(gerono.out PUBLIC -fsanitize=undefined)
target_link_options(heart.out PUBLIC -fsanitize=undefined)

