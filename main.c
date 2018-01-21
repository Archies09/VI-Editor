#include <stdio.h>
#include <fcntl.h> //for open
#include <stdlib.h>
#include "initialize.h"
#include "opening.h"

#include "function.h"
#include <signal.h> //for signal

//#include "view.h"
//#include "create.h"
int count=0;
void resizer (int signumber)
{
    /*FILE *test;
    test = fopen("/home/archies/Downloads/test","w");
    fprintf(test,"%d",count);
    count=count+1;
    fclose(test);
    */
    unsigned short window_size[2];
    get_term_dim(window_size);
    clear_scr();
    init_curs();

    //redraw(window_size,0);
}

int main(int argc,char *argv[])
{
    signal(SIGWINCH,resizer);

    if(argc==1) //no argument
	{
	init(0);//

	}
    else if(argc==2)
	{
	 int fd=open(argv[1],O_RDONLY); //assuming filename
     char *fname;
	 if(fd==-1)
	  openi(argv[1]);//printf("NOT FOUND");
	 else
	  {init(1);insert_mode(argv[1],1);}//printf("FOUND %d %d",13,fd);

	}
    return 0;
}
