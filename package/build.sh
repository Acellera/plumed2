DIR="$CONDA_DEFAULT_ENV"
mkdir -p "$DIR/lib/"
mkdir -p "$DIR/bin/"

printenv

./configure --prefix=$DIR --disable-mpi
make -j 4
cd acemd
make

cp libplumed2.so libplumedKernel.so libplumed2plugin.so  "$DIR/lib"






