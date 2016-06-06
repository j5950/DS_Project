#include <iostream>
#include<stdio.h>
#include<string.h>
#include<malloc.h>

using namespace std;

typedef struct Adj_w{       // US vertex의 멤버로 한 user가 사용한 모든 word 를 point하기 위해 linked list 이용
    char word[100];
    struct Adj_w* next;
}Adj_w;

void init_Adj_w(Adj_w* self)
{
    strcpy(self->word,"");
    self->next = NULL;
}

void add_Adj_w(Adj_w* self,char* wd)
{
    Adj_w* a = (Adj_w*)malloc(sizeof(Adj_w));
    init_Adj_w(a);
    strcpy(a->word,wd);

    self->next = a;
}





typedef struct Adj_id{      //WD vertex의 멤버로 한 word를 사용한 모든 user id를 point하기 위해 linked list 이용
    char id[50];
    struct Adj_id* next;
}Adj_id;


void init_Adj_id(Adj_id* self)
{
    strcpy(self->id,"");
    self->next = NULL;
}


void add_Adj_id(Adj_id* self,char* id)
{
    Adj_id* a = (Adj_id*)malloc(sizeof(Adj_id));
    init_Adj_id(a);
    strcpy(a->id,id);

    self->next = a;
}

typedef struct Adj_fr
{
    char fr_id[50];
    struct Adj_fr* next;
}Adj_fr;

void init_Adj_fr(Adj_fr* self)
{
    strcpy(self->fr_id,"");
    self->next = NULL;
}

void add_Adj_fr(Adj_fr* self, char* fr_id)
{
    Adj_fr* a = (Adj_fr*)malloc(sizeof(Adj_fr));
    init_Adj_fr(a);
    strcpy(a->fr_id,fr_id);
    self->next = a;
}

typedef struct US_info      //US vertex
{
    char id[50];
    char name[50];
    int tweetnum;
    Adj_w* wd; // user가 사용한 모든 단어들은 linked list로 저장한다.
    Adj_fr* frd;
}US_info;



void init_US(US_info* self)
{
    strcpy(self->id,"");
    strcpy(self->name,"");
    self->tweetnum=0;

    Adj_w* a = (Adj_w*)malloc(sizeof(Adj_w));
    init_Adj_w(a);
    self->wd = a;

    Adj_fr* b = (Adj_fr*)malloc(sizeof(Adj_fr));
    init_Adj_fr(b);
    self->frd = b;
}

typedef struct Adj_h_u{       // user id를 입력시 그 US vertex를 찾기 위한 hash
    struct Adj_h_u* next;
    US_info* upt;
}Adj_h_u;

void init_Adj_h_u(Adj_h_u* self)
{
    self->next = NULL;
    self->upt = NULL;
}

void add_hash_u(Adj_h_u* self, US_info* u) // user 정보 입력 받을 때, id 를 100으로 나눈 나머지를 기준으로 linked list 만들기
{
    Adj_h_u* a = (Adj_h_u*)malloc(sizeof(Adj_h_u));
    init_Adj_h_u(a);
    a->upt = u;
    self->next = a;
}



typedef struct WD_info
{
    char wd[100];
    int num;
    Adj_id* id;
}WD_info;


void init_WD(WD_info* self)
{
    strcpy(self->wd,"");
    self->num=1;

    Adj_id* a = (Adj_id*)malloc(sizeof(Adj_id));
    init_Adj_id(a);

    self->id = a;
}


typedef struct Adj_h_w{
    struct Adj_h_w* next;
    WD_info* wpt;
}Adj_h_w;

void init_Adj_h_w(Adj_h_w* self)
{
    self->next = NULL;
    self->wpt = NULL;
}

void add_hash_w(Adj_h_w* self, WD_info* w)
{
    Adj_h_w* a = (Adj_h_w*)malloc(sizeof(Adj_h_w));
    init_Adj_h_w(a);
    a->wpt=w;
    self->next = a;
}

Adj_h_u* hs_u[100]; // 100으로 나눈 나머지 user
Adj_h_w* hs_w[100]; // word



int main()
{
    FILE* frd;
    FILE* user;
    FILE* word;

    frd = fopen("friend.txt","rt");
    word = fopen("word.txt","rt");
    user = fopen("user.txt","rt");

    long long int totaluser=0, totalfriendship=0, totaltweet=0;

    char id_st[50]={0,};
    char name_st[50]={0,};
    char tmp[50]={0,};


    for(int i=0;i<100;i++)
    {
        hs_u[i] = (Adj_h_u*)malloc(sizeof(Adj_h_u));
        init_Adj_h_u(hs_u[i]);

        hs_w[i] = (Adj_h_w*)malloc(sizeof(Adj_h_w));
        init_Adj_h_w(hs_w[i]);
    }

    while(!feof(user))
    {
        fgets(id_st,50,user);
        fgets(tmp,50,user); //날짜 저장할 필요 x
        fgets(name_st,50,user);
        fgets(tmp,50,user); // 빈줄 저장할 필요 x

        US_info* a = (US_info*)malloc(sizeof(US_info));
        init_US(a);

        strcpy(a->id,id_st);
        strcpy(a->name,name_st);

        int hs_u_index=((id_st[8]-'0')+10*(id_st[7]-'0'))%100; // hash를 id를 100으로 나눈 나머지를 index로 취하므로, id는 9자리니까 char형을 int형으로 바꾸어 저장
        if(hs_u_index<0)
            hs_u_index+=100;

        Adj_h_u* tmp = hs_u[hs_u_index];
        while(1)
        {
            if(tmp->next==NULL)
            {
                add_hash_u(tmp, a);
                break;
            }
            else
                tmp=tmp->next;
        }
        totaluser++;
    }

    char word_st[100];

    while(!feof(word))
    {

        fgets(id_st,50,word);
        fgets(tmp,50,word); //날짜 저장 x
        fgets(word_st,100,word);
        fgets(tmp,50,word); //빈줄 저장 x

        int hs_u_index=((id_st[8]-'0')+10*(id_st[7]-'0'))%100; // hash를 id를 100으로 나눈 나머지를 index로 취하므로, id는 9자리니까 char형을 int형으로 바꾸어 저장

        if(hs_u_index<0)
            hs_u_index+=100;

        if(hs_u[hs_u_index]->next==NULL) // hash index에 등록된 user 없음 --> 등록되지 않은 user면 무효 처리
        {
            continue;
        }

        Adj_h_u* tmp_u=hs_u[hs_u_index]->next; //hashing 에 해당하는 index가 가리키는 곳 참조
        while(1)    // id_st에 해당하는 id에 해당하는 hash index를 참조하여, id를 찾은 뒤 있으면 id vertex의 tweetnum+1해주고 word를 더해준다.
        {
            if(strcmp(tmp_u->upt->id,id_st)==0)
            {
                tmp_u->upt->tweetnum++;
                add_Adj_w(tmp_u->upt->wd, word_st);
                break;
            }
            else if(tmp_u->next==NULL)
                break;
            else
                tmp_u=tmp_u->next;
        }

        if(tmp_u->upt==NULL)    // 등록되지 않은 user 면 무효처리
            continue;

        int hs_w_index = (word_st[strlen(word_st)-1]+16*word_st[strlen(word_st)-2])%100;    // word 문자열 값을 100으로 나눈 나머지이기 때문에, 마지막 두 글자에 대한 16진법 값을 계산후 100으로 나눈 나머지

        if(hs_w_index<0)
            hs_w_index+=100;


        if(hs_w[hs_w_index]->next==NULL)
        {
            WD_info* a = (WD_info*)malloc(sizeof(WD_info));
            init_WD(a);
            strcpy(a->wd,word_st); // vertex 생성 및 단어 넣기

            add_Adj_id(a->id,id_st); // word 사용한 id linked list로 넣기
            add_hash_w(hs_w[hs_w_index], a);
            continue;
        }

        Adj_h_w* tmp_w=hs_w[hs_w_index]->next; //hashing 에 해당하는 index 참조

        while(1)
        {
            if(strcmp(tmp_w->wpt->wd,word_st)==0) //해당하는 index에 이전에 이 단어가 나온적이 있으면, user id추가, num+1 해줌
            {
                tmp_w->wpt->num++;
                add_Adj_id(tmp_w->wpt->id, id_st);
                break;
            }
            else if(tmp_w->next==NULL)               //해당 index에 해당하는 단어 없으면 추가
            {
                WD_info* a = (WD_info*)malloc(sizeof(WD_info));
                init_WD(a);
                strcpy(a->wd,word_st); // vertex 생성 및 단어 넣기

                add_Adj_id(a->id,id_st); // word 사용한 id linked list로 넣기

                add_hash_w(tmp_w,a);

                break;
            }
            else
                tmp_w=tmp_w->next;

        }

        totaltweet++;
    }
    printf("%lld %lld\n",totaltweet,totaluser);
    char fr_id_st[10]={0,};

    while(!feof(frd))
    {
        fgets(id_st,sizeof(id_st),frd); //  주체 id
        fgets(fr_id_st,sizeof(fr_id_st),frd); // 친구 id
        fgets(tmp,50,frd); //빈줄 저장x

    }
    int nmenu;

    while(1) // Interface
    {
        printf("0. Read data files\n");
        printf("1. display statistics\n");
        printf("2. Top 5 most tweeted words\n");
        printf("3. Top 5 most tweeted users\n");
        printf("4. Find users who tweeted a word (e.g., ’연세대’)");
        printf("5. Find all people who are friends of the above users\n");
        printf("6. Delete all mentions of a word\n");
        printf("7. Delete all users who mentioned a word\n");
        printf("8. Find strongly connected components\n");
        printf("9. Find shortest path from a given user\n");
        printf("99. Quit\n");
        printf("Select Menu: ");
        scanf("%d",&nmenu);
    }
}
/*
int main()
{
    FILE* p;
    p=fopen("user.txt","rt");
    char s[15];
    char name[15];
    char tmp[50];
    fgets(s,sizeof(s),p);
    fgets(tmp,sizeof(tmp),p);
    fgets(name,sizeof(name),p);



    printf("%s %s %s",s,tmp,name);
}
*/
