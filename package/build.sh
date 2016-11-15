if [ "$CC" == "" ]; then CC=gcc; fi
if [ "$FC" == "" ]; then FC=gfortran; fi
if [ "$CXX" == "" ]; then CXX=g++; fi

DIR="$CONDA_DEFAULT_ENV"
mkdir -p "$DIR/lib/"
mkdir -p "$DIR/bin/"

printenv

./configure --prefix=$DIR --disable-mpi CC=$CC CXX=$CXX FC=$FC
make -j 4 


ls -l src/lib/install


cp src/lib/install/libplumed.* acemd/libplumed2.so 
cp src/lib/install/libplumedKernel.* acemd/libplumedKernel.so
 
cd acemd
make CC=$CC TCL="-I$SYS_PREFIX/include -L$SYS_PREFIX/lib" 

cp -r ../patches "$DIR/lib"
cp ../src/lib/install/plumed "$DIR/bin/plumed.bin"

# Toni - not sure if lib / bin copying is needed. The --standalone-executable quells the PLUMED_ROOT warning. 
echo '#!/bin/bash' > "$DIR/bin/plumed"
echo 'DIR=$(dirname "$(which python)"); LD_LIBRARY_PATH="$DIR/../lib/compat-libc"; PLUMED_ROOT="$DIR/../lib" "$DIR/plumed.bin" $@' >> "$DIR/bin/plumed"
chmod +x "$DIR/bin/plumed"

cp libplumed2.so libplumedKernel.so libplumed2plugin.so  "$DIR/lib"

if [ "$TRAVIS_OS_NAME" == "osx" ]; then
	cd "$DIR/lib"
	ln -s libplumed2.so libplumed2.dylib
	ln -s libplumedKernel.so libplumedKernel.dylib
	ln -s libplumed2plugin.so libplumed2plugin.dylib
fi


