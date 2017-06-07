#!/bin/bash

cp -r $RECIPE_DIR/acemd .

DIR="$PREFIX"

printenv

./configure --prefix=$DIR --disable-mpi
make -j 2 > log 2>&1

make install

ls -l src/lib/install

cp src/lib/install/libplumed.* acemd/libplumed2.so 
cp src/lib/install/libplumedKernel.* acemd/libplumedKernel.so
 
cd acemd
make CC=$CC TCL="-I$SYS_PREFIX/include -L$SYS_PREFIX/lib" 

cp libplumed2plugin.so $DIR/lib

