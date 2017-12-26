# GameChest
Container for games

------------------

#!/bin/bash

mv server lastSuccess
g++ pre/moduleListGen.cpp -o listGen
./listGen
rm -rf listGen
g++ *.cpp Modules/*.cpp -o server
./server 5555 &
python client.py

------------------

bot / module api to come
