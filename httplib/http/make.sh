#!/bin/bash
cd build
mycmake
make
cp httpclient  ../
cp compile_commands.json ../
./httpclient

