DIR="$CONDA_DEFAULT_ENV"
mkdir -p "$DIR/lib/"
mkdir -p "$DIR/bin/"

printenv

./configure --prefix=$DIR --disable-mpi CC=gcc-4.4 CXX=g++-4.4 FC=gfortran-4.4
make -j 4 
cd acemd
make CC=gcc-4.4

cp libplumed2.so libplumedKernel.so libplumed2plugin.so  "$DIR/lib"






