struct Node
{
    char ch;
    unsigned long long lineno;
    //int charno;
    struct Node *next;
    struct Node *prev;

};

void inserttop(struct Node**,char,unsigned long long);
void insertlast(struct Node**,char,unsigned long long);
void insertmid(struct Node**,char,unsigned long long,unsigned long long);
void deletebegin(struct Node**);
void deletein(struct Node*);
void displayll(struct Node**,unsigned long long);
void savell(char *,struct Node**);
