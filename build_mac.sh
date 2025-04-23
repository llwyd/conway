#!/bin/sh

if [ ! bin/ ]; then
    mkdir bin
fi

cmake -DTARGET_MAC=ON .
cmake --build  .

