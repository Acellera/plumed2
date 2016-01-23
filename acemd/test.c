#include <dlfcn.h>
#include <stdio.h>

void main(void) {

 void *      ptr = dlopen( "libplumed2plugin.so", RTLD_LAZY | RTLD_LOCAL );

	printf("%x [%s]\n", ptr, dlerror() );

}


