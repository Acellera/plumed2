if [ "$CC" == "" ]; then CC=gcc; fi
if [ "$FC" == "" ]; then FC=gfortran; fi
if [ "$CXX" == "" ]; then CXX=g++; fi

DIR="$CONDA_DEFAULT_ENV"
mkdir -p "$DIR/lib/"
mkdir -p "$DIR/bin/"

printenv

./configure --prefix=$DIR --disable-mpi CC=$CC CXX=$CXX FC=$FC
make -j 4 
cd acemd
make CC=$CC TCL="-I$SYS_PREFIX/include -L$SYS_PREFIX/lib" 

cp ../src/lib/install/plumed "$DIR/bin/plumed.bin"
echo 'DIR=$(dirname "$(which python)"); LD_LIBRARY_PATH="$DIR/.lib/compat-libc" "$DIR/plumed.bin" $@' > "$DIR/bin/plumed"
chmod +x "$DIR/bin/plumed"

cp libplumed2.so libplumedKernel.so libplumed2plugin.so  "$DIR/lib"






