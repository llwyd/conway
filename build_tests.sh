#!/bin/sh

if [ ! bin/ ]; then
    mkdir bin
fi

cmake -DTARGET_MAC=ON -DUNIT_TESTS=ON .
cmake --build  .

