#include <stdio.h>

int main(int argc, char* argv[]) 
{
    if (argc > 1) 
	{
        printf("Wojciech Szewczyk, numer wątku/procesu: %s\n", argv[1]);
    }
    return 0;
}
