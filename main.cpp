#include <iostream>
#include<stdio.h>
#include<string.h>
#include<malloc.h>

using namespace std;
int cnt; // TOP 5 계산
int totaluser=0, totalfriendship=0, totaltweet=0;


typedef struct Adj_w{       // US vertex의 멤버로 한 user가 사용한 모든 word 를 point하기 위해 linked list 이용
    char word[200];
    struct Adj_w* next;
    struct Adj_w* prev;
}Adj_w;

void init_Adj_w(Adj_w* self)
{
    strcpy(self->word,"");
    self->next = NULL;
    self->prev = NULL;
}

void add_Adj_w(Adj_w* self,char* wd)
{
    Adj_w* a = (Adj_w*)malloc(sizeof(Adj_w));
    init_Adj_w(a);
    strcpy(a->word,wd);
    while(self->next)
        self=self->next;
    self->next = a;
    a->prev = self;
}




typedef struct Adj_id{      //WD vertex의 멤버로 한 word를 사용한 모든 user id를 point하기 위해 linked list 이용
    char id[50];
    struct Adj_id* next;
    struct Adj_id* prev;
}Adj_id;


void init_Adj_id(Adj_id* self)
{
    strcpy(self->id,"");
    self->next = NULL;
    self->prev = NULL;
}


void add_Adj_id(Adj_id* self,char* id)
{
    Adj_id* a = (Adj_id*)malloc(sizeof(Adj_id));
    init_Adj_id(a);
    strcpy(a->id,id);
    while(self->next)
        self=self->next;
    self->next = a;
    a->prev = self;
}

typedef struct Adj_fr
{
    char fr_id[50];
    struct Adj_fr* next;
    struct Adj_fr* prev;

}Adj_fr;

void init_Adj_fr(Adj_fr* self)
{
    strcpy(self->fr_id,"");
    self->next = NULL;
    self->prev = NULL;
}

void add_Adj_fr(Adj_fr* self, char* fr_id)
{
    Adj_fr* a = (Adj_fr*)malloc(sizeof(Adj_fr));
    init_Adj_fr(a);
    strcpy(a->fr_id,fr_id);
    while(self->next)
        self=self->next;
    self->next = a;
    a->prev = self;
}


typedef struct US_info      //US vertex
{
    char id[50];
    char name[50];
    int tweetnum;
    int frdnum;
    Adj_fr* frd;
    Adj_fr* prev;
    Adj_w* wd; // user가 사용한 모든 단어들은 linked list로 저장한다.
}US_info;



void init_US(US_info* self)
{
    strcpy(self->id,"");
    strcpy(self->name,"");
    self->tweetnum=0;
    self->frdnum=0;

    Adj_w* a = (Adj_w*)malloc(sizeof(Adj_w));
    init_Adj_w(a);
    self->wd = a;


    Adj_fr* b = (Adj_fr*)malloc(sizeof(Adj_fr));
    init_Adj_fr(b);
    self->frd = b;
    Adj_fr* c = (Adj_fr*)malloc(sizeof(Adj_fr));
    init_Adj_fr(c);
    self->prev = c;
}

typedef struct Adj_h_u{       // user id를 입력시 그 US vertex를 찾기 위한 hash
    struct Adj_h_u* next;
    struct Adj_h_u* prev;
    US_info* upt;

}Adj_h_u;

void init_Adj_h_u(Adj_h_u* self)
{
    self->prev = NULL;
    self->next = NULL;
    self->upt = NULL;
}

void add_hash_u(Adj_h_u* self, US_info* u) // user 정보 입력 받을 때, id 를 100으로 나눈 나머지를 기준으로 linked list 만들기
{
    Adj_h_u* a = (Adj_h_u*)malloc(sizeof(Adj_h_u));
    init_Adj_h_u(a);
    a->upt = u;
    while(self->next)
        self=self->next;

    self->next = a;
    a->prev = self;
}



typedef struct WD_info{
    char wd[200];
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
    struct Adj_h_w* prev;
    WD_info* wpt;
}Adj_h_w;

void init_Adj_h_w(Adj_h_w* self)
{
    self->next = NULL;
    self->wpt = NULL;
    self->prev = NULL;
}

void add_hash_w(Adj_h_w* self, WD_info* w)
{
    Adj_h_w* a = (Adj_h_w*)malloc(sizeof(Adj_h_w));
    init_Adj_h_w(a);
    a->wpt=w;
    while(self->next)
        self=self->next;
    self->next = a;
    a->prev = self;
}





/////////////////////// Interface function
typedef struct BST
{
    struct BST* left;
    struct BST* right;
    WD_info* wpt;
    US_info* upt;
}BST;

void init_BT(BST* self)
{
    self->left=NULL;
    self->right=NULL;
    self->wpt=NULL;
    self->upt=NULL;
}

BST* BST_insert_wd(BST* tree, WD_info* wpt)
{
    BST* a = (BST*)malloc(sizeof(BST));
    init_BT(a);
    a->wpt=wpt;
    if(tree == NULL)
    {
        tree = a;
    }
    else if(tree->wpt->num > wpt->num)
    {
        if(tree->left==NULL)
            tree->left = a;
        else
            BST_insert_wd(tree->left, wpt);
    }
    else
    {
        if(tree->right==NULL)
            tree->right = a;
        else
            BST_insert_wd(tree->right, wpt);
    }
    return tree;
}

BST* BST_insert_user(BST* tree, US_info* upt)
{
    BST* a = (BST*)malloc(sizeof(BST));
    init_BT(a);
    a->upt=upt;
    if(tree == NULL)
    {
        tree = a;
    }
    else if(tree->upt->tweetnum > upt->tweetnum)
    {
        if(tree->left==NULL)
            tree->left = a;
        else
            BST_insert_user(tree->left, upt);
    }
    else
    {
        if(tree->right==NULL)
            tree->right = a;
        else
            BST_insert_user(tree->right, upt);
    }
    return tree;
}


void RDF(int totuser, int tottweet, int totfrship)
{
    printf("The total of user : %d\n",totuser);
    printf("The total of tweet : %d\n",tottweet);
    printf("The total of friendship : %d\n",totfrship);
}




Adj_h_u* hs_u[1000]; // 100으로 나눈 나머지 user
Adj_h_w* hs_w[1000]; // word

int Findminfrd()
{
    int minv=987654321;
    for(int i=0;i<1000;i++)
    {
        Adj_h_u* tmp = hs_u[i]->next;
        if(tmp==NULL)
            continue;
        while(1)
        {
            if(minv>tmp->upt->frdnum)
                minv=tmp->upt->frdnum;
            if(tmp->next==NULL)
                break;
            else
                tmp=tmp->next;
        }
    }
    return minv;
}

int Findmaxfrd()
{
    int maxv=0;
    for(int i=0;i<1000;i++)
    {
        Adj_h_u* tmp = hs_u[i]->next;
        if(tmp==NULL)
            continue;
        while(1)
        {
            if(maxv<tmp->upt->frdnum)
                maxv=tmp->upt->frdnum;
            if(tmp->next==NULL)
                break;
            else
                tmp=tmp->next;
        }
    }
    return maxv;
}

int Findmintweet()
{
    int minv=987654321;
    for(int i=0;i<1000;i++)
    {
        Adj_h_u* tmp = hs_u[i]->next;
        if(tmp==NULL)
            continue;
        while(1)
        {
            if(minv>tmp->upt->tweetnum)
                minv=tmp->upt->tweetnum;
            if(tmp->next==NULL)
                break;
            else
                tmp=tmp->next;
        }
    }
    return minv;
}

int Findmaxtweet()
{
    int maxv=0;
    for(int i=0;i<1000;i++)
    {
        Adj_h_u* tmp = hs_u[i]->next;
        if(tmp==NULL)
            continue;
        while(1)
        {
            if(maxv<tmp->upt->tweetnum)
            {
                maxv=tmp->upt->tweetnum;
            }
            if(tmp->next==NULL)
                break;
            else
                tmp=tmp->next;
        }
    }
    return maxv;
}


void DisplaySt(int totuser, int tottweet, int totfrship)
{
    printf("아래의 값들은 모두 내림한 값들 입니다.\n");
    printf("Average number of friends : %d\n",totfrship/totuser);
    printf("Minimum number of friends : %d\n",Findminfrd());
    printf("Maximum number of friends : %d\n",Findmaxfrd());
    printf("\n");
    printf("Average tweets per user : %d\n",tottweet/totuser);
    printf("Minimum tweets per user : %d\n",Findmintweet());
    printf("Maximum tweets per user : %d\n",Findmaxtweet());
}




void Print_top5_wd(BST* tree)
{
    if(cnt==5)
        return;
    if(tree->right!=NULL)
        Print_top5_wd(tree->right);
    if(cnt==5)
        return;
    printf("%d번 사용된 단어 word : %s",tree->wpt->num,tree->wpt->wd);
    cnt++;
    if(cnt==5)
        return;
    if(tree->left!=NULL)
        Print_top5_wd(tree->left);
}


void Print_top5_user(BST* tree)
{
    if(tree->right!=NULL&&cnt<5)
        Print_top5_user(tree->right);
    if(cnt>=5)
        return;
    printf("%d번 Tweet한  user infomation\nid :%sname :%s",tree->upt->tweetnum,tree->upt->id,tree->upt->name);
    cnt++;
    if(tree->left!=NULL&&cnt<5)
        Print_top5_user(tree->left);
}


void Top5_tw_word()
{
    BST* tree_wd=NULL;
    for(int i=0;i<1000;i++)
    {
        Adj_h_w* tmp = hs_w[i]->next;
        if(tmp==NULL)
            continue;
        while(1)
        {
            tree_wd = BST_insert_wd(tree_wd,tmp->wpt);
            if(tmp->next==NULL)
                break;
            else
                tmp=tmp->next;
        }
    }
    cnt=0;
    Print_top5_wd(tree_wd);
}

void Top5_tw_user()
{
    BST* tree_user = NULL;
    for(int i=0;i<1000;i++)
    {
        Adj_h_u* tmp = hs_u[i]->next;
        if(tmp==NULL)
            continue;
        while(1)
        {
            tree_user = BST_insert_user(tree_user,tmp->upt);
            if(tmp->next==NULL)
                break;
            else
                tmp=tmp->next;
        }
    }
    cnt=0;
    Print_top5_user(tree_user);
}


Adj_h_w* Find_w_inhash(char* tmp)
{
    int hs_w_index=0;
    for(int i=strlen(tmp)-1;i>=0;i--)
    {
        hs_w_index*=10;
        hs_w_index+=tmp[i];
    }
    hs_w_index%=1000;
    while(hs_w_index<0)
        hs_w_index+=1000;
    if(hs_w[hs_w_index]->next==NULL)
    {
        return NULL;
    }
    Adj_h_w* tmp_w=hs_w[hs_w_index]->next; //hashing 에 해당하는 index 참조


    while(tmp_w)
    {
        if(strcmp(tmp_w->wpt->wd,tmp)==0)
        {
            return tmp_w;
        }
        tmp_w=tmp_w->next;
    }
    return NULL;
}


Adj_h_u* Find_u_inhash(char* tmp)
{

    int hs_u_index = ((tmp[strlen(tmp)-1]+16*tmp[strlen(tmp)-2]+256*tmp[strlen(tmp)-3]))%1000;
    if(hs_u_index<0)
        hs_u_index+=1000;

    if(hs_u[hs_u_index]->next==NULL)
    {
        return NULL;
    }

    Adj_h_u* tmp_u=hs_u[hs_u_index]->next; //hashing 에 해당하는 index 참조


    while(tmp_u)
    {
        if(strcmp(tmp_u->upt->id,tmp)==0)
        {
            return tmp_u;
        }
        tmp_u=tmp_u->next;
    }
    return NULL;
}

void Finduser_wd()
{
    char tmp[200]={0,};
    printf("단어를 입력하세요 : ");
    scanf("%s",tmp);
    strcat(tmp,"\n");
    Adj_h_w* hpt = Find_w_inhash(tmp);

    if(hpt==NULL)
    {
        printf("단어가 tweet된 적이 없습니다.\n");
        return;
    }

    int i=1;

    Adj_id* idpt = hpt->wpt->id->next;

    printf("단어를 Tweet한 사람의 목록\n");
    while(idpt)
    {
        printf("%d. id : %s",i++,idpt->id);
        if(idpt->next == NULL)
            break;
        idpt=idpt->next;
    }
}



void Find_friend_wd()
{
    char tmp[200]={0,};
    printf("단어를 입력하세요 : ");
    scanf("%s",tmp);
    strcat(tmp,"\n");
    Adj_h_w* whpt = Find_w_inhash(tmp);

    if(whpt==NULL)
    {
        printf("단어가 tweet된 적이 없습니다.\n");
        return;
    }


    Adj_id* idp = whpt->wpt->id->next;

    int i=1;
    printf("%s 를 tweet한 사람들의 친구들 목록\n",tmp);
    while(idp)
    {
        Adj_h_u* uhpt = Find_u_inhash(idp->id);
        if(uhpt==NULL)
        {
            printf("그런 사람 없습니다.\n");
            idp = idp->next;
            continue;
        }
        Adj_fr* frp = uhpt->upt->frd->next;
        if(frp==NULL)
        {
            printf("그는 친구가 없습니다.\n");
            idp = idp->next;
            continue;
        }
        while(frp)
        {
            printf("%d. %s",i++,frp->fr_id);
            frp=frp->next;
        }
        idp = idp->next;
    }

}


void Delete_mention()
{
    char tmp[200];
    printf("단어를 입력하세요 : ");
    scanf("%s",tmp);
    strcat(tmp,"\n");
    Adj_h_w* whpt = Find_w_inhash(tmp);

    if(whpt==NULL)
    {
        printf("Tweet되지 않은 단어입니다.\n");
        return;
    }

    totaltweet-=whpt->wpt->num;
    whpt->prev->next = whpt->next;
    if(whpt->next!=NULL)
    whpt->next->prev = whpt->prev;

    Adj_id* idpt = whpt->wpt->id->next;
    while(idpt)
    {
        Adj_h_u* uhpt = Find_u_inhash(idpt->id);
        Adj_w* wpt = uhpt->upt->wd->next;
        while(wpt)
        {
            if(strcmp(wpt->word,whpt->wpt->wd)==0)
            {
                wpt->prev->next = wpt->next;
                if(wpt->next!=NULL)
                    wpt->next->prev = wpt->prev;
            }
            if(wpt->next==NULL)
                break;
            wpt=wpt->next;
        }
        if(idpt->next==NULL)
            break;
        idpt=idpt->next;
    }
}

void Delete_user_mention()
{
    char tmp[200];
    printf("단어를 입력하세요 : ");
    scanf("%s",tmp);
    strcat(tmp,"\n");

    Adj_h_w* whpt = Find_w_inhash(tmp);
    if(whpt==NULL)
    {
        printf("Tweet되지 않은 단어입니다.\n");
        return;
    }

    Adj_id* idpt = whpt->wpt->id->next;

    while(idpt)
    {
        //언급한 id마다 제거, friendship감소, 유저감소
        Adj_h_u* uhpt = Find_u_inhash(idpt->id);
        totalfriendship-=uhpt->upt->frdnum;
        totaluser--;
        uhpt->prev->next = uhpt->next;
        if(uhpt->next!=NULL)
            uhpt->next->prev = uhpt->prev;
        //id가 tweet한 단어 먼저 제거
        //단어 개수감소,
        Adj_w* wdpt = uhpt->upt->wd->next;
        while(wdpt)
        {
            Adj_h_w* whpt2 = Find_w_inhash(wdpt->word);
            if(whpt2->wpt->num>0)
            {
                whpt2->wpt->num--;
                totaltweet--;
            }
            Adj_id* id_tmp = whpt2->wpt->id->next;

            while(id_tmp)
            {
                if(strcmp(id_tmp->id,idpt->id)==0)
                {
                    id_tmp->prev->next=id_tmp->next;
                    if(id_tmp->next!=NULL)
                        id_tmp->next->prev=id_tmp->prev;
                }
                if(id_tmp->next==NULL)
                    break;
                id_tmp=id_tmp->next;
            }
            if(wdpt->next==NULL)
                break;
            wdpt=wdpt->next;
        }
        //친구관계 삭제
        Adj_fr* prev_frd = uhpt->upt->frd->next;
        while(prev_frd)
        {
            Adj_h_u* prev_upht = Find_u_inhash(prev_frd->fr_id);
            Adj_fr* prevpt = prev_upht->upt->prev->next;

            while(prevpt)
            {
                if(strcmp(prevpt->fr_id,uhpt->upt->id)==0)
                {
                    prevpt->prev->next = prevpt->next;
                    if(prevpt->next!=NULL)
                        prevpt->next->prev=prevpt->prev;
                }
                if(prevpt->next==NULL)
                    break;
                prevpt=prevpt->next;
            }
            if(prev_frd->next==NULL)
                break;
            prev_frd=prev_frd->next;
        }



        Adj_fr* frd = uhpt->upt->prev->next;
        while(frd)
        {
//            printf("%s",frd->fr_id);
            Adj_h_u* fr_upht = Find_u_inhash(frd->fr_id);
            Adj_fr* frpt = fr_upht->upt->frd->next;

            while(frpt)
            {
                if(strcmp(frpt->fr_id,uhpt->upt->id)==0)
                {
                    frpt->prev->next = frpt->next;
                    if(frpt->next!=NULL)
                        frpt->next->prev = frpt->prev;
                }
                if(frpt->next==NULL)
                {
                 //   frpt->prev->next = NULL;
                    break;
                }
                frpt=frpt->next;
            }
            if(frd->next==NULL)
                break;
            frd=frd->next;

        }

        if(idpt->next==NULL)
            break;
        idpt=idpt->next;
    }
    whpt->prev->next = whpt->next;
    if(whpt->next!=NULL)
        whpt->next->prev = whpt->prev;

}






int main()
{
    FILE* frd;
    FILE* user;
    FILE* word;

    frd = fopen("friend.txt","rt");
    word = fopen("word.txt","rt");
    user = fopen("user.txt","rt");


    char id_st[50]={0,};
    char name_st[50]={0,};
    char tmp[100]={0,};


    for(int i=0;i<1000;i++)
    {
        hs_u[i] = (Adj_h_u*)malloc(sizeof(Adj_h_u));
        init_Adj_h_u(hs_u[i]);

        hs_w[i] = (Adj_h_w*)malloc(sizeof(Adj_h_w));
        init_Adj_h_w(hs_w[i]);
    }

    while(!feof(user))
    {
        fgets(id_st,50,user);
        fgets(tmp,100,user); //날짜 저장할 필요 x
        fgets(name_st,50,user);
        fgets(tmp,100,user); // 빈줄 저장할 필요 x

        US_info* a = (US_info*)malloc(sizeof(US_info));
        init_US(a);

        strcpy(a->id,id_st);
        strcpy(a->name,name_st);

        int hs_u_index=((id_st[strlen(id_st)-1])+16*(id_st[strlen(id_st)-2])+256*(id_st[strlen(id_st)-3]))%1000; // hash를 id를 100으로 나눈 나머지를 index로 취하므로, id는 9자리니까 char형을 int형으로 바꾸어 저장
        if(hs_u_index<0)
            hs_u_index+=1000;

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

    char word_st[200];

    while(!feof(word))
    {
        fgets(id_st,50,word);
        fgets(tmp,100,word); //날짜 저장 x
        fgets(word_st,200,word);
        fgets(tmp,100,word); //빈줄 저장 x

        int hs_u_index=((id_st[strlen(id_st)-1])+16*(id_st[strlen(id_st)-2])+256*(id_st[strlen(id_st)-3]))%1000; // hash를 id를 100으로 나눈 나머지를 index로 취하므로, id는 9자리니까 char형을 int형으로 바꾸어 저장

        if(hs_u_index<0)
            hs_u_index+=1000;

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

        int hs_w_index=0;
        for(int i=strlen(word_st)-1;i>=0;i--)
        {
            hs_w_index*=10;
            hs_w_index+=word_st[i];
        }
        hs_w_index%=1000;
        while(hs_w_index<0)
            hs_w_index+=1000;




        if(hs_w[hs_w_index]->next==NULL)
        {
            WD_info* a = (WD_info*)malloc(sizeof(WD_info));
            init_WD(a);
            strcpy(a->wd,word_st); // vertex 생성 및 단어 넣기

            add_Adj_id(a->id,id_st); // word 사용한 id linked list로 넣기
            add_hash_w(hs_w[hs_w_index], a);

            totaltweet++;
            continue;
        }

        Adj_h_w* tmp_w=hs_w[hs_w_index]->next; //hashing 에 해당하는 index 참조

        while(1)
        {
            if(strcmp(tmp_w->wpt->wd,word_st)==0) //해당하는 index에 이전에 이 단어가 나온적이 있으면, user id추가, num+1 해줌
            {
                int state=0;
                tmp_w->wpt->num++;
                Adj_id* idtmp = tmp_w->wpt->id->next;
                while(idtmp)
                {
                    if(strcmp(idtmp->id,id_st)==0)
                    {
                        state=1;
                        break;
                    }
                    idtmp = idtmp->next;
                }
                if(state==0)
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
    char fr_id_st[50]={0,};

    while(!feof(frd))
    {
        fgets(fr_id_st,sizeof(id_st),frd); //  친구 id
        fgets(id_st,sizeof(fr_id_st),frd); // 주체 id
        fgets(tmp,50,frd); //빈줄 저장x


        Adj_h_u* tmp_u = Find_u_inhash(id_st);
        if(tmp_u == NULL)
            continue;

        int state=0;

        Adj_fr* frpt = tmp_u->upt->frd->next;
        while(frpt)
        {
            if(strcmp(frpt->fr_id,fr_id_st)==0)
            {
                state=1;
                break;
            }
            frpt=frpt->next;
        }
        if(state==0)
        {
            tmp_u->upt->frdnum++;
            add_Adj_fr(Find_u_inhash(id_st)->upt->frd, fr_id_st);
            add_Adj_fr(Find_u_inhash(fr_id_st)->upt->prev,id_st);
            totalfriendship++;
        }
    }

    int nmenu;



    while(1) // Interface
    {
        printf("0. Read data files\n");
        printf("1. display statistics\n");
        printf("2. Top 5 most tweeted words\n");
        printf("3. Top 5 most tweeted users\n");
        printf("4. Find users who tweeted a word (e.g., ’연세대’)\n");
        printf("5. Find all people who are friends of the above users\n");
        printf("6. Delete all mentions of a word\n");
        printf("7. Delete all users who mentioned a word\n");
        printf("8. Find strongly connected components\n");
        printf("9. Find shortest path from a given user\n");
        printf("99. Quit\n");
        printf("Select Menu: ");
        scanf("%d",&nmenu);

        if(nmenu==0)
            RDF(totaluser,totaltweet,totalfriendship);
        else if(nmenu==1)
            DisplaySt(totaluser,totaltweet,totalfriendship);
        else if(nmenu==2)
            Top5_tw_word();
        else if(nmenu==3)
            Top5_tw_user();
        else if(nmenu==4)
            Finduser_wd();
        else if(nmenu==5)
            Find_friend_wd();
        else if(nmenu==6)
            Delete_mention();
        else if(nmenu==7)
            Delete_user_mention();
        else if(nmenu==99)
        {
            printf("프로그램을 종료합니다.\n");
            break;
        }
        else
        {
            printf("다시 입력해주세요.\n");
        }
    }
}
