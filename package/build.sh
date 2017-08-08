#!/bin/bash

printenv

./configure --prefix=$PREFIX --disable-mpi
make -j 2 > log 2>&1
make install

cp -r $RECIPE_DIR/acemd .
cd acemd
make
cp libplumed2plugin.so $PREFIX/lib

