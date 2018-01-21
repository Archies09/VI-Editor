#include <stdio.h>
#include <fcntl.h>
#include <unistd.h> //for write
#include "function.h"
#include "initialize.h"

void openi(char *fname)
{
 init(1);
 //write(STDOUT_FILENO, "					WELCOME TO ARCHIES EDITOR",31);
 insert_mode(fname,0);
 return;
}
