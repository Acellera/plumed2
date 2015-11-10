all:

	gcc -g --shared -fPIC -o libplumed2plugin.so acemdplugin.c -I. -I../src/wrapper/ -L../src/lib -lplumed 

clean:
	rm libplumed2plugin.so

