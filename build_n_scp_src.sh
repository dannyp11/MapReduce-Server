#!/bin/bash

# try to build
make

# scp src folder to nunki
make clean
#scp -r ../ee450_projects datpham@nunki:~/ee450/project
echo "syncing with nunki..."
rsync -au --exclude=".*" --exclude="build_n_scp_src.sh*" . datpham@nunki:~/ee450/project
echo "done"

