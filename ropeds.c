#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "ropeds.h"

void inserttop(struct Node** head,char ch,unsigned long long lno)
{
    struct Node* temp = (struct Node*) malloc(sizeof(struct Node));
    temp->ch= ch;
    temp->lineno=lno;
    //temp->charno=cno;
    temp->prev = NULL;

    *head = temp;


}
void insertlast(struct Node** head,char ch,unsigned long long lno)
{
    struct Node* temp = (struct Node*) malloc(sizeof(struct Node));
    struct Node *p = *head;
    temp->ch=ch;
    temp->next=NULL;
    temp->lineno=lno;
    //temp->charno=cno;
   /* if(*head == NULL)
    {
        temp->prev=NULL;
        *head = temp;
        return;
    }
*/
    while(p->next!=NULL)
        p=p->next;
    p->next=temp;
    temp->prev=p;
    return;
}
void insertmid(struct Node** head,char ch,unsigned long long cno,unsigned long long lno)
{
    struct Node* temp = (struct Node*) malloc(sizeof(struct Node));
    struct Node *p = *head;

    temp->ch=ch;
    temp->lineno=lno;
    //temp->charno=cno;

    while(cno--)
        p=p->next;
    temp->next=p->next;
    p->next=temp;
    temp->prev=p;
    temp->next->prev=temp;
    return;
}

void deletebegin(struct Node** head)
{
    struct Node *p = *head;
    *head = p->next;
    free(p);
    return;
}

void deletein(struct Node* p)
{
    if(p->next!=NULL)//not last
        p->next->prev = p->prev;
    p->prev->next = p->next;
    free(p);
    return;
}

void displayll(struct Node** head,unsigned long long finalline)
{
    struct Node *p=*head;
    unsigned long long lno=p->lineno;
    while(p!=NULL)
    {
        while(p->lineno==lno)
            {write(STDOUT_FILENO,&(p->ch),sizeof(char));
                p=p->next;
                if(p==NULL)
                    break;
            }
        if(p!=NULL)
            write(STDOUT_FILENO,"\n",1);
        lno++;

    }

    while(lno-1!=finalline)
    {
        write(STDOUT_FILENO,"\n",1);
        lno++;
    }
}

void savell(char *fname,struct Node** head)
{
    FILE *opt;
    opt = fopen(fname, "w");
    struct Node *p=*head;
    unsigned long long temp=p->lineno;
    while(p!=NULL)
    {

        for(unsigned long long i=0;i<((p->lineno)-temp);i++)
                fprintf(opt,"\n");
        fprintf(opt,"%c",p->ch);
        temp=p->lineno;
        if(p->next!=NULL)
            p=p->next;
        else break;
    }

    fclose(opt);
    return;
}
/*int main()
{

    struct Node* head = NULL;
    inserttop(&head,'a',1,1);
    return 0;
}
*/
