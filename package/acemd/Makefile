all: libplumed2plugin.so

# TG added so one can compile outside of conda. Failure is ok.
PLUMED_ROOT=$(shell plumed info --root)

CONDA_ROOT=$(PREFIX)

libplumed2plugin.so:
	$(CC) -O2  -g --shared -fPIC -o libplumed2plugin.so acemdplugin.c  -I. -I../src/wrapper/ -I$(PLUMED_ROOT)/src/wrapper -L. -L$(CONDA_ROOT)/lib -lplumed
#	$(CC) -o test test.c -ldl $(

clean:
	rm -f lib*.so
	cd ../ && make clean

