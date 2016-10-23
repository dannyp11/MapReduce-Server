#!/bin/bash

# try to build
#make clean
make

# edit makefile for sunOS compability ---------------------------
ORIG="LDFLAGS = "
NEW="LDFLAGS = -lnsl -lsocket -lresolv "
sed -i "s/$ORIG/$NEW/g" Makefile
# edit makefile for compability ends ------------------------


# scp src folder to nunki
#make clean
echo "syncing with nunki..."
rsync -auR --no-implied-dirs --files-from=sync_list.txt . datpham@nunki:~/ee450/project
rsync -auR --no-implied-dirs --include="*.cpp" --include="*.h" --exclude="*" . datpham@nunki:~/ee450/project
echo "done"

#restore Makefile
sed -i "s/$NEW/$ORIG/g" Makefile

# run makefile on remote machine
#ssh datpham@nunki '( cd ~/ee450/project/ && make clean) ; ( cd ~/ee450/project/ && make)'
