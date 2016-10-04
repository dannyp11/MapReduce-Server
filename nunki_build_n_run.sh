#!/bin/bash

# go to directory
cd ~/ee450/project

# make
make clean
make

echo "------------- make done ----------------------"
echo ""
echo ""

# run
echo "running server A: "
echo ""
./serverA_exe


