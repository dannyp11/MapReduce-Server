#!/bin/bash

# try to build
make

# scp src folder to nunki
make clean

echo "syncing with nunki..."
rsync -auR --no-implied-dirs --files-from=sync_list.txt . datpham@nunki:~/ee450/project
rsync -avuR --no-implied-dirs --include="*.cpp" --include="*.h" --exclude="*" . datpham@nunki:~/ee450/project
echo "done"

