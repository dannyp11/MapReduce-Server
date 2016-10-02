#!/bin/bash

# try to build
make

# scp src folder to nunki
make clean
scp -r ../ee450_projects datpham@nunki:~/ee450/project


