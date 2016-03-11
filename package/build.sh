CC=gcc-4.4
CXX=g++-4.4
FC=gfortran-4.4

which $CC
if [ "$?" != "0" ]; then
  CC=gcc
fi
which $FC
if [ "$?" != "0" ]; then
  FC=gfortran
fi
which $CXX
if [ "$?" != "0" ]; then
  CXX=g++-4.4
fi


DIR="$CONDA_DEFAULT_ENV"
mkdir -p "$DIR/lib/"
mkdir -p "$DIR/bin/"

printenv

./configure --prefix=$DIR --disable-mpi CC=$CC CXX=$CXX FC=$FC
make -j 4 
cd acemd
make CC=$CC

cp libplumed2.so libplumedKernel.so libplumed2plugin.so  "$DIR/lib"






