/* xsh_hello.c - xsh_hello */

#include <xinu.h>
#include <string.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 * xsh_hello - obtain and print the hello world
 *------------------------------------------------------------------------
 */
shellcmd xsh_hello(int nargs, char *args[]) {
    if (nargs < 0){
        printf("No argument unpacked. Expected one.");
        return 1;
    }
    else if (nargs > 2){
        printf("Too many arguments to unpack.\n");
        return 1;
    }
	printf("Hello %s,  Welcome to the world of Xinu!! \n", args[1] );
    return 0;
	
}
