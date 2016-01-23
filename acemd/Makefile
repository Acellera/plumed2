all: libplumed2.so libplumedKernel.so

	gcc -g --shared -fPIC -o libplumed2plugin.so acemdplugin.c -I. -I../src/wrapper/ -L. -lplumed2 
	gcc -o test test.c -ldl

libplumed2.so:
	cd ../ && make -j 6 && cd - && cp ..//src/lib/install/libplumed.so libplumed2.so && cp ..//src/lib/install/libplumedKernel.so .

clean:
	rm -f lib*.so
	cd ../ && make clean

