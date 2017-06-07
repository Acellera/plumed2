all: libplumed2.so libplumedKernel.so libplumed2plugin.so

# TG added so one can compile outside of conda. Failure is ok.
PLUMED_ROOT=$(shell plumed info --root)

libplumed2plugin.so:
	$(CC) -O2  -g --shared -fPIC -o libplumed2plugin.so acemdplugin.c  -I. -I../src/wrapper/ -I$(PLUMED_ROOT)/src/wrapper -L. -lplumed2  $(TCL)
#	$(CC) -o test test.c -ldl $(

libplumed2.so:
	cd ../ && make -j 6 && cd - && cp ..//src/lib/install/libplumed.so libplumed2.so && cp ..//src/lib/install/libplumedKernel.so .

clean:
	rm -f lib*.so
	cd ../ && make clean

