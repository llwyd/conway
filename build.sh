#!/bin/sh

if [ ! bin/ ]; then
    mkdir bin
fi

cmake .
cmake --build .

