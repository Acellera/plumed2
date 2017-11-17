#include <dlfcn.h>
#include <stdio.h>

int main(void)
{
    void *ptr = dlopen("libplumed2plugin.so", RTLD_LAZY | RTLD_LOCAL);
    if (!ptr)
    {
        printf("%s\n", dlerror());
        return 1;
    }
    return 0;
}


