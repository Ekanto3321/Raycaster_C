#!/bin/bash

rm ray
gcc -o ray main.c -lraylib -lGL -lm -lpthread -ldl -lX11 -lXrandr -lXi
./ray
