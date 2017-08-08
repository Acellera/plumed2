default: libplumed2plugin.so

# TG some include paths added so one can compile outside of conda. Failure is ok.
PLUMED_ROOT=$(shell plumed info --root)
CONDA_ROOT=$(shell conda info --root)

# tcl.h is needed by aceplug.h. Using conda's distributed in the tk
# package, for reproducibility. Before it was...
# -I$(SYS_PREFIX)/include -L$(SYS_PREFIX)/lib 


libplumed2plugin.so:
	$(CC) -std=c99 -O2  -g --shared -fPIC -o libplumed2plugin.so acemdplugin.c  \
	 	-I. -I../src/wrapper/ -I$(PLUMED_ROOT)/src/wrapper -I$(CONDA_ROOT)/include/plumed/wrapper \
		-I $(CONDA_ROOT)/include \
		-L. -L$(PREFIX)/lib -L$(CONDA_ROOT)/lib  -lplumed

test:
	$(CC) -o test test.c -ldl $(

clean:
	rm -f lib*.so
	cd ../ && make clean

