#!/bin/bash

CC=gcc FC=gfortran  CXX=g++

# if [ "$CC" == "" ]; then CC=gcc; fi
# if [ "$FC" == "" ]; then FC=gfortran; fi
# if [ "$CXX" == "" ]; then CXX=g++; fi

cp -r $RECIPE_DIR/acemd .

DIR="$PREFIX"

printenv

./configure --prefix=$DIR --disable-mpi CC=$CC CXX=$CXX FC=$FC
make -j 2 > log 2>&1

make install

ls -l src/lib/install

cp src/lib/install/libplumed.* acemd/libplumed2.so 
cp src/lib/install/libplumedKernel.* acemd/libplumedKernel.so
 
cd acemd
make CC=$CC TCL="-I$SYS_PREFIX/include -L$SYS_PREFIX/lib" 

cp libplumed2plugin.so $DIR/lib

