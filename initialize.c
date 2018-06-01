#include<unistd.h>
#include<termios.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include "function.h"

#define cntrl_key(k) ((k) & 0x1f)

//#define cntrlkey(y) ((y) & 31) NOT WORKING
void init(int z)
{
	struct termios editor_settings,before_settings;
	FILE *ip,*op,*test;
	if (!isatty(fileno(stdin)))
	{
		fprintf(stderr,"You are not a terminal, OK.\n");
	}

	ip = fopen("/dev/tty", "r");
	op = fopen("/dev/tty", "w");

	if(!ip || !op)
	{
		fprintf(stderr, "Unable to open /dev/tty\n");
		exit(1);
	}

	tcgetattr(fileno(ip),&before_settings); //get terminal attributes

	editor_settings=before_settings;
	editor_settings.c_lflag &= ~ICANON; //non-canon mode
	editor_settings.c_iflag &= ~IXON; //disabled ctrl+q
    //editor_settings.c_iflag &= ~ICRNL;
	editor_settings.c_lflag &= ~ECHO; //no echo

    unsigned short init_window_size[2];
    get_term_dim(init_window_size);//get initial terminal size

	if(tcsetattr(fileno(ip), TCSANOW, &editor_settings) != 0) //set terminal attributes
	{
		fprintf(stderr,"could not set attributes\n");
		exit(1);
	}

	int ch;

    //clear_scr();
    write(STDOUT_FILENO, "\x1b[s",3);
    write(STDOUT_FILENO, "\033[?47h",6);


	//init_curs();


    //test = fopen("/home/archies/Downloads/test","w");

    if(z==0)
    {

    welcome();
    init_curs();
    do
	{
        read(fileno(ip),&ch,1);

        //fprintf(test,"%d\n",ch);
        //if(ch==CTRL_KEY('q'))
          //    break;


	}while(ch!=cntrl_key('q') && ch!='i');
    if(ch=='i')
    {
        clear_scr();

        insert_mode("UNKNOWN",0);

    }

    else{
	//clear_scr();
	//init_curs();

    write(STDOUT_FILENO, "\033[?47l",6);
    write(STDOUT_FILENO, "\x1b[u",3);
	tcsetattr(fileno(ip), TCSANOW, &before_settings);
    }

    }
	return;
}
