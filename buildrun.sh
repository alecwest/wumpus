#!/bin/sh

rm Default/src/*.o 
rm Default/src/*.d
cd Default
make
cd ..
Default/wumpus_world_csc4240_program_02
