#!/bin/bash

printenv

# Standard plumed2 build+install
./configure --prefix=$PREFIX --disable-mpi
make -j $CPU_COUNT
make install

# A compatibility symlink until  acemd3 and local openmm-plumed are updated.
if [ -e $PREFIX/lib/libplumed.so ]; then 
    ln -s libplumed.so $PREFIX/lib/libplumed2.so
elif [ -e $PREFIX/lib/libplumed.dylib ]; then
    ln -s libplumed.dylib $PREFIX/lib/libplumed2.dylib
else
    echo "File libplumed.{so|dylib} not generated, aborting"
    exit 1
fi

# Build and install ACEMD2 plugin
cd acemd2_plugin
make
cp libplumed2plugin.so $PREFIX/lib

