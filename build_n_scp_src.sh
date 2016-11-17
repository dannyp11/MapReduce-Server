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

# create tar submit file
rm -f ee450_datpham_session2.tar.gz
tar -cvf ee450_datpham_session2.tar -T tar_list.txt

gzip ee450_datpham_session2.tar

#restore Makefile
sed -i "s/$NEW/$ORIG/g" Makefile
