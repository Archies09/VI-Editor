#include<unistd.h>//write
#include<sys/ioctl.h>//windowsizeandcheck
#include<sys/wait.h>
#include<stdio.h>
#include<stdlib.h>
#include<termios.h>
#include<string.h>
#include "ropeds.h"
#include "initialize.h"
#include <signal.h>
#define ESC 27
#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77
#define ENTER 13

unsigned short win_size[2];
struct Node* head = NULL;
struct termios my_editor;
unsigned short termlines=0;
unsigned short termchars=0;
    unsigned long long alpha_counter=0;
    //unsigned long long line_counter=0;
    unsigned long long alpha_current=0;
    unsigned long long line_current=0;



unsigned long long line_counter=0;
    unsigned long long multiple=0;
    int prev_flag=0;
void normal_mode(char*);
void clear_scr() //http://man7.org/linux/man-pages/man4/console_codes.4.html
{
	write(STDOUT_FILENO, "\x1b[3J",4);
	write(STDOUT_FILENO, "\x1b[2J",4);
}

void welcome()
{
    write(STDOUT_FILENO, "					WELCOME TO ARCHIES EDITOR",31);
}

void init_curs() //http://man7.org/linux/man-pages/man4/console_codes.4.html
{
    write(STDOUT_FILENO, "\x1b[H", 3);
}

void get_term_dim(unsigned short *window_size) /*struct winsize *argp
Get window size.
TIOCSWINSZ
const struct winsize *argp

Set window size.
The struct used by these ioctls is defined as
struct winsize {
    unsigned short ws_row;
    unsigned short ws_col;
    unsigned short ws_xpixel;   unused
    unsigned short ws_ypixel;   unused
};

When the window size changes, a SIGWINCH signal is sent to the foreground process group.
*/
{
    struct winsize term_size;
    ioctl(STDOUT_FILENO,TIOCGWINSZ,&term_size);
    window_size[0]=term_size.ws_row;
    window_size[1]=term_size.ws_col;
    return;
}

void refresh_line(struct Node** head,unsigned long long currline,unsigned long long multiple)
{

    write(STDOUT_FILENO,"\x1b[2K",4);
    write(STDOUT_FILENO,"\x1b[0E",4);
    write(STDOUT_FILENO,"\x1b[0F",4);
    struct Node *p = *head;
    while(p->lineno!=currline)
        p=p->next;

    while(multiple--)
    {

        unsigned short win_size[2];
        get_term_dim(win_size);
        for(unsigned short i=0;i<win_size[1];i++)
            p=p->next;

    }
    while(p->lineno==currline)
        {char t=p->ch;
        write(STDOUT_FILENO,&t,sizeof(char));

        if(p->next==NULL)
            break;
        p=p->next;
        }

}

/*void redraw(unsigned short* windowsize,int state)
{
    if(state==1)//editing state
    {
        clear_scr();
        init_curs();
        displayll();
    }
}*/

void move_forward()
{
    write(STDOUT_FILENO,"\x1b[0C",4);

}

void move_backward()
{
    write(STDOUT_FILENO,"\x1b[0D",4);

}

void move_up()
{
    write(STDOUT_FILENO,"\x1b[0A",4);

}

void move_down()
{
    write(STDOUT_FILENO,"\x1b[0B",4);

}

void status_scroll()
{
    write(STDOUT_FILENO,"\x1b[999B",6);
    write(STDOUT_FILENO,"\x1b[999D",6);
}

void catchredraw(int signumber)
{
    //unsigned short window_size[2];
    get_term_dim(win_size);
    clear_scr();
    init_curs();
    displayll(&head,line_counter);
    //redraw(win_size,1);
}

void insert_mode(char *fname,int v)
{
    signal(SIGWINCH,catchredraw);
    char k;
        unsigned short count=0;

    get_term_dim(win_size);

    if(v==1)
    {
      FILE *ipt;
      ipt=fopen(fname,"r");
    do
    {
        k=fgetc(ipt);
        termchars++;
        if(termlines==win_size[0])
        {
            break;
        }

        if(termchars==win_size[1])
        {
            termchars=0;
            termlines++;
            line_counter++;
            line_current++;
            write(STDOUT_FILENO,"\x1b[0E",4);
            count=0;
            multiple=0;
            prev_flag=1;
        }

        if(feof(ipt))
            break;
        if(k==27)
            break;
        if(k!=127)//not backspace
            count++;

        if(count-2==win_size[1])
        {

            multiple++;
        }

        if(k=='\n')
        {
            //printf("JJJJJJ");
            //write(STDOUT_FILENO,"\n",1);
            line_counter++;
            line_current++;
            write(STDOUT_FILENO,"\x1b[0E",4);
            count=0;
            multiple=0;
            prev_flag=1;
            termchars=0;
            termlines++;

        }

        else if(k==127) //backspace
        {

            if(count==0)//newline
            {
                //write(STDOUT_FILENO,"\x1b[0F",4);
                struct Node *u=head;
                while(u->next!=NULL)
                    u=u->next;
                while(u->lineno!=line_counter)
                {line_counter--;line_current--;
                write(STDOUT_FILENO,"\x1b[0A",4);
                }

                struct Node *p=head;
                while(p->next!=NULL && p->lineno!=line_counter)
                    p=p->next;
                while(p->lineno==line_counter && p->next!=NULL)
                        {write(STDOUT_FILENO,"\x1b[0C",4);count++;if(p->next==NULL)break;p=p->next;}


            }
            else if(alpha_counter!=0)
            {
            struct Node *p=head;
            for(unsigned long long i=0;i<alpha_counter-1;i++)
                p=p->next;
            if(p->prev==NULL)
                {deletebegin(&p);clear_scr();init_curs();}
            else{
            deletein(p);
            if(count==1)//last char on line
                {write(STDOUT_FILENO,"\x1b[2K",4);write(STDOUT_FILENO,"\x1b[0E",4);
                    write(STDOUT_FILENO,"\x1b[0F",4);}

            else refresh_line(&head,line_current,multiple);
            }
            alpha_counter--;
            alpha_current--;
            count--;
            }
            else{/*do nothing*/}
        }

        else if(k=='\x1b')
        {
            read(STDIN_FILENO,&k,1);
            read(STDIN_FILENO,&k,1);
            if(k=='A')
               {move_up();
               }
            if(k=='B')
               {move_down();
               }
            if(k=='C')
               {move_forward();alpha_current++;}
            if(k=='D')
               {move_backward();alpha_current--;}

        }


        else
        {
        if(alpha_current==0 && line_counter==0)//cursor at beginning
            inserttop(&head,k,line_current);
        else if(alpha_counter==alpha_current)
             insertlast(&head,k,line_current);
        else    insertmid(&head,k,alpha_current,line_current);

        refresh_line(&head,line_current,multiple);
        alpha_counter++;alpha_current++;
        }

    }while(1);
    while(termlines==win_size[0])
    {
        char ltt;
        read(STDIN_FILENO,&ltt,1);
        if(ltt=='\x1b')
        {
            read(STDIN_FILENO,&ltt,1);
            read(STDIN_FILENO,&ltt,1);
            if(ltt=='B')
            {
                write(STDOUT_FILENO,"\x1b[1S",4);
                termlines--;
                termchars=0;
                char ltm;
                ltm=k;
                write(STDOUT_FILENO,"\x1b[0E",4);
                for(unsigned short i=0;i<win_size[1];i++)
                {
                    write(STDOUT_FILENO,&ltm,1);
                    insertlast(&head,ltm,line_counter);
                    ltm=fgetc(ipt);

                }
                line_counter++;
                termlines++;
            }
        }
        else if(ltt==':')
            normal_mode(fname);
        else if(ltt=='i')
                insert_mode(fname,0);

    }

    fclose(ipt);

    }
    else
    {

    do
    {
        read(STDIN_FILENO,&k,1);
        if(k==27)
            break;
        if(k!=127)//not backspace
            count++;

        if(count-2==win_size[1])
        {

            multiple++;
        }

        if(k=='\n')
        {
            FILE *test;
            test = fopen("/home/archies/Downloads/test","w");
            fprintf(test,"%hu %hu",win_size[0],win_size[1]);
            fclose(test);
            //write(STDOUT_FILENO,"\n",1);
            line_counter++;
            line_current++;
            write(STDOUT_FILENO,"\x1b[0E",4);
            count=0;
            multiple=0;
            prev_flag=1;
        }

        else if(k==127) //backspace
        {

            if(count==0)//newline
            {
                //write(STDOUT_FILENO,"\x1b[0F",4);
                line_counter--;line_current--;
                write(STDOUT_FILENO,"\x1b[0A",4);
                struct Node *p=head;
                while(p->next!=NULL && p->lineno!=line_counter)
                    p=p->next;
                while(p!=NULL && p->lineno==line_counter)
                        {write(STDOUT_FILENO,"\x1b[0C",4);count++;if(p->next==NULL)break;p=p->next;}


            }
            else if(alpha_counter!=0)
            {
            struct Node *p=head;
            for(unsigned long long i=0;i<alpha_counter-1;i++)
                p=p->next;
            if(p->prev==NULL)
                {deletebegin(&p);clear_scr();init_curs();}
            else{
            deletein(p);
            if(count==1)//last char on line
                {write(STDOUT_FILENO,"\x1b[2K",4);write(STDOUT_FILENO,"\x1b[0E",4);
                    write(STDOUT_FILENO,"\x1b[0F",4);}

            else refresh_line(&head,line_current,multiple);
            }
            alpha_counter--;
            alpha_current--;
            count--;
            }
            else{/*do nothing*/}
        }

        else if(k=='\x1b')
        {
            read(STDIN_FILENO,&k,1);
            read(STDIN_FILENO,&k,1);
            if(k=='A')
               {move_up();
               }
            if(k=='B')
               {move_down();
               }
            if(k=='C')
               move_forward();
            if(k=='D')
               move_backward();

        }


        else
        {
        if(alpha_current==0 && line_current==0)//cursor at beginning
            inserttop(&head,k,line_current);
        else if(alpha_counter==alpha_current)
             insertlast(&head,k,line_current);
        else    insertmid(&head,k,alpha_current,line_current);

        refresh_line(&head,line_current,multiple);
        alpha_counter++;alpha_current++;
        }

    }while(1);
    }
    normal_mode(fname);
    return;
}


void normal_mode(char *fname)
{
    signal(SIGWINCH,catchredraw);
    char k;
    do{

        read(STDIN_FILENO,&k,1);
        if(k=='g')
        {
         read(STDIN_FILENO,&k,1);
         if(k=='g')
        {
                write(STDOUT_FILENO,"\x1b[999A",6);
                write(STDOUT_FILENO,"\x1b[999D",6);
        }
        }

        if(k=='\x1b')
        {
            read(STDIN_FILENO,&k,1);
            read(STDIN_FILENO,&k,1);
            if(k=='A')
               {move_up();
               }
            if(k=='B')
               {move_down();
               }
            if(k=='C')
               move_forward();
            if(k=='D')
               move_backward();

        }
        else if(k=='h')
            move_backward();
        else if(k=='j')
            move_forward();
        else if(k=='k')
            move_up();
        else if(k=='l')
            move_down();


        else if(k==':')
        {
              status_scroll();

              FILE *tp,*tpp;
              tp = fopen("/dev/tty", "r");
              tpp = fopen("/dev/tty", "w");
              tcgetattr(fileno(tp),&my_editor);
              my_editor.c_lflag &= ICANON;
              my_editor.c_lflag &= ECHO;
              tcsetattr(fileno(tp), TCSANOW, &my_editor);
              //read(STDIN_FILENO,&k,1);
              printf(":");
              char kp[100];
              scanf("%s",kp);
              //printf("%s",kp);
              if(strcmp(kp,"w")==0)
              {
                  //read(STDIN_FILENO,&k,1);
                  //if(k=='\n')//save

                        savell(fname,&head);
              }

              else if(strcmp(kp,"q")==0)
              {
                  //read(STDIN_FILENO,&k,1);
                  /*if(k=='!')//save
                  {
                      read(STDIN_FILENO,&k,1);
                      if(k=='\n')//save
                                {clear_scr();init_curs();}//savell(fname,&head);
                  }*/
                  //else if(k=='\n')
                    //{
                            //read(STDIN_FILENO,&k,1);
                            char tyyy[100];
                            scanf("%s",tyyy);
                            if(strcmp(tyyy,"y")==0)//save
                                {savell(fname,&head);clear_scr();init_curs();}//savell(fname,&head);
                            else if(strcmp(tyyy,"n")==0)
                                {clear_scr();init_curs();}
                    //}
                        clear_scr();init_curs();my_editor.c_lflag &= ICANON;
                tcsetattr(fileno(tp), TCSANOW, &my_editor);
                exit(0);
              }
              else if(strcmp(kp,"q!")==0)
              {
                  clear_scr();init_curs();my_editor.c_lflag &= ICANON;
                tcsetattr(fileno(tp), TCSANOW, &my_editor); init(0);
                exit(0);

              }
            else{
			//char *x[20];
			clear_scr();

			int update;
			//read(STDIN_FILENO,&x,20);
             pid_t proid;
			if((proid=fork())<0)
			{
				write(STDOUT_FILENO,"ERROR",5);
				exit(1);
			}
			else if(proid==0)//child
			{
				//printf("HII\n");
			    char* argus[2];
			    argus[0]=kp;
			    argus[1]=NULL;

			    //write(STDOUT_FILENO,"HII\n",3);
				if(execvp(argus[0],argus)==-1)
				//write(STDOUT_FILENO,"HII",3);
				{
               				printf("exec failed\n");
               				exit(1);
                }
                //displayll(&head,line_counter);}

			}
			else {while(wait(&update)!=proid);}

		}
                //read(STDIN_FILENO,&k,1);

        }

    }while(k!='i');
    if(k=='i')
        insert_mode(fname,0);
}
