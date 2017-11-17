#!/bin/bash

printenv

# Standard plumed2 build+install
./configure --prefix=$PREFIX --disable-mpi
make -j 2 > log 2>&1
make install

# A compatibility symlink until  acemd3 and local openmm-plumed are updated.
ln -s libplumed.so $PREFIX/lib/libplumed2.so || ln -s libplumed.dylib $PREFIX/lib/libplumed2.dylib

# Build and install acemd's plugin
cp -r $RECIPE_DIR/../acemd2_plugin .
cd acemd2_plugin
make
cp libplumed2plugin.so $PREFIX/lib

